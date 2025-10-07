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
#include "LookupSnapshots.h"
#include "Snapshots.h"
#include <Database/Database.h>
#include <Database/SnapshotsDB.h>
#include <QMessageBox>
#include <App/AppRoot.h>
#include <App/AppGcs.h>
const QString sWarningCaption = QObject::tr("Warning");
//=============================================================================
LookupSnapshots::LookupSnapshots(Snapshots *snapshots)
    : DatabaseSnapshotLookup(snapshots,
                     "lookup",
                     tr("Screenshots"),
                     tr("Control"),
                     Database::instance()->snapshots)
    , snapshots(snapshots)
    , m_recordId(0)
    , m_recordsCount(0)
    , m_recordNum(0)
{
    setFilterEnabled(false);
    /*if(parent){
        setParentFact(qobject_cast<FactBase *>(parent));
    }*/

    dbModel()->qmlMapSafe = true;
    dbModel()->ordered = true;


    connect(this, &LookupSnapshots::areaChanged, this, &LookupSnapshots::updateRect);
    connect(this, &LookupSnapshots::areaChanged, this, &LookupSnapshots::updateStatus);
    connect(this, &DatabaseSnapshotLookup::itemTriggered, this, &LookupSnapshots::itemTriggered);
    connect(this, &DatabaseSnapshotLookup::itemClicked, this, &LookupSnapshots::itemSelect);
    connect(this->dbModel(),&DatabaseLookupModel::synced,this,&LookupSnapshots::updateStatus);


    connect(this, &LookupSnapshots::recordIdChanged, this, &LookupSnapshots::updateStatus);
    connect(this, &LookupSnapshots::recordTriggered, this, &LookupSnapshots::updateStatus);
    connect(this, &LookupSnapshots::recordMapMove, this, &LookupSnapshots::updateStatus);
    connect(this, &LookupSnapshots::recordsCountChanged, this, &LookupSnapshots::updateStatus);
    connect(this, &LookupSnapshots::recordNumChanged, this, &LookupSnapshots::updateStatus);
    connect(this, &LookupSnapshots::recordSelectedChanged, this, &LookupSnapshots::updateStatus);

    connect(this, &LookupSnapshots::sig_updateSorting, this, &LookupSnapshots::updateSorting);
    connect(this, &LookupSnapshots::sig_updateFiltring, this, &LookupSnapshots::updateFiltring);

    connect(this, &LookupSnapshots::sort_orderChanged, this, &LookupSnapshots::updateSorting);
    connect(this, &LookupSnapshots::sort_criteriaChanged, this, &LookupSnapshots::updateSorting);
    connect(this, &LookupSnapshots::filter_distanceChanged, this, &LookupSnapshots::updateFiltring);
    connect(this, &LookupSnapshots::filter_dateBeginChanged, this, &LookupSnapshots::updateFiltring);
    connect(this, &LookupSnapshots::filter_dateEndChanged, this, &LookupSnapshots::updateFiltring);
    connect(this, &LookupSnapshots::filter_dateEnabledChanged, this, &LookupSnapshots::updateFiltring);
    connect(this, &LookupSnapshots::filterChanged, this, &LookupSnapshots::updateFiltring);



   /* f_sortMode = new Fact(this,
                          "sort_mode",
                          tr("sort_mode"),
                          tr("sort_mode"),
                          Enum | PersistentValue,
                          "fit-to-page-outline");
    f_sortMode->setEnumStrings({tr("Fit"), tr("Scale"), tr("Full")});
    f_sortMode->setDefaultValue(tr("Full"));*/

    f_move_to = new Fact(this,
                        "moveto",
                        tr("Move to"),
                        tr("Move to selected screenshots"),
                        Action | ShowDisabled | Apply | IconOnly,
                         "map-marker-question-outline");

    f_move_to->setEnabled(false);

    f_open_folder = new Fact(this,
                        "open_folder",
                        tr("Open snaphots folder"),
                        tr("Open snaphots folder"),
                        Action | ShowDisabled | Apply | IconOnly,
                         "folder-open");

    f_open_folder->setEnabled(true);

    f_edit_scr = new Fact(this,
                        "edit_scr",
                        tr("Edit"),
                        tr("Edit selected screenshots"),
                        Action | ShowDisabled | Apply | IconOnly,
                         "content-save-edit");

    f_edit_scr->setEnabled(false);

    f_move_to_scr_pos = new Fact(this,
                                 "move",
                                 tr("Return"),
                                 tr("Return selected screenshots to screenshot position"),
                                 Action | Apply | CloseOnTrigger | ShowDisabled | IconOnly,
                                 "image-move");

    f_move_to_scr_pos->setEnabled(false);

    f_open = new Fact(this,
                         "open_scr",
                         tr("Open"),
                         tr("Open selected screenshots"),
                         Action | ShowDisabled | Apply | IconOnly,
                         "movie-open-outline");

    f_open->setEnabled(false);

    f_select_all = new Fact(this,
                        "select_all",
                        tr("Select all"),
                        tr("Select all visible screenshots"),
                        Action | ShowDisabled | Apply | IconOnly,
                        "select-all");

    f_select_all->setEnabled(true);

    f_unselect_all = new Fact(this,
                        "unselect_all",
                        tr("Unselect all"),
                        tr("Unselect all visible screenshots"),
                        Action | ShowDisabled | Apply | IconOnly,
                        "select-off");

    f_unselect_all->setEnabled(false);

    f_remove = new Fact(this,
                        "remove",
                        tr("Remove"),
                        tr("Remove selected screenshots"),
                        Action | ShowDisabled | Remove | IconOnly,
                        "delete");
    f_remove->setEnabled(false);


    connect(f_remove, &Fact::triggered, this, &LookupSnapshots::dbRemove);
    connect(f_move_to, &Fact::triggered, this, &LookupSnapshots::goToMap);
    connect(f_edit_scr, &Fact::triggered, this, &LookupSnapshots::edit_scr);
    connect(f_open, &Fact::triggered, this, &LookupSnapshots::openSnapshots);
    connect(f_move_to_scr_pos, &Fact::triggered, this, &LookupSnapshots::move_to_scr_pos);
    connect(f_open_folder, &Fact::triggered, this, &LookupSnapshots::openFolder);
    connect(f_select_all, &Fact::triggered, this, &LookupSnapshots::select_all);
    connect(f_unselect_all, &Fact::triggered, this, &LookupSnapshots::unselect_all);

    disconnect(db, &DatabaseSession::modified, this, nullptr);
}
void LookupSnapshots::updateSorting()
{
    defaultLookup();
}
void LookupSnapshots::updateFiltring()
{
    defaultLookup();
}
//=============================================================================
int LookupSnapshots::getSortOrder(){
   return sort_order_value();
}
int LookupSnapshots::getSortCriteria(){
    return sort_criteria_value();
}
QString LookupSnapshots::getFilterDistance(){
    return filter_distance();
}
QString LookupSnapshots::getFilterDateBegin(){
    return filter_date_begin();
}
QString LookupSnapshots::getFilterDateEnd(){
    return filter_date_end();
}
bool LookupSnapshots::getFilterDateEnabled(){
    return filter_date_enabled();
}
QString LookupSnapshots::getFilter()
{
    return filter();
}
void LookupSnapshots::setQuantity(int value){
    set_quantity(value);
}
void LookupSnapshots::setLowestDateTime(QDateTime date){
   set_lowest_date_time(date);
}

