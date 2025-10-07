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
#ifndef LookupSnapshots_H
#define LookupSnapshots_H
//=============================================================================
#include <Database/DatabaseSnapshotLookup.h>
#include <QGeoCoordinate>
#include <QGeoRectangle>
#include <QtCore>
class Snapshots;
//=============================================================================
class LookupSnapshots : public DatabaseSnapshotLookup
{
    Q_OBJECT
    Q_PROPERTY(QGeoShape area READ area WRITE setArea NOTIFY areaChanged)
   // Q_PROPERTY(quint64 selectedItems READ selectedItems NOTIFY recordSelectedChanged)
    Q_PROPERTY(quint64 recordId READ recordId NOTIFY recordIdChanged)
    Q_PROPERTY(quint64 recordsCount READ recordsCount NOTIFY recordsCountChanged)
    Q_PROPERTY(quint64 recordNum READ recordNum NOTIFY recordNumChanged)
    Q_PROPERTY(QString sort_order READ sort_order NOTIFY sort_orderChanged)
    Q_PROPERTY(QString sort_criteria READ sort_criteria NOTIFY sort_criteriaChanged)
    Q_PROPERTY(QString filter_distance READ filter_distance NOTIFY filter_distanceChanged)
    Q_PROPERTY(QString filter_date_begin READ filter_date_begin NOTIFY filter_dateBeginChanged)
    Q_PROPERTY(QString filter_date_end READ filter_date_end NOTIFY filter_dateEndChanged)
    Q_PROPERTY(QString filter_date_enabled READ filter_date_enabled NOTIFY filter_dateEnabledChanged)
    Q_PROPERTY(QString filter READ filter WRITE set_filter NOTIFY filterChanged)
    Q_PROPERTY(int quantity READ quantity NOTIFY quantityChanged)
    Q_PROPERTY(QDateTime lowest_date_time READ lowest_date_time NOTIFY lowest_date_timeChanged)


public:
    explicit LookupSnapshots(Snapshots *snapshots);

    Fact *f_remove;
    Fact *f_sortMode;
    Fact *f_open_folder;
    Fact *f_move_to;
    Fact *f_edit_scr;
    Fact *f_move_to_scr_pos;
    Fact *f_open;
    Fact *f_select_all;
    Fact *f_unselect_all;
    void updSorting();
    void updFiltring();

private:
    Snapshots *snapshots;
    QGeoCoordinate reqCenter;
    double reqDist;
    QGeoRectangle reqRect;
    QMutex mutex;

protected:
    bool fixItemDataThr(QVariantMap *item) override;
    void defaultLookup() override;
    void setRecordsCount(quint64 v) override;
    int getSortOrder() override;
    int getSortCriteria() override;
    QString getFilterDistance() override;
    QString getFilterDateBegin() override;
    QString getFilterDateEnd() override;
    bool getFilterDateEnabled() override;
    void setSelectedItem(quint64 v, bool bState) override;
    QString getFilter() override;
    void setQuantity(int value) override;
    void setLowestDateTime(QDateTime date) override;


signals:
     void recordMapMove(quint64 recordID);
     void recordTriggered(quint64 recordID);
     void recordDeleteted(quint64 recordID);
     void recordIdChanged();
     void sort_orderChanged();
     void sort_criteriaChanged();
     void filter_distanceChanged();
     void filter_dateBeginChanged();
     void filter_dateEndChanged();
     void filter_dateEnabledChanged();
     void recordSelectedChanged();
     void recordNumChanged();
     void recordsCountChanged();
     void filterChanged();
     void quantityChanged();
     void lowest_date_timeChanged(QDateTime date);

private slots:
    void updateRect();
    void itemSelect(QVariantMap modelData,bool isChecked);
    void itemTriggered(QVariantMap modelData,bool isChecked);
    void dbRemove();
    void goToMap();
    void edit_scr();
    void openSnapshots();
    void move_to_scr_pos();
    void openFolder();
    void updateStatus();
    void updateSorting();
    void updateFiltring();
    void select_all();
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
    QString sort_order();
    void set_sort_order(QString value);
    QString sort_criteria();
    void set_sort_criteria(QString value);
    int sort_order_value();
    void set_sort_order_value(int value);
    int sort_criteria_value();
    void set_sort_criteria_value(int value);
    QString filter_distance();
    void set_filter_distance(QString qsText);
    QString filter_date_begin();
    void set_filter_date_begin(QDateTime begin);
    QString filter_date_end();
    void set_filter_date_end(QDateTime end);
    bool filter_date_enabled();
    void set_filter_date_enabled(bool state);
    QString filter();
    void set_filter(QString text);
    int quantity();
    void set_quantity(int i);
    QDateTime lowest_date_time();
    void set_lowest_date_time(QDateTime date);

private:
    QGeoShape m_area;
    QMutex mutexRecordId;
    QMutex mutexSelected;
    QMutex mutexOrder;
    QMutex mutexCriteria;
    QMutex mutexOrderValue;
    QMutex mutexCriteriaValue;
    QMutex mutexDistance;
    QMutex mutexDateBegin;
    QMutex mutexDateEnd;
    QMutex mutexDateEnabled;
    QMutex mutexFilter;
    QMutex mutexQuantity;
    QMutex mutexDateTime;

    quint64 m_recordId;
    quint64 m_recordsCount;
    quint64 m_recordNum;
    QSet<quint64> m_selected_items;
    QString m_sort_order;
    QString m_sort_criteria;
    int m_sort_order_value;
    int m_sort_criteria_value;
    QString m_filter_distance;
    QString m_filter_date_begin;
    QString m_filter_date_end;
    bool m_filter_date_enabled;
    QString m_filter;
    int m_quantity;
    QDateTime m_lowest_date_time;
signals:
    void areaChanged();
    void sig_updateSorting();
    void sig_updateFiltring();
};
//=============================================================================
#endif
