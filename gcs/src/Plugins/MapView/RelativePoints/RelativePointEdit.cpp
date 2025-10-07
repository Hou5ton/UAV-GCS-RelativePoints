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
#include "RelativePointEdit.h"
#include <App/App.h>

#include <App/AppGcs.h>
#include <KartaInterface.h>
//=============================================================================
RelativePointEdit::RelativePointEdit(Fact *parent,
                   const QString &name,
                   const QString &title,
                   const QString &descr,
                   QVariantMap modelData)
    : Fact(parent, name, title, descr, Group)
    , modelData(modelData)
    , blockUpdateItemData(false)
{
    f_title = new Fact(this, "rpname", tr("Title"), tr("Relative point name"), Text);
    f_descr = new Fact(this, "rpdescr", tr("Description"), tr("Relative point description"), Text);
    f_latitude = new Fact(this, "latitude", tr("Latitude"), tr("Relative point latitude"), Float);
    f_latitude->setUnits("lat");
    f_longitude = new Fact(this,
                           "longitude",
                           tr("Longitude"),
                           tr("Relative point longitude"),
                           Float);
    f_longitude->setUnits("lon");

    /*f_altitude = new Fact(this,
                           "altitude",
                           tr("Altitude"),
                           tr("Relative point altitude"),
                           Int);
    f_altitude->setUnits("m");

    f_altitude_matrix = new Fact(this,
                                 "altitude_matrix",
                                 tr("Altitude from matrix"),
                                 tr("Altitude from altitude matrix"),
                                 Int);
    f_altitude_matrix->setUnits("m");
    f_altitude_matrix->setEnabled(false);
    f_altitude_matrix->setVisible(false);*/


    f_sk42x = new Fact(this, "sk42y", tr("X"), tr("Relative point X (SK42)"), Int);
    f_sk42x->setUnits("m");
    f_sk42y = new Fact(this, "sk42x", tr("Y"), tr("Relative point Y (SK42)"), Int);
    f_sk42y->setUnits("m");

    if (!modelData.isEmpty()) {
        //f_relativePoints = new LookupRelativePoints(nullptr, this, Action);
        //connect(f_relativePoints, &Fact::triggered, this, &SiteEdit::lookupMissions);

        a_edit = new Fact(this,
                            "edit",
                            tr("Apply"),
                            tr("Apply"),
                            Action | Apply | CloseOnTrigger | ShowDisabled | IconOnly);
        connect(a_edit, &Fact::triggered, this, [this]() {
            emit relativePointEdited(this->modelData);

        });

        a_remove = new Fact(this,
                            "remove",
                            tr("Remove"),
                            "",
                            Action | Remove | CloseOnTrigger | ShowDisabled | IconOnly);
        connect(a_remove, &Fact::triggered, this, [this]() {
            emit removeTriggered(this->modelData);
        });

    } else {
        a_add = new Fact(this, "add", tr("Add"), "", Action | Apply | CloseOnTrigger | IconOnly | ShowDisabled);
        a_add->setEnabled(false);
        connect(a_add, &Fact::triggered, this, [this]() { emit addTriggered(this->modelData); });
        connect(f_title, &Fact::textChanged, this, [=]() {
            a_add->setEnabled(!f_title->text().isEmpty());
        });
        connect(this, &Fact::triggered, this, &RelativePointEdit::reset);
        reset();
    }



   /* if (!modelData.isEmpty()) {
        f_missions = new LookupRelativePoints(nullptr, this, Action);
        connect(f_missions, &Fact::triggered, this, &RelativePointEdit::lookupMissions);

        a_remove = new Fact(this,
                            "remove",
                            tr("Remove"),
                            "",
                            Action | Remove | CloseOnTrigger | ShowDisabled);
        connect(a_remove, &Fact::triggered, this, [this]() {
            emit removeTriggered(this->modelData);
        });
    } else {

    }*/

    loadFromModelData();

    connect(f_descr, &Fact::valueChanged, this, &RelativePointEdit::saveToModelData);
    connect(f_title, &Fact::valueChanged, this, &RelativePointEdit::saveToModelData);
    connect(f_latitude, &Fact::valueChanged, this, &RelativePointEdit::saveToModelData);
    connect(f_longitude, &Fact::valueChanged, this, &RelativePointEdit::saveToModelData);
    //connect(f_altitude, &Fact::valueChanged, this, &RelativePointEdit::saveToModelData);
    connect(f_sk42x, &Fact::valueChanged, this, &RelativePointEdit::saveToModelData);
    connect(f_sk42y, &Fact::valueChanged, this, &RelativePointEdit::saveToModelData);

    //coordinates system
    f_coordinateSystem = AppSettings::instance()->findChild("interface.coordinate_system");
    if(f_coordinateSystem){
        onCoordinateSystemChanged(); // set init state
        connect(f_coordinateSystem, &Fact::valueChanged, this, &RelativePointEdit::onCoordinateSystemChanged);
    }
}
//=============================================================================
void RelativePointEdit::reset()
{
    modelData.remove("title");
    modelData.remove("descr");
    QGeoCoordinate c(App::propertyValue<QGeoCoordinate>("apx.tools.missionplanner.clickCoordinate"));
    modelData["lat"] = c.latitude();
    modelData["lon"] = c.longitude();
    MPoint geoCoord(c.longitude(), c.latitude());
    auto sk42coord = AppGcs::instance()->mapInterface()->geoWgs84ToGaussSk42(geoCoord);
    modelData["sk42x"] = std::round(sk42coord.y);
    modelData["sk42y"] = std::round(sk42coord.x);

    const auto altitudeMatrix = AltitudeMatrix::instance();
    if (altitudeMatrix->isLoaded() &&
        altitudeMatrix->getAltitudeAt(c)){
        modelData["alt"] = altitudeMatrix->getAltitudeAt(c);
    }else{
        modelData["alt"] = 0;
    }
    loadFromModelData();
}
//=============================================================================
void RelativePointEdit::loadFromModelData()
{
    blockUpdateItemData = true;
    f_title->setValue(modelData.value("title").toString());
    f_descr->setValue(modelData.value("descr").toString());
    f_latitude->setValue(modelData.value("lat").toDouble());
    f_longitude->setValue(modelData.value("lon").toDouble());
    //f_altitude->setValue(modelData.value("alt").toInt());

    QGeoCoordinate coordinate;
    coordinate.setLatitude(modelData.value("lat").toDouble());
    coordinate.setLongitude(modelData.value("lon").toDouble());
    /*const auto altitudeMatrix = AltitudeMatrix::instance();
    if (altitudeMatrix->isLoaded() &&
        altitudeMatrix->getAltitudeAt(coordinate)){
        f_altitude_matrix->setValue(altitudeMatrix->getAltitudeAt(coordinate));
    }else{
        f_altitude_matrix->setValue(f_altitude->value());
    }
    if(f_altitude->value().toInt() == f_altitude_matrix->value().toInt()){
        f_altitude_matrix->setVisible(false);
    }else{
        f_altitude_matrix->setVisible(true);
    }*/


    // calculate sk42 coordinates, don't stored in DB
    if (!modelData.contains("sk42x") || !modelData.contains("sk42y")){
        MPoint geoCoord(modelData.value("lon").toDouble(), modelData.value("lat").toDouble());
        auto sk42coord = AppGcs::instance()->mapInterface()->geoWgs84ToGaussSk42(geoCoord);
        modelData["sk42x"] = std::round(sk42coord.y);
        modelData["sk42y"] = std::round(sk42coord.x);
    }
    f_sk42x->setValue(modelData.value("sk42x").toInt());
    f_sk42y->setValue(modelData.value("sk42y").toInt());

    blockUpdateItemData = false;
}
void RelativePointEdit::saveToModelData()
{
    if (blockUpdateItemData)
        return;
    if (f_title->text().isEmpty()) {
        loadFromModelData();
        return;
    }
    double lat{0.0};
    double lon{0.0};
    int sk42x{0};
    int sk42y{0};

    if(f_coordinateSystem && f_coordinateSystem->value().toInt() == 1){
        MPoint sk42coord(f_sk42y->value().toDouble(), f_sk42x->value().toDouble());
        auto geoCoord = AppGcs::instance()->mapInterface()->gaussSk42ToGeoWgs84(sk42coord);
        lat = geoCoord.y;
        lon = geoCoord.x;
        sk42x = f_sk42x->value().toInt();
        sk42y = f_sk42y->value().toInt();
    }
    else{
        lat = f_latitude->value().toDouble();
        lon = f_longitude->value().toDouble();
        MPoint geoCoord(lon, lat);
        auto sk42coord = AppGcs::instance()->mapInterface()->geoWgs84ToGaussSk42(geoCoord);
        sk42x = std::round(sk42coord.y);
        sk42y = std::round(sk42coord.x);
    }

    if (std::isnan(lat) || std::isnan(lon) || lat == 0.0 || lon == 0.0) {
        loadFromModelData();
        return;
    }
    modelData["title"] = f_title->text();
    modelData["descr"] = f_descr->text();
    modelData["lat"] = lat;
    modelData["lon"] = lon;
    modelData["sk42x"] = sk42x;
    modelData["sk42y"] = sk42y;
    modelData["disp_on_map"] = 1;
    //modelData["alt"] = f_altitude->value().toInt();
    emit relativePointEdited(modelData);
}
void RelativePointEdit::setModelData(QVariantMap v)
{
    if (modelData.value("key").toULongLong() != v.value("key").toULongLong())
        return;
    modelData = v;
    loadFromModelData();
}
void RelativePointEdit::updateFromEditedModelData(int i, QVariantMap v)
{
    Q_UNUSED(i)
    setModelData(v);
    emit relativePointEdited(modelData);
}
//=============================================================================

void RelativePointEdit::onCoordinateSystemChanged()
{
    if (f_coordinateSystem) {
        int v = f_coordinateSystem->value().toInt();
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
//=============================================================================