bool LookupSnapshots::fixItemDataThr(QVariantMap *item)
{
    mutex.lock();
    QGeoCoordinate rc = reqCenter;
    //double rd = reqDist;
    mutex.unlock();
    double dist = 0;
    QGeoCoordinate c(item->value("lat").toDouble(), item->value("lon").toDouble());
    if (rc.isValid()) {
        dist = rc.distanceTo(c);
        /*if (dist > rd)
            return false;*/
        item->insert("status", dist);

        if(selectedItems().contains(item->value("key").toULongLong())){
           item->insert("active", true);
        }else{
           item->insert("active", false);
        }


        QString key = item->value("filename").toString();
        QString qsPath = AppDirs::previews().absolutePath();
        QString filename = QString("%1/%2.%3").arg(qsPath, key, "png");
        if(!QFile::exists(filename)){
            QByteArray ba = item->value("file_preview").toByteArray();
            QBuffer buffer(&ba);
            QImageReader reader(&buffer,"PNG");
            QImage image = reader.read();
            if(image.save(filename))
                 apxMsg() <<  tr("Restore preview") <<  filename   << tr("from database");
        }
    }
    return true;
}
//=============================================================================
void LookupSnapshots::updateRect()
{
    QGeoRectangle r = m_area.boundingGeoRectangle();
    mutex.lock();
    reqCenter = r.center();
    mutex.unlock();
    if ((!r.isValid()) || r.width() <= 0 || r.height() <= 0)
        return;
    double sz = qMax(1.0, qMax(r.width(), r.height()) * 2.0);
    r.setWidth(sz);
    r.setHeight(sz);
    while (!reqRect.isEmpty()) {
        double d = qMax(reqRect.width() / r.width(), reqRect.height() / r.height());
        if (d > 8.0)
            break;
        if (reqRect.contains(r)) {
            reloadQueryResults();
            return;
        }
        break;
    }
    r.setWidth(sz * 2.0);
    r.setHeight(sz * 2.0);
    reqRect = r;
    mutex.lock();
    reqDist = r.topLeft().distanceTo(r.bottomRight());
    mutex.unlock();
    defaultLookup();
}
void LookupSnapshots::itemSelect(QVariantMap modelData,bool isChecked)
{
    quint64 key = modelData.value("key", 0).toUInt();
    if (!key)
        return;
    setRecordId(key);
    setSelectedItem(key,isChecked);
    emit recordTriggered(recordId());

    if(!isChecked){
        if(selectedItems().size()){
           setRecordId(selectedItems().values().at(0));
        }else{
            setRecordId(0);
        }
    }
    reloadQueryResults();
}

