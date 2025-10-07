#include "LookupRelativePoints.h"
#include "RelativePoints.h"
#include <Database/Database.h>
#include <Database/RelativePointsDB.h>

#include <QMessageBox>

const QString sWarningCaption = QObject::tr("Warning");
LookupRelativePoints::LookupRelativePoints(RelativePoints *relativePoints)
    : DatabaseRelativePointsLookup(relativePoints,
                     "lookup",
                     tr("Relative points"),
                     tr("Control"),
                     Database::instance()->relativePoints)
    , relativePoints(relativePoints)
    , m_recordId(0)
    , m_recordsCount(0)
    , m_recordNum(0)
{
    setFilterEnabled(false);
    dbModel()->qmlMapSafe = true;
    dbModel()->ordered = true;


    connect(this, &LookupRelativePoints::areaChanged, this, &LookupRelativePoints::updateRect);
    connect(this, &LookupRelativePoints::areaChanged, this, &LookupRelativePoints::updateStatus);
    connect(this, &DatabaseRelativePointsLookup::itemTriggered, this, &LookupRelativePoints::itemTriggered);
    connect(this, &DatabaseRelativePointsLookup::itemClicked, this, &LookupRelativePoints::itemSelect);
    connect(this->dbModel(),&DatabaseLookupModel::synced,this,&LookupRelativePoints::updateStatus);


    connect(this, &LookupRelativePoints::recordIdChanged, this, &LookupRelativePoints::updateStatus);
    connect(this, &LookupRelativePoints::recordTriggered, this, &LookupRelativePoints::updateStatus);
    connect(this, &LookupRelativePoints::recordMapMove, this, &LookupRelativePoints::updateStatus);
    connect(this, &LookupRelativePoints::recordsCountChanged, this, &LookupRelativePoints::updateStatus);
    connect(this, &LookupRelativePoints::recordNumChanged, this, &LookupRelativePoints::updateStatus);
    connect(this, &LookupRelativePoints::recordSelectedChanged, this, &LookupRelativePoints::updateStatus);
    connect(this, &LookupRelativePoints::filterChanged, this, &LookupRelativePoints::updateFiltring);

    //connect(this, &LookupRelativePoints::sig_updateSorting, this, &LookupRelativePoints::updateSorting);

   // connect(this, &LookupRelativePoints::sort_orderChanged, this, &LookupRelativePoints::updateSorting);


    f_move_to = new Fact(this,
                        "moveto",
                        tr("Move to"),
                        tr("Move to selected relative point"),
                        Action | ShowDisabled | Apply | IconOnly,
                         "map-marker-question-outline");

    f_move_to->setEnabled(false);

    f_edit = new Fact(this,
                        "edit_relative_point",
                        tr("Edit"),
                        tr("Edit selected relative points"),
                        Action | ShowDisabled | Apply | IconOnly,
                         "content-save-edit");

    f_edit->setEnabled(false);

    f_show_all = new Fact(this,
                         "show_all",
                         tr("Show all"),
                         tr("Show all relative points on the map"),
                         Action | ShowDisabled | Apply | IconOnly,
                         "bullseye");
    f_show_all->setEnabled(true);


    f_select_all = new Fact(this,
                        "select_all",
                        tr("Select all"),
                        tr("Select all visible relative points"),
                        Action | ShowDisabled | Apply | IconOnly,
                        "select-all");
    f_select_all->setEnabled(true);

    f_unselect_all = new Fact(this,
                        "unselect_all",
                        tr("Unselect all"),
                        tr("Unselect all visible relative points"),
                        Action | ShowDisabled | Apply | IconOnly,
                        "select-off");

    f_unselect_all->setEnabled(false);

    f_remove = new Fact(this,
                        "remove",
                        tr("Remove"),
                        tr("Remove selected relative points"),
                        Action | ShowDisabled | Remove | IconOnly,
                        "delete");
    f_remove->setEnabled(false);


    connect(f_remove, &Fact::triggered, this, &LookupRelativePoints::dbRemove);
    connect(f_move_to, &Fact::triggered, this, &LookupRelativePoints::goToMap);
    connect(f_edit, &Fact::triggered, this, &LookupRelativePoints::edit);
    connect(f_select_all, &Fact::triggered, this, &LookupRelativePoints::select_all);
    connect(f_show_all, &Fact::triggered, this, &LookupRelativePoints::show_all);
    connect(f_unselect_all, &Fact::triggered, this, &LookupRelativePoints::unselect_all);

    disconnect(db, &DatabaseSession::modified, this, nullptr);
}
void LookupRelativePoints::updateSorting()
{
    defaultLookup();
}
void LookupRelativePoints::updateFiltring()
{
    defaultLookup();
}
//=============================================================================

void LookupRelativePoints::setQuantity(int value)
{
    set_quantity(value);
}

void LookupRelativePoints::setQuantityMap(int value)
{
    set_quantity_on_map(value);
}

QString LookupRelativePoints::getFilter()
{
    return filter();
}

int LookupRelativePoints::getSortOrder()
{
   return sort_order_value();
}

bool LookupRelativePoints::fixItemDataThr(QVariantMap *item)
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
    }
    return true;
}
//=============================================================================
void LookupRelativePoints::updateRect()
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
void LookupRelativePoints::itemSelect(QVariantMap modelData,bool isChecked)
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

