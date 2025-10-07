/*
 * Copyright (C) 2011 Aliaksei Stratsilatau <sa@uavos.com>
 *
 * This file is part of the UAV Open System Project
 *  http://www.uavos.com/
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth
 * Floor, Boston, MA 02110-1301, USA.
 *
 */
#include "VectorTileLoader.h"
#include "VectorPlugin.h"
#include "VectorMapsDB.h"
#include <App/App.h>
#include <App/AppDirs.h>
#include <App/AppLog.h>
#include <App/AppSettings.h>
#include <Database/Database.h>
#include <Fact/Fact.h>
#include <QImage>
#include <QPen>
#include <QFont>
#include <QPainter>

//=============================================================================
#define Random(low, high) ((int) (low + qrand() % (high - low)))
//=============================================================================

void VectorTileLoader::loadMapLog(const std::string &msg)
{
    apxConsole() << tr("Loading extra map: ") << msg.c_str();
    QApplication::processEvents();
}


VectorTileLoader *VectorTileLoader::_instance = nullptr;
VectorTileLoader::VectorTileLoader(Fact *parent)
    : Fact(parent,
           QString(PLUGIN_NAME).toLower(),
           tr("Tile loader"),
           tr("Map tiles loader service"),
           Group,
           "earth")
{
    _instance = this;
    f_prefs = nullptr;
    f_loadExtraMap = nullptr;
    f_loadSXF = nullptr;
    db = new VectorMapsDB(this, QLatin1String("VectorPluginDbSession"));

    connect(db, &VectorMapsDB::tileLoaded, this, &VectorTileLoader::tileLoaded);
    connect(db, &VectorMapsDB::tileNotExists, this, &VectorTileLoader::render);

    connect(this, &VectorTileLoader::requestCountChanged, this, &VectorTileLoader::updateStatus);
    connect(App::instance(), &App::loadingFinished, this, &VectorTileLoader::createSettingsFact);
    initMap();
}
VectorTileLoader::~VectorTileLoader()
{
    abort();
}
//=============================================================================
void VectorTileLoader::updateStatus()
{
    int cnt = requestCount();
    setStatus(cnt > 0 ? QString::number(cnt) : "");
    setProgress(cnt > 0 ? 0 : -1);
}
//=============================================================================
//=============================================================================
void VectorTileLoader::loadTile(quint64 uid)
{
    ////////////////////////////////////////////////////
    (new DBReqLoadTile(db, type(uid), dbHash(uid), uid))->exec();
    //db->reqLoadTile(type(uid),dbHash(uid),uid);
}
void VectorTileLoader::loadCancel(quint64 uid)
{
    Q_UNUSED(uid)
    //qDebug()<<"Cancel: "<<uid;
}
//=============================================================================
bool VectorTileLoader::checkImage(const QByteArray &data)
{
    QImage image;
    image.loadFromData(data);
    if (image.isNull() || image.size() != QSize(256, 256)) {
        return false;
    }
    return true;
}
//=============================================================================


void VectorTileLoader::abort()
{
    foreach (QNetworkReply *reply, reqMap.keys()) {
        reply->abort();
        emit tileError(reqMap.value(reply), reply->errorString());
    }
    reqMap.clear();
    downloads.clear();
}
//=============================================================================
void VectorTileLoader::render(quint64 uid)
{    
    quint8 _level = level(uid);
    quint32 _x =  x(uid);
    quint32 _y = y(uid);

    QImage *image;
    m_ki->GetTile(_level,_x, _y, image);

    QByteArray data;
    QBuffer buffer(&data);
    buffer.open(QIODevice::WriteOnly);
    image->save(&buffer,"jpg",100);

    emit tileLoaded(uid,data);

    if (!checkImage(data)) {
        apxConsoleW() << "Error rendering map (not an image)";
    } else {
        //db->reqSaveTile(type(uid),dbHash(uid),data);
        (new DBReqSaveTile(db, type(uid), dbHash(uid), versionMaps.toUInt(), data))->exec();
    }

    delete image;
}