void LookupSnapshots::itemTriggered(QVariantMap modelData,bool isChecked)
{
    Q_UNUSED(isChecked)
    quint64 key = modelData.value("key", 0).toUInt();
    if (!key)
        return;
    setRecordId(key);
    //setSelectedItem(key,isChecked);

    //emit recordTriggered(recordId());
    emit recordMapMove(recordId());
    //reloadQueryResults(); //setArea used
}

void LookupSnapshots::updateStatus()
{
    if(f_remove){
        if(selectedItems().size()){
            f_remove->setEnabled(true);
        }else{
            f_remove->setEnabled(false);
        }
    }

    if(f_move_to){
        if(!selectedItems().size() ||
                selectedItems().size() > 1){
            f_move_to->setEnabled(false);
        }else if(selectedItems().size() == 1){
            quint64 key = selectedItems().values().at(0);
            int i = dbModel()->indexOf("key", key);
            QVariantMap modelData = dbModel()->get(i);
            if(!modelData.isEmpty()){
                if(!modelData["status"].toInt()){
                    f_move_to->setEnabled(false);
                }else{
                   f_move_to->setEnabled(true);
                }
            }
        }
    }

    if(f_edit_scr){
        if(!selectedItems().size() ||
                selectedItems().size() > 1){
            f_edit_scr->setEnabled(false);
        }else if(selectedItems().size() == 1){
            quint64 key = selectedItems().values().at(0);
            int i = dbModel()->indexOf("key", key);
            QVariantMap modelData = dbModel()->get(i);
            if(!modelData.isEmpty()){
                /*if(!modelData["status"].toInt()){
                    f_edit_scr->setEnabled(false);
                }else{
                   f_edit_scr->setEnabled(true);
                }*/
                f_edit_scr->setEnabled(true);
            }
        }
    }

    if(f_select_all){
        if(selectedItems().size() &&
                selectedItems().size() == dbModel()->getKeys().size()){
            f_select_all->setEnabled(false);
        }else{
            f_select_all->setEnabled(true);
        }
    }

    if(f_unselect_all){
        if(!selectedItems().size()){
            f_unselect_all->setEnabled(false);
        }else{
            f_unselect_all->setEnabled(true);
        }
    }

    if(f_open){
        if(!selectedItems().size() ||
                selectedItems().size() > 5){
            f_open->setEnabled(false);
        }else{
            f_open->setEnabled(true);
        }
    }

    if(f_move_to_scr_pos){
        if(!selectedItems().size()){
            f_move_to_scr_pos->setEnabled(false);
        }else{
            bool bState = false;
            for(int i = 0; i < selectedItems().size(); i++){
                quint64 key = selectedItems().values().at(i);
                int ii = dbModel()->indexOf("key", key);
                QVariantMap modelData = dbModel()->get(ii);
                if(!modelData.isEmpty()){
                    double lat = modelData["lat"].toDouble();
                    double lon = modelData["lon"].toDouble();
                    double lat_scr = modelData["lat_scr"].toDouble();
                    double lon_scr = modelData["lon_scr"].toDouble();
                    if( lat != lat_scr ||
                            lon !=lon_scr){
                        bState = true;
                    }
                }
            }
            f_move_to_scr_pos->setEnabled(bState);
        }
    }

    /*if(f_open_folder){
        if(!selectedItems().size()){
            f_open_folder->setEnabled(false);
        }else{
            f_open_folder->setEnabled(true);
        }
    }*/

    if((int)recordsCount() != quantity()){
        setStatus(QString("%1/%2/%3").arg(selectedItems().size()).arg(recordsCount()).arg(quantity()));
    }else{
        setStatus(QString("%1/%2").arg(selectedItems().size()).arg(recordsCount()));
    }
}