void LookupRelativePoints::itemTriggered(QVariantMap modelData,bool isChecked)
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

void LookupRelativePoints::updateStatus()
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

    if(f_edit){
        if(!selectedItems().size() ||
                selectedItems().size() > 1){
            f_edit->setEnabled(false);
        }else if(selectedItems().size() == 1){
            quint64 key = selectedItems().values().at(0);
            int i = dbModel()->indexOf("key", key);
            QVariantMap modelData = dbModel()->get(i);
            if(!modelData.isEmpty()){
                f_edit->setEnabled(true);
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

    if((int)recordsCount() != quantity()){
        setStatus(QString("%1/%2/%3/%4").arg(selectedItems().size()).arg(quantity_on_map()).arg(recordsCount()).arg(quantity()));
    }else{
        setStatus(QString("%1/%2/%3").arg(selectedItems().size()).arg(quantity_on_map()).arg(recordsCount()));
    }
}

void LookupRelativePoints::show_all()
{
    Fact *f =  qobject_cast<Fact *>(sender());
    if (!f)
        return;
    int value = 0;
    if(f->dataType() == Apply){
       f->setDataType(Stop);
       value = 1;
       f->setDescr(tr("Disable all relative points on the map"));
    }else if(f->dataType() == Stop){
        f->setDataType(Apply);
        f->setDescr(tr("Show all relative points on the map"));
     }

    QList<quint64> list = dbModel()->getKeys();
    QListIterator<quint64> i(list);
    while (i.hasNext()){
        quint64 v = i.next();
        int i = dbModel()->indexOf("key", v);
        QVariantMap modelData = dbModel()->get(i);
        if(!modelData.isEmpty()){
            modelData["disp_on_map"]=value;
            emit recordEditedted(modelData);
        }
    }
}

void LookupRelativePoints::select_all()
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
void LookupRelativePoints::unselect_all()
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
void LookupRelativePoints::goToMap()
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
void LookupRelativePoints::edit()
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

void LookupRelativePoints::dbRemove()
{
    /*quint64 key = recordId();
    if (!key)
        return;
    emit recordDeleteted(recordId());*/

    QString qsWarning =  tr("You are going to delete selected relative points");
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
void LookupRelativePoints::defaultLookup()
{
    QGeoCoordinate c = reqRect.center();
    QVariantList v;

    bool bDesc = sort_order_value();
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
        qQuery += " FROM RelativePoints";
        if(bDesc){
                qQuery += " ORDER BY dist DESC";
        }else{
                qQuery += " ORDER BY dist ASC";
        }
        query(qQuery,v);
    }else{
        qQuery = "SELECT * FROM RelativePoints";
        if(bDesc){
            qQuery += " ORDER BY key DESC";
        }else{
            qQuery += " ORDER BY key ASC";
        }
        query(qQuery);
    }
}

QGeoShape LookupRelativePoints::area() const
{
    return m_area;
}
void LookupRelativePoints::setArea(const QGeoShape &v)
{
    if (m_area == v)
        return;
    m_area = v;
    emit areaChanged();
}
quint64 LookupRelativePoints::recordId()
{
    QMutexLocker lock(&mutexRecordId);
    return m_recordId;
}

QSet<quint64> LookupRelativePoints::selectedItems(){
    QMutexLocker lock(&mutexSelected);
    return m_selected_items;
}

void LookupRelativePoints::setSelectedItem(quint64 v, bool bState)
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


int LookupRelativePoints::quantity()
{
    QMutexLocker lock(&mutexQuantity);
    return m_quantity;
}

int LookupRelativePoints::quantity_on_map()
{
    QMutexLocker lock(&mutexQuantityMap);
    return m_quantity_on_map;
}


void LookupRelativePoints::set_quantity(int i)
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

void LookupRelativePoints::set_quantity_on_map(int i)
{
    mutexQuantityMap.lock();
    if(m_quantity_on_map != i ){
        m_quantity_on_map = i;
        mutexQuantityMap.unlock();
        emit quantityOnMapChanged();
        return;
    }else{
        mutexQuantityMap.unlock();
        return;
    }
}
QString LookupRelativePoints::filter()
{
    QMutexLocker lock(&mutexFilter);
    return m_filter;
}

void LookupRelativePoints::set_filter(QString text)
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
int LookupRelativePoints::sort_order_value()
{
    QMutexLocker lock(&mutexOrderValue);
    return m_sort_order_value;
}
QString LookupRelativePoints::sort_order()
{
    QMutexLocker lock(&mutexOrder);
    return m_sort_order;
}

void LookupRelativePoints::set_sort_order_value(int value)
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

void LookupRelativePoints::set_sort_order(QString value)
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

void LookupRelativePoints::setRecordId(quint64 v)
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
void LookupRelativePoints::setRecordsCount(quint64 v)
{
    if (m_recordsCount == v)
        return;
    m_recordsCount = v;
    emit recordsCountChanged();
}
quint64 LookupRelativePoints::recordNum() const
{
    return m_recordNum;
}
quint64 LookupRelativePoints::recordsCount() const
{
    return m_recordsCount;
}
