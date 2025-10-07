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
#include "DatabaseSnapshotLookup.h"
#include "DatabaseSession.h"
#include <qalgorithms.h>
#include <App/AppRoot.h>
//=============================================================================
DatabaseSnapshotLookup::DatabaseSnapshotLookup(Fact *parent,
                               const QString &name,
                               const QString &title,
                               const QString &descr,
                               DatabaseSession *db,
                               Flags flags)
    : Fact(parent, name, title, descr, flags, "database-search")
    , db(db)
    , m_filterEnabled(true)
{
    setQmlPage("Menu/FactMenuPageLookupSnapshot.qml");

    m_dbModel = new DatabaseLookupModel(this);
    connect(this,
            &DatabaseSnapshotLookup::_itemsLoaded,
            this,
            &DatabaseSnapshotLookup::loadRecordsItems,
            Qt::QueuedConnection);

    connect(this, &DatabaseSnapshotLookup::filterChanged, this, &DatabaseSnapshotLookup::defaultLookup);
    connect(db,
            &DatabaseSession::modified,
            this,
            &DatabaseSnapshotLookup::defaultLookup,
            Qt::QueuedConnection);

    connect(this, &Fact::triggered, this, &DatabaseSnapshotLookup::defaultLookup);

    Fact *f_tools = new Fact(this,
                             "tools",
                             tr("Tools"),
                             tr("Database maintenance"),
                             Action | IconOnly,
                             "wrench");
    f_tools->bind(db);

    modelSyncTimer.setSingleShot(true);
    modelSyncTimer.setInterval(500);
    connect(&modelSyncTimer, &QTimer::timeout, this, &DatabaseSnapshotLookup::loadItems);
}
//=============================================================================
void DatabaseSnapshotLookup::query(const QString &queryString, const QVariantList &bindValues)
{
    //qDebug()<<"updateQuery"<<queryString<<bindValues;
    DatabaseRequest *req = new DatabaseRequest(db, queryString, bindValues);
    query(req);
}
void DatabaseSnapshotLookup::query(DatabaseRequest *req)
{
    connect(req,
            &DatabaseRequest::queryResults,
            this,
            &DatabaseSnapshotLookup::loadQueryResults,
            Qt::DirectConnection);
    req->exec();
}
void DatabaseSnapshotLookup::loadQueryResults(DatabaseRequest::Records records)
{
    loadMutex.lock();
    this->records = records;
    loadMutex.unlock();
    reloadQueryResults();
}

bool sort_distance_asc(const QVariantMap &m1, const QVariantMap &m2){

    QString qs1 = m1.value("status").toString();
    QString qs2 = m2.value("status").toString();
    return qs1.toDouble() < qs2.toDouble();
}

bool sort_distance_desc(const QVariantMap &m1, const QVariantMap &m2){

    QString qs1 = m1.value("status").toString();
    QString qs2 = m2.value("status").toString();
    return qs1.toDouble() > qs2.toDouble();

}
bool sort_date_asc(const QVariantMap &m1, const QVariantMap &m2){

    QString qs1 = m1.value("filename").toString();
    QString qs2 = m2.value("filename").toString();
    return qs1 < qs2;
}
bool sort_date_desc(const QVariantMap &m1, const QVariantMap &m2){

    QString qs1 = m1.value("filename").toString();
    QString qs2 = m2.value("filename").toString();
    return qs1 > qs2;
}