void LookupSnapshots::updSorting()
{
   emit sig_updateSorting();
}
void LookupSnapshots::updFiltring()
{
   emit sig_updateFiltring();
}
void LookupSnapshots::select_all()
{
    QList<quint64> list = dbModel()->getKeys();
    QListIterator<quint64> i(list);
    while (i.hasNext()){
        quint64 v = i.next();
        int i = dbModel()->indexOf("key", v);
        QVariantMap modelData = dbModel()->get(i);
        if(!modelData.isEmpty()){
            //triggerItem(modelData,true);
            clickItem(modelData,true);
        }
    }
}
void LookupSnapshots::unselect_all()
{
    QList<quint64> list = dbModel()->getKeys();
    QListIterator<quint64> i(list);
    while (i.hasNext()){
        quint64 v = i.next();
        int i = dbModel()->indexOf("key", v);
        QVariantMap modelData = dbModel()->get(i);
        if(!modelData.isEmpty()){
            //triggerItem(modelData,false);
             clickItem(modelData,false);
        }
    }

}
void LookupSnapshots::goToMap()
{
    quint64 key = recordId();
    if (!key)
        return;

    int i = dbModel()->indexOf("key", key);
    if (i < 0) {
        return;
    }
    QVariantMap item = dbModel()->get(i);
    if(!item.isEmpty()){
        if(selectedItems().contains(key)){
            triggerItem(item,true);
        }else{
           triggerItem(item,false);
        }
    }

   // defaultLookup();
}
void LookupSnapshots::edit_scr()
{
    quint64 key = recordId();
    if (!key)
        return;

    int i = dbModel()->indexOf("key", key);
    if (i < 0) {
        return;
    }
    QVariantMap item = dbModel()->get(i);
    if(!item.isEmpty() &&
            selectedItems().contains(key)){
        createEditor(item);
    }
}
void LookupSnapshots::move_to_scr_pos()
{
    foreach (quint64 key, selectedItems()) {
        int i = dbModel()->indexOf("key", key);
        if (i < 0) {

        }else{
            QVariantMap item = dbModel()->get(i);
            if(!item.isEmpty()){
                double lat = item["lat"].toDouble();
                double lon = item["lon"].toDouble();
                double lat_scr = item["lat_scr"].toDouble();
                double lon_scr = item["lon_scr"].toDouble();
                if( lat != lat_scr ||
                        lon !=lon_scr){
                    item["lat"] = item["lat_scr"];
                    item["lon"] = item["lon_scr"];
                    moveSnapshot(item);
                }
            }
        }
    }
}
void LookupSnapshots::openSnapshots()
{
    foreach (quint64 key, selectedItems()) {
        int i = dbModel()->indexOf("key", key);
        if (i < 0) {

        }else{
            QVariantMap item = dbModel()->get(i);
            if(!item.isEmpty()){
                //snapshotShow(item);
                showSnapshot(item);
            }
        }
    }
}
void LookupSnapshots::openFolder()
{
    QString qs_path = AppDirs::images().absolutePath();
    qs_path+="/";
    showFolder(qs_path);
}
void LookupSnapshots::dbRemove()
{
    /*quint64 key = recordId();
    if (!key)
        return;
    emit recordDeleteted(recordId());*/

    QString qsWarning =  tr("You are going to delete selected screenshot");
    qsWarning += " ";
    qsWarning += QString::number(selectedItems().size());
    qsWarning +=  tr("pts.");
    qsWarning += "\n";
    qsWarning += tr("Are you sure?");

    if(QMessageBox::warning(nullptr,
                            sWarningCaption,
                            qsWarning,
                            tr("Yes"),
                            tr("No"),
                            QString(),
                            0,
                            1)) return;

    auto it = selectedItems().begin();
    while (it != selectedItems().end()) {
       quint64 key = *it;
       emit recordDeleteted(key);
       setSelectedItem(key,false);
       it = selectedItems().begin();
    }
}


