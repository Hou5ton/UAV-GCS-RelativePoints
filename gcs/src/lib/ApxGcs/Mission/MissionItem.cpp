﻿/*
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
#include "MissionItem.h"
#include "MissionField.h"
#include "VehicleMission.h"

#include <App/AppRoot.h>
#include <QGeoCircle>

#include <App/AppGcs.h>
#include <KartaInterface.h>
//=============================================================================
MissionItem::MissionItem(MissionGroup *parent,
                         const QString &name,
                         const QString &title,
                         const QString &descr)
    : Fact(parent, name, title, descr, Group)
    , group(parent)
    , blockUpdateCoordinate(false)
    , m_course(0)
    , m_time(0)
    , m_distance(0)
    , m_totalDistance(0)
    , m_totalTime(0)
    , m_selected(false)
{
    setOpt("pos", QPointF(0.25, 0.5));

    f_order = new MissionField(this, "order", tr("Order"), tr("Object sequence number"), Int);
    f_order->setMin(1);
    updateOrderState();
    connect(f_order, &Fact::valueChanged, this, &MissionItem::updateOrder);
    connect(this, &Fact::numChanged, this, &MissionItem::updateOrderState);
    connect(group, &Fact::sizeChanged, this, &MissionItem::updateOrderState);

    f_latitude = new MissionField(this,
                                  "latitude",
                                  tr("Latitude"),
                                  tr("Global position latitude"),
                                  Float);
    f_latitude->setUnits("lat");
    f_longitude = new MissionField(this,
                                   "longitude",
                                   tr("Longitude"),
                                   tr("Global position longitude"),
                                   Float);
    f_longitude->setUnits("lon");

    f_sk42x = new MissionField(this, "sk42y", tr("X"), tr("Global postition X (SK42)"), Int);
    f_sk42x->setUnits("m");
    f_sk42y = new MissionField(this, "sk42x", tr("Y"), tr("Global postition Y (SK42)"), Int);
    f_sk42y->setUnits("m");

    f_remove = new Fact(this, "remove", tr("Remove"), tr("Remove mission item"), Action | Remove);
    connect(f_remove, &Fact::triggered, this, &Fact::remove);

    //active index
    if (group->f_activeIndex) {
        Fact *f_select = new Fact(this,
                                  "select",
                                  tr("Set as current"),
                                  group->f_activeIndex->descr(),
                                  Action | Apply,
                                  "target");
        connect(f_select, &Fact::triggered, this, &MissionItem::selectTriggered);

        connect(group->f_activeIndex, &Fact::valueChanged, this, [this]() {
            setActive(group->f_activeIndex->value().toInt() == num());
        });
        setActive(group->f_activeIndex->value().toInt() == num());
    }

    connect(f_latitude, &Fact::valueChanged, this, &MissionItem::updateCoordinate);
    connect(f_longitude, &Fact::valueChanged, this, &MissionItem::updateCoordinate);
    connect(f_sk42x, &Fact::valueChanged, this, &MissionItem::updateSk42Coordinate);
    connect(f_sk42y, &Fact::valueChanged, this, &MissionItem::updateSk42Coordinate);

    connect(this, &MissionItem::coordinateChanged, this, &MissionItem::updatePath);

    connect(this, &Fact::numChanged, this, &MissionItem::updatePath, Qt::QueuedConnection);

    //selection support
    connect(group->mission,
            &VehicleMission::selectedItemChanged,
            this,
            &MissionItem::updateSelected);

    //title
    connect(parent, &Fact::numChanged, this, &Fact::nameChanged);
    connect(this, &Fact::numChanged, this, &MissionItem::updateTitle);
    updateTitle();

    //status totals
    connect(this, &MissionItem::timeChanged, group, &MissionGroup::updateTime);
    connect(this, &MissionItem::distanceChanged, group, &MissionGroup::updateDistance);

    connect(this, &MissionItem::totalTimeChanged, this, &MissionItem::updateStatus);
    connect(this, &MissionItem::totalDistanceChanged, this, &MissionItem::updateStatus);

    //coordinates system
    Fact *f{nullptr};
    f = AppSettings::instance()->findChild("interface.coordinate_system");
    if (f){
        onCoordinateSystemChanged(); // set init state
        connect(f, &Fact::valueChanged, this, &MissionItem::onCoordinateSystemChanged);
    }

    updateStatus();
}
//=============================================================================
void MissionItem::hashData(QCryptographicHash *h) const
{
    for (int i = 0; i < size(); ++i) {
        child(i)->hashData(h);
    }
}
//=============================================================================
int MissionItem::missionItemType() const
{
    return group->missionItemType();
}
//=============================================================================
void MissionItem::updateTitle()
{
    setTitle(QString::number(num() + 1));
}
//=============================================================================
void MissionItem::updateStatus()
{
    uint d = totalDistance();
    uint t = totalTime();
    if ((d | t) == 0)
        setStatus(QString());
    else {
        QStringList st;
        st.append(AppRoot::distanceToString(d));
        st.append(AppRoot::timeToString(t, false));
        setStatus(st.join(' '));
    }
}
//=============================================================================
void MissionItem::updateCoordinate()
{
    if (blockUpdateCoordinate)
        return;
    setCoordinate(QGeoCoordinate(f_latitude->value().toDouble(), f_longitude->value().toDouble()));
}

void MissionItem::updateSk42Coordinate()
{
    if (blockUpdateCoordinate)
        return;
    MPoint sk42coord(f_sk42y->value().toDouble(), f_sk42x->value().toDouble());
    auto geoCoord = AppGcs::instance()->mapInterface()->gaussSk42ToGeoWgs84(sk42coord);
    setCoordinate(QGeoCoordinate(geoCoord.y, geoCoord.x));
}
//=============================================================================
void MissionItem::updatePath()
{
    setGeoPath(getPath());
    //check to propagate updates to next items
    MissionItem *next = nextItem();
    if (next) {
        QGeoCoordinate p1, p2;
        if (m_geoPath.path().size() >= 2)
            p1 = QGeoCoordinate(m_geoPath.path().last());
        if (next->geoPath().path().size() >= 2)
            p2 = QGeoCoordinate(next->geoPath().path().first());
        if (next->geoPath().path().size() < 2 || m_geoPath.path().size() < 2
            || p1.latitude() != p2.latitude() || p1.longitude() != p2.longitude()) {
            next->updatePath();
        }
    }
}
//=============================================================================
void MissionItem::updateOrder()
{
    int n = f_order->value().toInt() - 1;
    if (n < 0)
        n = 0;
    else if (n >= group->size())
        n = group->size() - 1;
    if (n == num() || group->size() < 2) {
        f_order->setValue(num() + 1);
        return;
    }
    move(n, true);
}
void MissionItem::updateOrderState()
{
    f_order->setValue(num() + 1);
    f_order->setEnabled(group->size() > 1);
}
void MissionItem::updateSelected()
{
    setSelected(group->mission->selectedItem() == this);
}
//=============================================================================
void MissionItem::selectTriggered()
{
    group->f_activeIndex->setValue(num());
}
//=============================================================================
void MissionItem::onCoordinateSystemChanged()
{
    Fact *f;

    f = AppSettings::instance()->findChild("interface.coordinate_system");
    if (f) {
        int v = f->value().toInt();
        switch (v) {
        case 1: // SK-42
            f_sk42x->setVisible(true);
            f_sk42y->setVisible(true);
            f_latitude->setVisible(false);
            f_longitude->setVisible(false);
            break;
        default:
            f_sk42x->setVisible(false);
            f_sk42y->setVisible(false);
            f_latitude->setVisible(true);
            f_longitude->setVisible(true);
            break;
        }
    }
}
//=============================================================================
void MissionItem::resetPath()
{
    m_geoPath = QGeoPath();
}
//=============================================================================
QGeoPath MissionItem::getPath()
{
    return QGeoPath();
}
//=============================================================================
MissionItem *MissionItem::prevItem() const
{
    return static_cast<MissionItem *>(parentFact()->child(indexInParent() - 1));
}
MissionItem *MissionItem::nextItem() const
{
    return static_cast<MissionItem *>(parentFact()->child(indexInParent() + 1));
}
//=============================================================================
QGeoRectangle MissionItem::boundingGeoRectangle() const
{
    QGeoRectangle r(QGeoCircle(coordinate(), 20).boundingGeoRectangle());
    r.setWidth(r.width() * 1.2);
    r.setHeight(r.height() * 1.2);
    return r;
}
//=============================================================================
//=============================================================================
//=============================================================================
QGeoCoordinate MissionItem::coordinate() const
{
    return m_coordinate;
}
void MissionItem::setCoordinate(const QGeoCoordinate &v)
{
    if (m_coordinate == v)
        return;
    m_coordinate = v;
    emit coordinateChanged();
    blockUpdateCoordinate = true;
    f_latitude->setValue(v.latitude());
    f_longitude->setValue(v.longitude());
    MPoint geoCoord(v.longitude(), v.latitude());
    auto sk42coord = AppGcs::instance()->mapInterface()->geoWgs84ToGaussSk42(geoCoord);
    f_sk42x->setValue(std::round(sk42coord.y));
    f_sk42y->setValue(std::round(sk42coord.x));
    blockUpdateCoordinate = false;
}
QGeoPath MissionItem::geoPath() const
{
    return m_geoPath;
}
void MissionItem::setGeoPath(const QGeoPath &v)
{
    if (m_geoPath == v)
        return;
    m_geoPath = v;
    emit geoPathChanged();
}
double MissionItem::course() const
{
    return m_course;
}
void MissionItem::setCourse(const double &v)
{
    if (m_course == v)
        return;
    m_course = v;
    emit courseChanged();
}
uint MissionItem::time() const
{
    return m_time;
}
void MissionItem::setTime(uint v)
{
    if (m_time == v)
        return;
    m_time = v;
    emit timeChanged();
}
uint MissionItem::distance() const
{
    return m_distance;
}
void MissionItem::setDistance(uint v)
{
    if (m_distance == v)
        return;
    m_distance = v;
    emit distanceChanged();
}
uint MissionItem::totalDistance() const
{
    return m_totalDistance;
}
void MissionItem::setTotalDistance(uint v)
{
    if (m_totalDistance == v)
        return;
    m_totalDistance = v;
    emit totalDistanceChanged();
}
uint MissionItem::totalTime() const
{
    return m_totalTime;
}
void MissionItem::setTotalTime(uint v)
{
    if (m_totalTime == v)
        return;
    m_totalTime = v;
    emit totalTimeChanged();
}
bool MissionItem::selected() const
{
    return m_selected;
}
void MissionItem::setSelected(bool v)
{
    if (m_selected == v)
        return;
    m_selected = v;
    emit selectedChanged();
    if (v)
        group->mission->setSelectedItem(this);
    else if (group->mission->selectedItem() == this)
        group->mission->setSelectedItem(nullptr);
}
//=============================================================================
//=============================================================================