bool VectorTileLoader::initMap()
{    
    QWidget *mapWidget = new QWidget();
    mapWidget->setMinimumSize(QSize(256,256));
    QSizePolicy sp_retain = mapWidget->sizePolicy();
    sp_retain.setRetainSizeWhenHidden(true);
    mapWidget->setSizePolicy(sp_retain);
    mapWidget->hide();

    m_ki  = QSharedPointer<MKartaInterface>(new MKartaInterface(mapWidget));
    m_config = QSharedPointer<MConfig>(new MConfig(mapWidget, m_ki.data(), false));

    QFileInfo map_ini(AppDirs::maps().absoluteFilePath("map.ini"));
    if (!map_ini.exists())
    {
        if (! QFile::exists("/usr/share/gcs/map/map.ini")) return false;
         AppDirs::user().mkpath(AppDirs::maps().absolutePath());
         QFile::copy("/usr/share/gcs/map/map.ini",AppDirs::maps().absoluteFilePath("map.ini"));
    }

    RegisterOutputProgress(&VectorTileLoader::loadMapLog);

    m_config->initMap(false, AppDirs::maps().absoluteFilePath("map.ini").toStdString().c_str());

    connect(m_config.data(), SIGNAL(closeClicked()), SIGNAL(m_sConfigChanged()));
    connect(m_config.data(), SIGNAL(mapPainted()), SIGNAL(m_sConfigChanged()));

    MCoo *cooSys = CreateCooByType(MString("SPM"));
    cooSys->SetStandartEllipsoid(ELLIPSOID_WGS_84);
    cooSys->setPressfactor(0);

    convertCooSys(m_ki->m_karta->m_pObjectCoo,cooSys,m_ki.data());
    m_ki->DrawMap(false); 
    return true;
}


void VectorTileLoader::convertCooSys(MCoo *from, MCoo *to, MKartaInterface *ki)
{
    MRect rect = ki->m_zoombounds;
    MPoint ptLeftBottom (rect.left, rect.bottom);
    MPoint ptRightTop(rect.right, rect.top);
    double h;
    from->ToBLH(ptLeftBottom, 0, ptLeftBottom, h);
    from->ToBLH(ptRightTop, 0, ptRightTop, h);
    to->FromBLH(ptLeftBottom, 0, ptLeftBottom, h);
    to->FromBLH(ptRightTop, 0, ptRightTop, h);

    ki->m_karta->ConvertKartaToCoo(to);
    ki->m_karta->b_converted = true;
    rect = MRect(ptLeftBottom, ptRightTop);
    rect.NormalizeGeoRect();
    ki->ZoomTo(rect);
}

void VectorTileLoader::createSettingsFact()
{
    Fact *millsionPlaner{nullptr};
    millsionPlaner = AppRoot::instance()->findChild("apx.tools.missionplanner");    
    foreach (FactBase *a, millsionPlaner->actions()) {
        if(static_cast<Fact *>(a)->name() == QStringLiteral("prefs"))
            f_prefs = static_cast<Fact *>(a);
    }

    if (!f_prefs) return;

    Fact *f_provider = f_prefs->child("provider");

    connect(f_prefs, &Fact::triggered, this, &VectorTileLoader::updateLoadExtraMapAction);
    connect(f_provider, &Fact::valueChanged, this, &VectorTileLoader::changeToolsVisibility);

    Fact *sxfLayers;
     sxfLayers = new Fact(f_prefs,
                        "sxflayers",
                        tr("Layers"),
                        "",
                        PersistentValue,"layers-outline");

     updateLayersFact();
     connect(this, &VectorTileLoader::mapFilesChanged,this, &VectorTileLoader::updateLayersFact );

     f_loadExtraMap = new Fact(f_prefs, "loadExtraMap", tr("Load Extra Map"), tr("Load Extra Map"), Action, "file-import");
     connect(f_loadExtraMap, &Fact::triggered, this, &VectorTileLoader::loadExtraMap);

     f_loadSXF = new Fact(f_prefs, "loadSXF", tr("Load SXF"), tr("Load SXF"), Action, "file-import");
     connect(f_loadSXF, &Fact::triggered, this, &VectorTileLoader::loadSXF);
}