//=============================================================================
void LookupSnapshots::defaultLookup()
{
    QGeoCoordinate c = reqRect.center();
    QVariantList v;
    //const QString s = QString("%%%1%%").arg(filter());
   // set_filter(filter());
    bool bDesc = sort_order_value();
    bool bDistance = sort_criteria_value();
    QString qQuery;

    v << c.latitude();
    v << c.latitude();
    v << std::pow(std::cos(qDegreesToRadians(c.latitude())), 2);
    v << c.longitude();
    v << c.longitude();

    if(!reqRect.isEmpty()){
        qQuery = "SELECT *,";
        qQuery += " ((?-lat)*(?-lat)*?)+((?-lon)*(?-lon))";
        qQuery += " AS dist";
        qQuery += " FROM Snapshots";
        if(bDesc){
            if(bDistance){
                qQuery += " ORDER BY dist DESC";
            }else{
               qQuery += " ORDER BY filename DESC";
            }
        }else{
            if(bDistance){
                qQuery += " ORDER BY dist ASC";
            }else{
                qQuery += " ORDER BY filename ASC";
            }
        }
        query(qQuery,v);
    }else{
        qQuery = "SELECT * FROM Snapshots";
        if(bDesc){
            qQuery += " ORDER BY filename DESC";
        }else{
            qQuery += " ORDER BY filename ASC";
        }
        query(qQuery);
    }

    /*if(filter() != ""){
       if(!reqRect.isEmpty()){

           v << c.latitude();
           v << c.latitude();
           v << std::pow(std::cos(qDegreesToRadians(c.latitude())), 2);
           v << c.longitude();
           v << c.longitude();

           qQuery = "SELECT *,((?-lat)*(?-lat)*?)+((?-lon)*(?-lon)) AS dist FROM Snapshots";
           qQuery += " WHERE";
           qQuery += " ( title LIKE ? OR descr LIKE ?)";
           if(qSortOrder == tr("Descending")){
               if(qSortCriteria == tr("Distance")){
                   qQuery += " ORDER BY dist DESC LIMIT 100";
               }else{
                   qQuery += " ORDER BY filename DESC LIMIT 100";
               }
           }else{
               if(qSortCriteria == tr("Distance")){
                   qQuery += " ORDER BY dist ASC LIMIT 100";
               }else{
                  qQuery += " ORDER BY filename ASC LIMIT 100";
               }
           }
           query(qQuery,QVariantList() << v << s << s);
       }else{
           qQuery = "SELECT * FROM Snapshots";
           qQuery += " WHERE";
           qQuery += " ( title LIKE ? OR descr LIKE ?)";
           if(qSortOrder == tr("Descending")){
               if(qSortCriteria == tr("Distance")){
                   qQuery += " ORDER BY dist DESC LIMIT 100";
               }else{
                   qQuery += " ORDER BY filename DESC LIMIT 100";
               }
           }else{
               if(qSortCriteria == tr("Distance")){
                   qQuery += " ORDER BY dist ASC LIMIT 100";
               }else{
                   qQuery += " ORDER BY filename ASC LIMIT 100";
               }
           }
           query(qQuery,QVariantList() << s << s);
       }

    }else if(!reqRect.isEmpty()){

        v << c.latitude();
        v << c.latitude();
        v << std::pow(std::cos(qDegreesToRadians(c.latitude())), 2);
        v << c.longitude();
        v << c.longitude();

        qQuery = "SELECT *,";
        qQuery += " ((?-lat)*(?-lat)*?)+((?-lon)*(?-lon))";
        qQuery += " AS dist";
        qQuery += " FROM Snapshots";
        if(qSortOrder == tr("Descending")){
            if(qSortCriteria == tr("Distance")){
                qQuery += " ORDER BY dist DESC LIMIT 100";
            }else{
               qQuery += " ORDER BY filename DESC LIMIT 100";
            }
        }else{
            if(qSortCriteria == tr("Distance")){
                qQuery += " ORDER BY dist ASC LIMIT 100";
            }else{
                qQuery += " ORDER BY filename ASC LIMIT 100";
            }
        }

        query(qQuery,v);
    }*/
}

