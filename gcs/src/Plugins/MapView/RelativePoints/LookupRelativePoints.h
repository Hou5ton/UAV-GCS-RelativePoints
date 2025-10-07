#ifndef LOOKUPRELATIVEPOINTS_H
#define LOOKUPRELATIVEPOINTS_H

//=============================================================================
#include <Database/DatabaseRelativePointsLookup.h>
#include <QGeoCoordinate>
#include <QGeoRectangle>
#include <QtCore>

class RelativePoints;
class LookupRelativePoints : public DatabaseRelativePointsLookup
{
     Q_OBJECT

    Q_PROPERTY(QGeoShape area READ area WRITE setArea NOTIFY areaChanged)
   // Q_PROPERTY(quint64 selectedItems READ selectedItems NOTIFY recordSelectedChanged)
    Q_PROPERTY(quint64 recordId READ recordId NOTIFY recordIdChanged)
    Q_PROPERTY(quint64 recordsCount READ recordsCount NOTIFY recordsCountChanged)
    Q_PROPERTY(quint64 recordNum READ recordNum NOTIFY recordNumChanged)
    Q_PROPERTY(int quantity READ quantity NOTIFY quantityChanged)
    Q_PROPERTY(int quantity_on_map READ quantity_on_map NOTIFY quantityOnMapChanged)
    Q_PROPERTY(QString filter READ filter WRITE set_filter NOTIFY filterChanged)
    Q_PROPERTY(QString sort_order READ sort_order NOTIFY sort_orderChanged)
public:
    explicit LookupRelativePoints(RelativePoints *relativePoints);

    Fact *f_remove;
    Fact *f_sortMode;
    Fact *f_move_to;
    Fact *f_edit;
    Fact *f_select_all;
    Fact *f_show_all;
    Fact *f_unselect_all;
private:
    RelativePoints *relativePoints;
    QGeoCoordinate reqCenter;
    double reqDist;
    QGeoRectangle reqRect;
    QMutex mutex;

protected:
    bool fixItemDataThr(QVariantMap *item) override;
    void defaultLookup() override;
    void setRecordsCount(quint64 v) override;
    void setSelectedItem(quint64 v, bool bState) override;
    void setQuantity(int value) override;
    void setQuantityMap(int value) override;
    QString getFilter() override;
    int getSortOrder() override;

signals:
     void recordMapMove(quint64 recordID);
     void recordTriggered(quint64 recordID);
     void recordDeleteted(quint64 recordID);
     void recordEditedted(QVariantMap item);
     void recordIdChanged();
     void recordSelectedChanged();
     void recordNumChanged();
     void recordsCountChanged();
     void quantityChanged();
     void quantityOnMapChanged();
     void filterChanged();
     void areaChanged();
     void sort_orderChanged();

private slots:
    void updateRect();
    void itemSelect(QVariantMap modelData,bool isChecked);
    void itemTriggered(QVariantMap modelData,bool isChecked);
    void dbRemove();
    void goToMap();
    void edit();
    void updateStatus();
    void updateSorting();
    void updateFiltring();
    void select_all();
    void show_all();
    void unselect_all();

    //---------------------------------------
    // PROPERTIES
public:
    QGeoShape area() const;
    void setArea(const QGeoShape &v);
    quint64 recordId();
    void setRecordId(quint64 v);
    quint64 recordsCount() const;
    //void setRecordsCount(quint64 v);
    quint64 recordNum() const;
    void setRecordNum(quint64 v);
    QSet<quint64> selectedItems();
    int quantity();
    void set_quantity(int i);
    int quantity_on_map();
    void set_quantity_on_map(int i);
    QString filter();
     void set_filter(QString text);
     int sort_order_value();
     void set_sort_order_value(int value);
     QString sort_order();
     void set_sort_order(QString value);

private:
    QGeoShape m_area;
    QMutex mutexRecordId;
    QMutex mutexSelected;
    QMutex mutexQuantity;
    QMutex mutexQuantityMap;
    QMutex mutexFilter;
    QMutex mutexOrderValue;
    QMutex mutexOrder;

    quint64 m_recordId;
    quint64 m_recordsCount;
    quint64 m_recordNum;
    QSet<quint64> m_selected_items;
    int m_quantity;
    int m_quantity_on_map;
    QString m_filter;
    int m_sort_order_value;
    QString m_sort_order;
};

#endif // LOOKUPRELATIVEPOINTS_H