void VectorTileLoader::updateLayersFact()
{
    Fact *sxfLayers = f_prefs->child("sxflayers");
    if (!sxfLayers) return;

    sxfLayers->removeAll();
    QStringList layerNames{};
    const QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
    MLayerList *m_laylist= &m_ki->m_karta->m_laylist;
    LAYITER it = m_laylist->begin();
    while (it != m_laylist->end())
    {     // i++;

        QString lyrName=codec->toUnicode((*it)->m_name.c_str());

        if ( layerNames.indexOf(lyrName)== -1)
        {
            layerNames.append(lyrName);
            Fact * layer = new Fact(sxfLayers,
                            QString(lyrName).replace(" ",""),
                            lyrName,
                            "",
                            Bool);
              layer->setValue((*it)->x_visible);
              connect(layer, &Fact::valueChanged, this, &VectorTileLoader::layerVisibilityUpdate);
       }
       it++;
    }
}

void VectorTileLoader::layerVisibilityUpdate()
{
    Fact *sxfLayers = f_prefs->child("sxflayers");
    if (!sxfLayers) return;

    const QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
    MLayerList *m_laylist= &m_ki->m_karta->m_laylist;
    LAYITER it = m_laylist->begin();
    while (it != m_laylist->end())
    {     // i++;
        QString layerName=codec->toUnicode((*it)->m_name.c_str());
        Fact *f_sxfLayer = sxfLayers->childByTitle(layerName);
        if (f_sxfLayer)
        {
           (*it)->x_visible = f_sxfLayer->value().toBool();
        }
        it++;
    }
    m_config->saveConfig();
    (new DBReqClearCache(db))->exec();
    emit requestEngieClearCache();
}

void VectorTileLoader::updateLoadExtraMapAction()
{
    QGeoCoordinate c(
         App::propertyValue<QGeoCoordinate>("apx.tools.missionplanner.clickCoordinate"));
    bool isLoaded = isExtraMapLoaded(c);
    MRect mapRect;
    QString m_utpSearch =  GetSheetAtPoint(100, MPoint(c.longitude(), c.latitude()), mapRect).c_str();
    if ( f_loadExtraMap  )
    {
        f_loadExtraMap->setIcon(isLoaded ? "file-document-box-remove":"file-import");
        QString act = isLoaded ? tr("Remove ") : tr("Load ");
        f_loadExtraMap->setTitle(act + m_utpSearch);
        f_loadExtraMap->setDescr(m_utpSearch);
    }

}

bool VectorTileLoader::isExtraMapLoaded(const QGeoCoordinate &c)
{
    MPoint geoCoord(c.longitude(), c.latitude());
    MRect mapRect;
    QString m_utpSearch =  GetSheetAtPoint(100, geoCoord, mapRect).c_str();

    QString wildFile = m_utpSearch + ".utp " + m_utpSearch + ".UTP";

    const MLayerList m_laylist = m_ki->m_karta->m_laylist;
    ConstLAYITER it = m_laylist.begin();
    QString utpFile;
    const QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
    while(it != m_laylist.end())
    {
        const QString sheetName = codec->toUnicode((*it)->m_sheetname);
        const int searchIndex = sheetName.indexOf(m_utpSearch);
        if(searchIndex >= 0)
        {
            //take the name of the sheet =  path name
            //of the sheet to the sheet - catalog - ".utp"
            const QString sCheck = sheetName.mid(searchIndex,
                                                 sheetName.length() - searchIndex - 4);
            if(sCheck.length() != m_utpSearch.length())
            {
                it++;
                continue;
            }
            utpFile = sheetName;
            break;
        }
        it++;
    }

    return !utpFile.isEmpty();
}


void VectorTileLoader::loadExtraMap()
{
    slotLoadMapInPoint();
    updateLoadExtraMapAction();
    emit mapFilesChanged();
    (new DBReqClearCache(db))->exec();    
    emit requestEngieClearCache();

}

QString VectorTileLoader::getMapPath(const int& scale, const QString& wildFile)
{

    QString path = "/usr/share/gcs/map/" + QString::number(scale);
    const QDir dir(path, wildFile);
    QString utpFile;
        if (dir.count() == 1)
        {
            utpFile= path + "/" + dir[0];
        }
        else
        {
            utpFile = QFileDialog::getOpenFileName(nullptr,
                                                   tr("Select ..."), QDir::homePath(), wildFile);
        }

        return utpFile;
}