QGeoShape LookupSnapshots::area() const
{
    return m_area;
}
void LookupSnapshots::setArea(const QGeoShape &v)
{
    if (m_area == v)
        return;
    m_area = v;
    emit areaChanged();
}
quint64 LookupSnapshots::recordId()
{
    QMutexLocker lock(&mutexRecordId);
    return m_recordId;
}

QSet<quint64> LookupSnapshots::selectedItems(){
    QMutexLocker lock(&mutexSelected);
    return m_selected_items;
}

void LookupSnapshots::setSelectedItem(quint64 v, bool bState)
{
    mutexSelected.lock();
    auto it = m_selected_items.find(v);
    if(it != m_selected_items.end()){
        if(!bState){
            m_selected_items.remove(v);
            mutexSelected.unlock();
            emit recordIdChanged();
            return;
        }
    }else{
        if(bState){
            m_selected_items << v;
            mutexSelected.unlock();
            emit recordIdChanged();
            return;
        }
    }
    mutexSelected.unlock();
}
int LookupSnapshots::sort_order_value()
{
    QMutexLocker lock(&mutexOrderValue);
    return m_sort_order_value;
}
int LookupSnapshots::sort_criteria_value()
{
    QMutexLocker lock(&mutexCriteria);
    return m_sort_criteria_value;
}
QString LookupSnapshots::sort_order()
{
    QMutexLocker lock(&mutexOrder);
    return m_sort_order;
}
QString LookupSnapshots::sort_criteria()
{
    QMutexLocker lock(&mutexCriteria);
    return m_sort_criteria;
}
QString LookupSnapshots::filter_distance()
{
    QMutexLocker lock(&mutexDistance);
    return m_filter_distance;
}
QString LookupSnapshots::filter_date_begin()
{
    QMutexLocker lock(&mutexDateBegin);
    return m_filter_date_begin;
}
QString LookupSnapshots::filter_date_end()
{
    QMutexLocker lock(&mutexDateEnd);
    return m_filter_date_end;
}
bool LookupSnapshots::filter_date_enabled()
{
    QMutexLocker lock(&mutexDateEnabled);
    return m_filter_date_enabled;
}
QString LookupSnapshots::filter()
{
    QMutexLocker lock(&mutexFilter);
    return m_filter;
}
int LookupSnapshots::quantity()
{
    QMutexLocker lock(&mutexQuantity);
    return m_quantity;
}
QDateTime LookupSnapshots::lowest_date_time()
{
    QMutexLocker lock(&mutexQuantity);
    return m_lowest_date_time;
}

