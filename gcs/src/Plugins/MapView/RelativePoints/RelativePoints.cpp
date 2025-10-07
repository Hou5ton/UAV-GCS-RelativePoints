#include "RelativePoints.h"
#include "RelativePointEdit.h"
#include <App/App.h>
#include <Database/RelativePointsDB.h>
#include <QFileDialog>
#include <KartaInterface.h>
#include <App/AppGcs.h>
#include "sys/wait.h"
#include <QMessageBox>
const QString sWarningCaption = QObject::tr("Warning");

/**
 * @brief RelativePoints::RelativePoints
 * @param parent
 */
RelativePoints::RelativePoints(Fact *parent):
    Fact(parent, QString(PLUGIN_NAME).toLower(),
         tr("Relative points"),
         tr("Relative points on map")), f_add(nullptr)
{
    setIcon("bullseye");
    connect(App::instance(), &App::loadingFinished, this, &RelativePoints::appLoaded);

    f_lookup = new LookupRelativePoints(this);
    connect(f_lookup->dbModel(),
            &DatabaseLookupModel::synced,
            this,
            &RelativePoints::syncEditorFromModel);

    connect(f_lookup, &LookupRelativePoints::recordTriggered, this, &RelativePoints::on_trigger_record);
    connect(f_lookup, &LookupRelativePoints::recordMapMove, this, &RelativePoints::on_map_move);
    connect(f_lookup, &LookupRelativePoints::recordDeleteted, this, &RelativePoints::on_delete_record);
    connect(f_lookup, &LookupRelativePoints::recordEditedted, this, &RelativePoints::on_edit_record);
    connect(f_lookup, &LookupRelativePoints::editorCreated, this, &RelativePoints::on_editorCreated);
   // connect(f_lookup, &LookupRelativePoints::itemTriggered, this, &RelativePoints::onItemTriggered);

    //facts
    f_add_ = new RelativePointEdit(this,
                                  "add",
                                  tr("Add new point"),
                                  tr("Add new relative point"),
                                  QVariantMap());
    f_add_->setIcon("plus-circle");
    connect(f_add_, &RelativePointEdit::addTriggered, this, &RelativePoints::dbAddRelativePoint);
    connect(this,&RelativePoints::relativePointEdited,this,&RelativePoints::dbUpdateRelativePoint);

    f_lookup->set_sort_order_value(/*f_sort_order->value().toInt()*/0);

    loadQml("qrc:/" PLUGIN_NAME "/RelativePointsPlugin.qml");
}

void RelativePoints::appLoaded()
{
    Fact *fMapAdd = AppRoot::instance()->findChild("apx.tools.missionplanner.add");
    if (!fMapAdd)
        return;
    //create tool for map
    RelativePointEdit *f = new RelativePointEdit(fMapAdd,
                                                 "relativepoint",
                                                 tr("Relative point"),
                                                 "",
                                                 QVariantMap());
    f->setIcon("bullseye");
    connect(f, &RelativePointEdit::addTriggered, f_add_, &RelativePointEdit::addTriggered);
}

void RelativePoints::editRelativePoint(QVariantMap item)
{
    int i = f_lookup->dbModel()->indexOf("key", item.value("key").toULongLong());
    if (i < 0) {
        return;
    }
    if(f_lookup->dbModel()->get(i) != item){
        f_lookup->dbModel()->set(i,item);
    }

    QGeoCoordinate coordinate;
    coordinate.setLatitude(item.value("lat").toDouble());
    coordinate.setLongitude(item.value("lon").toDouble());
    const auto altitudeMatrix = AltitudeMatrix::instance();
    if (altitudeMatrix->isLoaded() &&
        altitudeMatrix->getAltitudeAt(coordinate)){
        item["alt"] = altitudeMatrix->getAltitudeAt(coordinate);
    }

    emit relativePointEdited(item);
}

void RelativePoints::dbAddRelativePoint(QVariantMap item)
{
    //qDebug()<<item;
    DBReqRelativePointsSave *req = new DBReqRelativePointsSave(item.value("title").toString(),
                                                           item.value("descr").toString(),
                                                           item.value("lat").toDouble(),
                                                           item.value("lon").toDouble(),
                                                           item.value("alt").toInt(),
                                                           item.value("disp_on_map").toInt());
    connect(req,
            &DatabaseRequest::finished,
            f_lookup,
            &DatabaseRelativePointsLookup::defaultLookup,
            Qt::QueuedConnection);
    connect(
        req,
        &DBReqRelativePointsSave::relativePointAdded,
        this,
        [](QString title) { apxMsg() << tr("Relative point added").append(':') << title; },
        Qt::QueuedConnection);
    req->exec();
}

void RelativePoints::dbUpdateRelativePoint(QVariantMap item)
{
    //qDebug()<<item;
    quint64 key = item.value("key").toULongLong();
    if (!key)
        return;
    DBReqRelativePointsSave *req = new DBReqRelativePointsSave(item.value("title").toString(),
                                                           item.value("descr").toString(),
                                                           item.value("lat").toDouble(),
                                                           item.value("lon").toDouble(),
                                                           item.value("alt").toInt(),
                                                           item.value("disp_on_map").toInt(),
                                                           key);

    connect(req,
            &DatabaseRequest::finished,
            f_lookup,
            &DatabaseRelativePointsLookup::defaultLookup,
            Qt::QueuedConnection);
    connect(
        req,
        &DBReqRelativePointsSave::relativePointModified,
        this,
        [](QString title) { apxMsg() << tr("Relative point updated").append(':') << title; },
        Qt::QueuedConnection);
    req->exec();
}