void VectorTileLoader::slotLoadMapInPoint()
{
        QGeoCoordinate c(
             App::propertyValue<QGeoCoordinate>("apx.tools.missionplanner.clickCoordinate"));

        apxConsole() << tr("Request load extra map: ") << c.toString();
        MPoint geoCoord(c.longitude(), c.latitude());
        MRect mapRect;
        QString m_utpSearch =  GetSheetAtPoint(100, geoCoord, mapRect).c_str();

        QString wildFile = m_utpSearch + ".utp " + m_utpSearch + ".UTP";

        const MLayerList m_laylist = m_ki->m_karta->m_laylist;
        ConstLAYITER it = m_laylist.begin();
        QString utpFile;
        const QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
        while(it != m_laylist.end())
        {
            const QString sheetName = codec->toUnicode((*it)->m_sheetname);
            const int searchIndex = sheetName.indexOf(m_utpSearch);
            if(searchIndex >= 0)
            {
                //take the name of the sheet =  path name
                //of the sheet to the sheet - catalog - ".utp"
                const QString sCheck = sheetName.mid(searchIndex,
                                                     sheetName.length() - searchIndex - 4);
                if(sCheck.length() != m_utpSearch.length())
                {
                    it++;
                    continue;
                }
                utpFile = sheetName;
                break;
            }
            it++;
        }


        if(utpFile.isEmpty())
        {
           QString path = getMapPath(100,wildFile);
           if (path.isEmpty())
           {
               apxConsole() << tr("The path to the map file is empty");
               return;
           }
           m_ki->AddSheet(path.toStdString());
        }
        if(utpFile.length())
        {
             m_ki->DeleteSheet(codec->fromUnicode(utpFile).data());
        }
     m_config->saveConfig();
}

void VectorTileLoader::changeToolsVisibility()
{
    Fact *f_provider = f_prefs->child("provider");
    bool ifCurrentSXF = f_provider->value().toString().startsWith("SXF");
    Fact *f_ptr;
    f_ptr = f_prefs->child("sxflayers");
    if (f_ptr) f_ptr->setVisible(ifCurrentSXF);

    if (f_loadExtraMap) f_loadExtraMap->setVisible(ifCurrentSXF);
    if (f_loadSXF) f_loadSXF->setVisible(ifCurrentSXF);

    f_ptr = f_prefs->child("maptype");
    if (f_ptr) f_ptr->setVisible(!ifCurrentSXF);
}


void VectorTileLoader::loadSXF()
{

    QString sxfFile = QFileDialog::getOpenFileName(nullptr,
                                           tr("Select ..."), QDir::homePath(), tr("SXF binary file (*.sxf)"));

    QFileInfo sxfFileInfo(sxfFile);

    if (!sxfFileInfo.exists()) return;

    MKarta *utpMap = new MKarta();
    utpMap->ConnectKlassifikator(m_ki->GetKlassifikator());

    MTable		m_CompareFontTable, m_CompareTable, m_AtrValueSooTable;
    MInArray	m_AtrSooTable, m_OrdAtrList;
    QString strCmpPath("/usr/share/gcs/map/sxf.cmp");
    QString strCmfPath("/usr/share/gcs/map/font_table.cmf");

    int CmpFileVer=0;


    if(!m_config->readCmp(strCmpPath, m_CompareTable, m_AtrValueSooTable, m_AtrSooTable, m_OrdAtrList, CmpFileVer))
    {
        return;
    }


    if(!m_config->readCmf(strCmfPath, m_CompareFontTable))
    {
        return;
    }

    m_config->loadSXF(sxfFile,&m_CompareTable, &m_AtrValueSooTable, &m_AtrSooTable, &m_OrdAtrList, &m_CompareFontTable, CmpFileVer,
            27, utpMap);

    QString utpfile = QDir::tempPath() + "/" + sxfFileInfo.baseName() + ".utp";
    FILE *fl; fl=fopen(utpfile.toUtf8(), "wb");
    utpMap-> WriteBin(fl);
    fclose(fl);
    if (QFileInfo(utpfile).exists())
    {
        m_ki->AddSheet(utpfile.toStdString());
        emit mapFilesChanged();
        (new DBReqClearCache(db))->exec();
        emit requestEngieClearCache();
    }
}