void LookupSnapshots::set_sort_order_value(int value)
{
    mutexOrderValue.lock();
    if(m_sort_order_value != value){
        m_sort_order_value = value;
        mutexOrderValue.unlock();
        return;
    }else{
        mutexOrderValue.unlock();
        return;
    }
}
void LookupSnapshots::set_sort_order(QString value)
{
    mutexOrder.lock();
    if(m_sort_order != value){
        m_sort_order = value;
        mutexOrder.unlock();
        emit sort_orderChanged();
        return;
    }else{
        mutexOrder.unlock();
        return;
    }
}

void LookupSnapshots::set_sort_criteria_value(int value)
{
    mutexCriteriaValue.lock();
    if(m_sort_criteria_value != value){
        m_sort_criteria_value = value;
        mutexCriteriaValue.unlock();
        return;
    }else{
        mutexCriteriaValue.unlock();
        return;
    }
}

void LookupSnapshots::set_sort_criteria(QString value)
{
    mutexCriteria.lock();
    if(m_sort_criteria != value){
        m_sort_criteria = value;
        mutexCriteria.unlock();
        emit sort_criteriaChanged();
        return;
    }else{
        mutexCriteria.unlock();
        return;
    }
}

void LookupSnapshots::set_filter_distance(QString qsText)
{
    mutexDistance.lock();
    if(m_filter_distance != qsText){
        m_filter_distance = qsText;
        mutexDistance.unlock();
        emit filter_distanceChanged();
        return;
    }else{
        mutexDistance.unlock();
        return;
    }
}

void LookupSnapshots::set_filter_date_begin(QDateTime time)
{
    mutexDateBegin.lock();
    if(m_filter_date_begin != time.toString("dd MM yyyy hh:mm:ss") ){
        m_filter_date_begin = time.toString("dd MM yyyy hh:mm:ss");
        mutexDateBegin.unlock();
        emit filter_dateBeginChanged();
        return;
    }else{
        mutexDateBegin.unlock();
        return;
    }
}

void LookupSnapshots::set_filter_date_end(QDateTime time)
{
    mutexDateEnd.lock();
    if(m_filter_date_end != time.toString("dd MM yyyy hh:mm:ss") ){
        m_filter_date_end = time.toString("dd MM yyyy hh:mm:ss");
        mutexDateEnd.unlock();
        emit filter_dateEndChanged();
        return;
    }else{
        mutexDateEnd.unlock();
        return;
    }
}
void LookupSnapshots::set_filter_date_enabled(bool state)
{
    mutexDateEnabled.lock();
    if(m_filter_date_enabled != state ){
        m_filter_date_enabled = state;
        mutexDateEnabled.unlock();
        emit filter_dateEnabledChanged();
        return;
    }else{
        mutexDateEnabled.unlock();
        return;
    }
}

void LookupSnapshots::set_filter(QString text)
{
    mutexFilter.lock();
    if(m_filter != text ){
        m_filter = text;
        mutexFilter.unlock();
        emit filterChanged();
        return;
    }else{
        mutexFilter.unlock();
        return;
    }
}
void LookupSnapshots::set_quantity(int i)
{
    mutexQuantity.lock();
    if(m_quantity != i ){
        m_quantity = i;
        mutexQuantity.unlock();
        emit quantityChanged();
        return;
    }else{
        mutexQuantity.unlock();
        return;
    }
}

void LookupSnapshots::set_lowest_date_time(QDateTime date)
{
    mutexDateTime.lock();
    if(m_lowest_date_time != date ){
        m_lowest_date_time = date;
        mutexDateTime.unlock();
        emit lowest_date_timeChanged(m_lowest_date_time);
        return;
    }else{
        mutexDateTime.unlock();
        return;
    }
}


void LookupSnapshots::setRecordId(quint64 v)
{
    mutexRecordId.lock();
    if (m_recordId == v) {
        mutexRecordId.unlock();
        return;
    }
    m_recordId = v;
    mutexRecordId.unlock();
    emit recordIdChanged();
}
void LookupSnapshots::setRecordsCount(quint64 v)
{
    if (m_recordsCount == v)
        return;
    m_recordsCount = v;
    emit recordsCountChanged();
}
quint64 LookupSnapshots::recordNum() const
{
    return m_recordNum;
}
quint64 LookupSnapshots::recordsCount() const
{
    return m_recordsCount;
}