void DatabaseSnapshotLookup::reloadQueryResults()
{
    //run in thread
    loadMutex.lock();
    const QStringList &n = records.names;
    DatabaseLookupModel::ItemsList list;
    QDateTime dateTime = QDateTime::currentDateTime();
    for (int i = 0; i < records.values.size(); ++i) {
        const QVariantList &r = records.values.at(i);
        QDateTime dateTime_  = QDateTime::fromString(r.at(11).toString(),"dd_MM_yyyyThh_mm_ss_zzz");
        if(dateTime_.isValid() &&
                dateTime_ < dateTime){
           dateTime =  dateTime_;
        }
        //create item data
        QVariantMap m;
        for (int j = 0; j < n.size(); ++j) {
            const QString &fn = n.at(j);
            if (m.contains(fn))
                continue;
            m.insert(fn, r.value(j));
        }
        if (!fixItemDataThr(&m))
            break;
        list.append(m);
    }
    loadMutex.unlock();
    setLowestDateTime(dateTime);
    setQuantity(list.size());

    QString qsFilter = getFilter();
    if(qsFilter != ""){
        QList<QVariantMap>::iterator it = list.begin();
        while (it != list.end()) {
           QString qsLat =  AppRoot::latToString((it->value("lat_scr").toDouble()));
           QString qsLon =  AppRoot::lonToString((it->value("lon_scr").toDouble()));

            if (it->value("title").toString().contains(qsFilter) ||
                it->value("descr").toString().contains(qsFilter) ||
                it->value("vehicle").toString().contains(qsFilter) ||
                it->value("alt").toString().contains(qsFilter) ||
                    qsLat.contains(qsFilter) ||
                    qsLon.contains(qsFilter) ||
               // it->value("lat_scr").toString().contains(qsFilter) ||
               // it->value("lon_scr").toString().contains(qsFilter) ||
                it->value("date").toString().contains(qsFilter) ||
                it->value("time").toString().contains(qsFilter) ||
                it->value("filename").toString().contains(qsFilter)||
                it->value("key").toString().contains(qsFilter)||
                it->value("status").toString().contains(qsFilter)){
                    ++it;
            }else{
                it = list.erase(it);
            }
        }
    }

    QList<QVariantMap>::iterator it = list.begin();
    double dist = getFilterDistance().toDouble() * 1000.;
    while (it != list.end()) {
        QString qs = it->value("status").toString();
        if(qs.toDouble() > dist){
            setSelectedItem(it->value("key").toInt(),false);    //unselect items
            it = list.erase(it);
        }else{
            ++it;
        }
    }

    bool bFilterDateEnabled = getFilterDateEnabled();
    QString qFilterDateBegin = getFilterDateBegin();
    QString qFilterDateEnd = getFilterDateEnd();
    if(bFilterDateEnabled &&
            qFilterDateBegin != "" &&
        qFilterDateEnd != ""){

        it = list.begin();
        while (it != list.end()) {
            QString tmp = it->value("date").toString();
            QDate file_date = QDate::fromString(it->value("date").toString(),"yyyy MMM dd");
            QTime file_time = QTime::fromString(it->value("time").toString());
            QDateTime file_date_time = QDateTime(file_date,file_time);
            QDateTime file_filter_date_begin = QDateTime::fromString(qFilterDateBegin,"dd MM yyyy hh:mm:ss");
            QDateTime file_filter_date_end = QDateTime::fromString(qFilterDateEnd,"dd MM yyyy hh:mm:ss");

            if(file_filter_date_begin.isValid() &&
                   file_filter_date_end.isValid()){
                if(file_filter_date_begin.toSecsSinceEpoch() <= file_date_time.toSecsSinceEpoch() &&
                      file_date_time.toSecsSinceEpoch() <= file_filter_date_end.toSecsSinceEpoch()){
                        ++it;
                }else{
                    setSelectedItem(it->value("key").toInt(),false);    //unselect items
                    it = list.erase(it);
                }
            }else{
                 ++it;
            }
        }
    }

    bool bDesc = getSortOrder();
    bool bDistance = getSortCriteria();
    if(bDesc){
      if(bDistance)  {
          std::sort(list.begin(), list.end(), sort_distance_desc);
      }else{
          std::sort(list.begin(),list.end(),sort_date_desc);
      }
    }else{
        if(bDistance)  {
            std::sort(list.begin(),list.end(),sort_distance_asc);
        }else{
            std::sort(list.begin(),list.end(),sort_date_asc);
        }
    }

    emit _itemsLoaded(list);
    setRecordsCount(list.isEmpty() ? 0 : list.size());


    //qDebug()<<"list.size()"<<list.size();
}
bool DatabaseSnapshotLookup::fixItemDataThr(QVariantMap *item)
{
    Q_UNUSED(item)
    return true;
}
int DatabaseSnapshotLookup::getSortOrder(){
   return 0;
}
int DatabaseSnapshotLookup::getSortCriteria(){
    return 0;
}
QString DatabaseSnapshotLookup::getFilterDistance(){
    return "";
}
QString DatabaseSnapshotLookup::getFilterDateBegin(){
    return "";
}
QString DatabaseSnapshotLookup::getFilterDateEnd(){
    return "";
}
bool DatabaseSnapshotLookup::getFilterDateEnabled(){
    return false;
}
QString DatabaseSnapshotLookup::getFilter()
{
   return "";
}
void DatabaseSnapshotLookup::setQuantity(int value)
{
    Q_UNUSED(value);
}
void DatabaseSnapshotLookup::setLowestDateTime(QDateTime date)
{
    Q_UNUSED(date);
}

void DatabaseSnapshotLookup::setRecordsCount(quint64 v)
{
    Q_UNUSED(v)
}
void DatabaseSnapshotLookup::setSelectedItem(quint64 v, bool bState)
{
    Q_UNUSED(v)
    Q_UNUSED(bState)
}
void DatabaseSnapshotLookup::loadRecordsItems(DatabaseLookupModel::ItemsList list)
{
    recordsItems = list;
    modelSyncTimer.start();
}


void DatabaseSnapshotLookup::loadItems()
{
    dbModel()->syncItems(recordsItems);
}
//=============================================================================
void DatabaseSnapshotLookup::triggerItem(QVariantMap modelData,bool isChecked)
{
    emit itemTriggered(modelData,isChecked);
}
void DatabaseSnapshotLookup::clickItem(QVariantMap modelData,bool isChecked)
{
    emit itemClicked(modelData,isChecked);
}
void DatabaseSnapshotLookup::showSnapshot(QVariantMap modelData){
    emit snapshotShow(modelData);
}
void DatabaseSnapshotLookup::moveSnapshot(QVariantMap modelData){
    emit snapshotMove(modelData);
}
void DatabaseSnapshotLookup::createEditor(QVariantMap modelData){
    emit editorCreated(modelData);
}
void DatabaseSnapshotLookup::showFolder(QString path){
    emit folderOpened(path);
}
//=============================================================================
void DatabaseSnapshotLookup::defaultLookup() {}
//=============================================================================
DatabaseLookupModel *DatabaseSnapshotLookup::dbModel() const
{
    return m_dbModel;
}
QString DatabaseSnapshotLookup::filter() const
{
    return m_filter;
}
void DatabaseSnapshotLookup::setFilter(const QString &v)
{
    if (m_filter == v)
        return;
    m_filter = v;
    emit filterChanged();
}
bool DatabaseSnapshotLookup::filterEnabled() const
{
    return m_filterEnabled;
}
void DatabaseSnapshotLookup::setFilterEnabled(const bool &v)
{
    if (m_filterEnabled == v)
        return;
    m_filterEnabled = v;
    emit filterEnabledChanged();
}
//=============================================================================