void RelativePoints::dbRemoveRelativePoint(QVariantMap item)
{
    destroyEditor(item);
    //qDebug()<<item;
    quint64 key = item.value("key").toULongLong();
    if (!key)
        return;
    DBReqRelativePointsRemove *req = new DBReqRelativePointsRemove(key);
    connect(req,
            &DatabaseRequest::finished,
            f_lookup,
            &DatabaseRelativePointsLookup::defaultLookup,
            Qt::QueuedConnection);
    connect(
        req,
        &DBReqRelativePointsRemove::relativePointRemoved,
        this,
        []() { apxMsg() << tr("Relative point removed"); },
        Qt::QueuedConnection);
    req->exec();
}

void RelativePoints::on_trigger_record(quint64 recordID)
{
    int i = f_lookup->dbModel()->indexOf("key", recordID);
    if (i < 0) {
        return;
    }
    QVariantMap item = f_lookup->dbModel()->get(i);
    //emit moveMap(item.value("key").toInt());
    //createEditor(item);
}
void RelativePoints::on_map_move(quint64 recordID)
{
    int i = f_lookup->dbModel()->indexOf("key", recordID);
    if (i < 0) {
        return;
    }
    QVariantMap item = f_lookup->dbModel()->get(i);
    emit moveMap(item.value("key").toInt());
}

void RelativePoints::on_delete_record(quint64 recordID)
{
    int i = f_lookup->dbModel()->indexOf("key", recordID);
    if (i < 0) {
        return;
    }
    QVariantMap item = f_lookup->dbModel()->get(i);
    dbRemoveRelativePoint(item);
}

void RelativePoints::on_edit_record(QVariantMap item)
{
    dbUpdateRelativePoint(item);
}

void RelativePoints::on_editorCreated(QVariantMap item)
{
    createEditor(item);
    f_add->trigger(item);
}

void RelativePoints::syncEditorFromModel()
{
    if (!f_add)
        return;
    if(f_add->modelData.value("key").toString() == ""){    //on add
        destroyEditor(f_add->modelData);
        return;
    }else{

       // qDebug()<<f_edit->modelData.value("key").toString();

        int i = f_lookup->dbModel()->indexOf("key", f_add->modelData.value("key"));
        if (i < 0) {
            destroyEditor(f_add->modelData);
            return;
        }
        QVariantMap vm = f_lookup->dbModel()->get(i);
        MPoint geoCoord(vm.value("lon").toDouble(), vm.value("lat").toDouble());
        auto sk42coord = AppGcs::instance()->mapInterface()->geoWgs84ToGaussSk42(geoCoord);
        vm["sk42x"] = std::round(sk42coord.y);
        vm["sk42y"] = std::round(sk42coord.x);
        f_add->setModelData(vm);
    }
}

void RelativePoints::createEditor(QVariantMap item)
{
    if(item.size()){
        if(f_add)
            destroyEditor(/*QVariantMap()*/f_add->modelData);

        QString qsKey = item.value("key").toString();
        QString qsDescr = tr("Edit relative point")+ ":" + item.value("key").toString();

        f_add = new RelativePointEdit(this, "edit", tr("Relative point"), qsDescr, item);
        f_add->setIcon("settings");

        connect(f_add, &RelativePointEdit::removed, this, [=]() { f_add = nullptr; });
        connect(f_add, &RelativePointEdit::removeTriggered, this, &RelativePoints::dbRemoveRelativePoint);
        connect(f_add, &RelativePointEdit::relativePointEdited, this, &RelativePoints::dbUpdateRelativePoint);
        connect(f_lookup->dbModel(),
                &DatabaseLookupModel::itemEdited,
                f_add,
                &RelativePointEdit::updateFromEditedModelData);

        f_add->setVisible(false);

        App::jsync(this);
    }
}
void RelativePoints::destroyEditor(QVariantMap item)
{
    //Q_UNUSED(item)
    if (!f_add)
        return;

    if (f_add->modelData.value("key").toULongLong() != item.value("key").toULongLong()){
         qDebug()<<f_add->modelData.value("key").toString()<<":"<<item.value("key").toString();
    }

    disconnect(f_add, &RelativePointEdit::removeTriggered, this, &RelativePoints::dbRemoveRelativePoint);
    disconnect(f_add, &RelativePointEdit::relativePointEdited, this, &RelativePoints::dbUpdateRelativePoint);
    disconnect(f_lookup->dbModel(),
            &DatabaseLookupModel::itemEdited,
            f_add,
            &RelativePointEdit::updateFromEditedModelData);

    f_add->remove();
    f_add = nullptr;

    App::jsync(this);

}

qint64  RelativePoints::currentKey() const
{
     return m_currentKey;
}
void RelativePoints::setCurrentKey(const qint64 &v)
{
    if (m_currentKey== v)
        return;
    m_currentKey = v;
    emit currentKeyChanged();
}
