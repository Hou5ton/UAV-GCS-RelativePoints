#include "DatabaseRelativePointsLookup.h"
#include "DatabaseSession.h"
#include <qalgorithms.h>
#include <App/AppRoot.h>

DatabaseRelativePointsLookup::DatabaseRelativePointsLookup(Fact *parent,
                                                           const QString &name,
                                                           const QString &title,
                                                           const QString &descr,
                                                           DatabaseSession *db,
                                                           Flags flags) : Fact(parent,
                                                                               name,
                                                                               title,
                                                                               descr,
                                                                               flags,
                                                                               "database-search")
  , db(db)
  , m_filterEnabled(true)
{
    setQmlPage("Menu/FactMenuPageLookupRelativePoint.qml");

    m_dbModel = new DatabaseLookupModel(this);
    connect(this,
            &DatabaseRelativePointsLookup::_itemsLoaded,
            this,
            &DatabaseRelativePointsLookup::loadRecordsItems,
            Qt::QueuedConnection);

    connect(db,
            &DatabaseSession::modified,
            this,
            &DatabaseRelativePointsLookup::defaultLookup,
            Qt::QueuedConnection);

    connect(this, &Fact::triggered, this, &DatabaseRelativePointsLookup::defaultLookup);



    connect(this, &DatabaseRelativePointsLookup::filterChanged, this, &DatabaseRelativePointsLookup::defaultLookup);


    Fact *f_tools = new Fact(this,
                             "tools",
                             tr("Tools"),
                             tr("Database maintenance"),
                             Action | IconOnly,
                             "wrench");
    f_tools->bind(db);

    modelSyncTimer.setSingleShot(true);
    modelSyncTimer.setInterval(500);
    connect(&modelSyncTimer, &QTimer::timeout, this, &DatabaseRelativePointsLookup::loadItems);
}

void DatabaseRelativePointsLookup::query(const QString &queryString, const QVariantList &bindValues)
{
    //qDebug()<<"updateQuery"<<queryString<<bindValues;
    DatabaseRequest *req = new DatabaseRequest(db, queryString, bindValues);
    query(req);
}
void DatabaseRelativePointsLookup::query(DatabaseRequest *req)
{
    connect(req,
            &DatabaseRequest::queryResults,
            this,
            &DatabaseRelativePointsLookup::loadQueryResults,
            Qt::DirectConnection);
    req->exec();
}

void DatabaseRelativePointsLookup::loadRecordsItems(DatabaseLookupModel::ItemsList list)
{
    recordsItems = list;
    modelSyncTimer.start();
}
void DatabaseRelativePointsLookup::loadItems()
{
    dbModel()->syncItems(recordsItems);
}

void DatabaseRelativePointsLookup::defaultLookup() {}

void DatabaseRelativePointsLookup::loadQueryResults(DatabaseRequest::Records records)
{
    loadMutex.lock();
    this->records = records;
    loadMutex.unlock();
    reloadQueryResults();
}
void DatabaseRelativePointsLookup::triggerItem(QVariantMap modelData,bool isChecked)
{
    emit itemTriggered(modelData,isChecked);
}
void DatabaseRelativePointsLookup::clickItem(QVariantMap modelData,bool isChecked)
{
    emit itemClicked(modelData,isChecked);
}

void DatabaseRelativePointsLookup::showRelativePoint(QVariantMap modelData){
    emit relativePointShow(modelData);
}
void DatabaseRelativePointsLookup::editRelativePoint(QVariantMap modelData){
    emit relativePointEdit(modelData);
}
void DatabaseRelativePointsLookup::createEditor(QVariantMap modelData){
    emit editorCreated(modelData);
}
void DatabaseRelativePointsLookup::setQuantity(int value)
{
    Q_UNUSED(value);
}
void DatabaseRelativePointsLookup::setQuantityMap(int value)
{
    Q_UNUSED(value);
}
void DatabaseRelativePointsLookup::setSelectedItem(quint64 v, bool bState)
{
    Q_UNUSED(v)
    Q_UNUSED(bState)
}
bool DatabaseRelativePointsLookup::fixItemDataThr(QVariantMap *item)
{
    Q_UNUSED(item)
    return true;
}
void DatabaseRelativePointsLookup::setRecordsCount(quint64 v)
{
    Q_UNUSED(v)
}
DatabaseLookupModel *DatabaseRelativePointsLookup::dbModel() const
{
    return m_dbModel;
}
QString DatabaseRelativePointsLookup::filter() const
{
    return m_filter;
}
void DatabaseRelativePointsLookup::setFilter(const QString &v)
{
    if (m_filter == v)
        return;
    m_filter = v;
    emit filterChanged();
}
bool DatabaseRelativePointsLookup::filterEnabled() const
{
    return m_filterEnabled;
}
void DatabaseRelativePointsLookup::setFilterEnabled(const bool &v)
{
    if (m_filterEnabled == v)
        return;
    m_filterEnabled = v;
    emit filterEnabledChanged();
}
int DatabaseRelativePointsLookup::getSortOrder()
{
   return 0;
}
QString DatabaseRelativePointsLookup::getFilter()
{
   return "";
}
bool sort_distance_asc_(const QVariantMap &m1, const QVariantMap &m2){

    QString qs1 = m1.value("status").toString();
    QString qs2 = m2.value("status").toString();
    return qs1.toDouble() < qs2.toDouble();
}

bool sort_distance_desc_(const QVariantMap &m1, const QVariantMap &m2){

    QString qs1 = m1.value("status").toString();
    QString qs2 = m2.value("status").toString();
    return qs1.toDouble() > qs2.toDouble();

}

void DatabaseRelativePointsLookup::reloadQueryResults()
{
    //run in thread
    loadMutex.lock();
    const QStringList &n = records.names;
    DatabaseLookupModel::ItemsList list;
    int count = 0;
    QDateTime dateTime = QDateTime::currentDateTime();
    for (int i = 0; i < records.values.size(); ++i) {
        const QVariantList &r = records.values.at(i);

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
        if(m.value("disp_on_map").toInt()){
            count++;
        }
    }
    loadMutex.unlock();
    setQuantity(list.size());
    setQuantityMap(count);

    QString qsFilter = getFilter();
    if(qsFilter != ""){
        QList<QVariantMap>::iterator it = list.begin();
        while (it != list.end()) {
           QString qsLat =  AppRoot::latToString((it->value("lat").toDouble()));
           QString qsLon =  AppRoot::lonToString((it->value("lon").toDouble()));

            if (it->value("title").toString().contains(qsFilter) ||
                it->value("descr").toString().contains(qsFilter) ||
                it->value("alt").toString().contains(qsFilter) ||
                    qsLat.contains(qsFilter) ||
                    qsLon.contains(qsFilter) ||
                it->value("key").toString().contains(qsFilter)||
                it->value("status").toString().contains(qsFilter)){
                    ++it;
            }else{
                it = list.erase(it);
            }
        }
    }

    bool bDesc = getSortOrder();
    if(bDesc){
        std::sort(list.begin(), list.end(), sort_distance_desc_);
    }else{
        std::sort(list.begin(),list.end(),sort_distance_asc_);
    }

    emit _itemsLoaded(list);
    setRecordsCount(list.isEmpty() ? 0 : list.size());
}
