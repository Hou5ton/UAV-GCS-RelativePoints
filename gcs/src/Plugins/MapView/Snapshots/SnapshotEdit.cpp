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
#include "SnapshotEdit.h"
#include <KartaInterface.h>
#include <App/AppGcs.h>
#include <App/App.h>
#include <QMessageBox>

const QString sWarningCaption = QObject::tr("Warning");
//=============================================================================
SnapshotEdit::SnapshotEdit(Fact *parent,
                   const QString &name,
                   const QString &title,
                   const QString &descr,
                   QVariantMap modelData)
    : Fact(parent, name, title, descr, Group)
    , modelData(modelData)
    , modelDataStart(modelData)
    , blockUpdateItemData(false)
{   

    if (modelData.isEmpty()) {
         qDebug()<< "modelData.size():" << modelData.size();
    }
    else{

        f_title = new Fact(this, "sname", tr("Title"), tr("Screenshot name"), Text);
        //f_title->setEnabled(false);

        f_descr = new Fact(this, "sdescr", tr("Description"), tr("Screenshot description"), Text);

        f_vehicle = new Fact(this, "svehicle", tr("Vehicle"), tr("Screenshot vehicle"), Text);
       // f_vehicle->setEnabled(false);

        f_filename = new Fact(this, "sfilename", tr("File"), tr("File Name"), Text);
       // f_filename->setEnabled(false);

        f_use_label_coo = new Fact(this,
                             "use_label_coo",
                             tr("Display label coordinates"),
                             "",
                             Bool | PersistentValue,
                             /*"wifi-off"*/"");
        f_use_label_coo->setDefaultValue(false);
        f_use_label_coo->setValue(false);
        connect(f_use_label_coo,
                 &Fact::valueChanged,
                this,
                &SnapshotEdit::onCoordinateSystemChanged);

        connect(this,
                &SnapshotEdit::sig_use_label_coo_changed,
                this,
                &SnapshotEdit::onCoordinateSystemChanged);

        f_latitude = new Fact(this,
                              "latitude",
                              tr("Latitude"),
                              tr("Label latitude"), Float);
        f_latitude->setUnits("lat");
        f_latitude->setEnabled(false);
        f_latitude->setVisible(false);

        f_longitude = new Fact(this,
                               "longitude",
                               tr("Longitude"),
                               tr("Label longitude"),
                               Float);
        f_longitude->setUnits("lon");
        f_longitude->setEnabled(false);
        f_longitude->setVisible(false);

        f_latitude_scr = new Fact(this, "latitude_scr", tr("Latitude"), tr("Screenshot latitude"), Float);
        f_latitude_scr->setUnits("lat");
        f_latitude_scr->setEnabled(false);

        f_longitude_scr = new Fact(this,
                               "longitude_scr",
                               tr("Longitude"),
                               tr("Screenshot longitude"),
                               Float);
        f_longitude_scr->setUnits("lon");
        f_longitude_scr->setEnabled(false);

        f_sk42x = new Fact(this, "sk42y", tr("X"), tr("Label X (SK42)"), Int);
        f_sk42x->setUnits("m");
        f_sk42x->setEnabled(false);

        f_sk42y = new Fact(this, "sk42x", tr("Y"), tr("Label Y (SK42)"), Int);
        f_sk42y->setUnits("m");
        f_sk42y->setEnabled(false);

        f_sk42x_scr = new Fact(this, "sk42y_scr", tr("X"), tr("Screenshot X (SK42)"), Int);
        f_sk42x_scr->setUnits("m");
        f_sk42x_scr->setEnabled(false);

        f_sk42y_scr = new Fact(this, "sk42x_scr", tr("Y"), tr("Screenshot Y (SK42)"), Int);
        f_sk42y_scr->setUnits("m");
        f_sk42y_scr->setEnabled(false);

        f_alt= new Fact(this,
                               "altitude",
                               tr("Altitude"),
                               tr("Screenshot altitude"),
                               Float);
        f_alt->setUnits("m");
        f_alt->setEnabled(false);

        f_alt_label= new Fact(this,
                               "altitude_label",
                               tr("Altitude"),
                               tr("Label altitude"),
                               Float);
        f_alt_label->setUnits("m");
        f_alt_label->setEnabled(false);
        f_alt_label->setVisible(false);


        /*f_missions = new LookupMissions(nullptr, this, Action);
        connect(f_missions, &Fact::triggered, this, &SnapshotEdit::lookupMissions);*/


        a_edit = new Fact(this,
                            "edit",
                            tr("Apply"),
                            tr("Apply"),
                            Action | Apply | CloseOnTrigger | ShowDisabled | IconOnly);
        connect(a_edit, &Fact::triggered, this, [this]() {
            emit snapshotEdited(this->modelData);

        });

        a_move_to_scr_pos = new Fact(this,
                                     "move",
                                     tr("Return"),
                                     tr("Return to screenshot position"),
                                     Action | Apply | CloseOnTrigger | ShowDisabled | IconOnly,
                                     "image-move");
                 connect(a_move_to_scr_pos, &Fact::triggered, this, [this]() {

                    // qDebug()<<this->modelData;
                     QVariantMap tmp = modelDataStart;

                     tmp["lat"] = tmp["lat_scr"];
                     tmp["lon"] = tmp["lon_scr"];

                     this->modelData = tmp;
                     emit snapshotMove(this->modelData);

                     this->remove();

                 });

        double lat =  modelData["lat"].toDouble();
        double lon =  modelData["lon"].toDouble();
        double lat_scr = modelData["lat_scr"].toDouble();
        double lon_scr = modelData["lon_scr"].toDouble();

        if(lat != lat_scr ||
               lon !=  lon_scr){
            a_move_to_scr_pos->setEnabled(true);
        }else{
            a_move_to_scr_pos->setEnabled(false);
        }

        a_show = new Fact(this,
                            "show",
                            tr("Open"),
                            tr("Open current screenshot"),
                           Action | ShowDisabled | Apply | IconOnly,
                          "movie-open-outline");
        connect(a_show, &Fact::triggered, this, [this]() {
            emit snapshotShow(this->modelData);
        });

        a_remove = new Fact(this,
                            "remove",
                            tr("Remove"),
                            tr("Remove current screenshot"),
                            Action | ShowDisabled | Remove | IconOnly,
                            "delete");

        connect(a_remove, &Fact::triggered, this, [this]() {
            QString qsWarning =  tr("You are going to detete the screenshot");
            qsWarning += " ";
            qsWarning += this->modelData["title"].toString();
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
            emit removeTriggered(this->modelData);
        });

        loadFromModelData();

        connect(f_vehicle, &Fact::valueChanged, this, &SnapshotEdit::rollBackToModelData);
        connect(f_filename, &Fact::valueChanged, this, &SnapshotEdit::rollBackToModelData);

        connect(f_title, &Fact::valueChanged, this, &SnapshotEdit::saveToModelData);
        connect(f_latitude, &Fact::valueChanged, this, &SnapshotEdit::saveToModelData);
        connect(f_longitude, &Fact::valueChanged, this, &SnapshotEdit::saveToModelData);
        connect(f_latitude_scr, &Fact::valueChanged, this, &SnapshotEdit::saveToModelData);
        connect(f_longitude_scr, &Fact::valueChanged, this, &SnapshotEdit::saveToModelData);
        connect(f_alt, &Fact::valueChanged, this, &SnapshotEdit::saveToModelData);
        connect(f_descr, &Fact::valueChanged, this, &SnapshotEdit::saveToModelData);
        connect(f_sk42x, &Fact::valueChanged, this, &SnapshotEdit::saveToModelData);
        connect(f_sk42y, &Fact::valueChanged, this, &SnapshotEdit::saveToModelData);
        connect(f_sk42x_scr, &Fact::valueChanged, this, &SnapshotEdit::saveToModelData);
        connect(f_sk42y_scr, &Fact::valueChanged, this, &SnapshotEdit::saveToModelData);

        //coordinates system
        f_coordinateSystem = AppSettings::instance()->findChild("interface.coordinate_system");
        if(f_coordinateSystem){
            onCoordinateSystemChanged(); // set init state
            connect(f_coordinateSystem, &Fact::valueChanged, this, &SnapshotEdit::onCoordinateSystemChanged);
        }
    }
}
//=============================================================================
void SnapshotEdit::reset()
{
    modelData.remove("title");
    //modelData.remove("descr");
    QGeoCoordinate c(
        App::propertyValue<QGeoCoordinate>("apx.tools.missionplanner.clickCoordinate"));
    modelData["lat"] = c.latitude();
    modelData["lon"] = c.longitude();

   /* MPoint geoCoord(c.longitude(), c.latitude());
    auto sk42coord = AppGcs::instance()->mapInterface()->geoWgs84ToGaussSk42(geoCoord);
    modelData["sk42x"] = std::round(sk42coord.y);
    modelData["sk42y"] = std::round(sk42coord.x);*/

    loadFromModelData();
}
//=============================================================================
void SnapshotEdit::loadFromModelData()
{
    blockUpdateItemData = true;

    if(modelData.size()){
        f_title->setValue(modelData.value("title").toString());
        f_latitude->setValue(modelData.value("lat").toDouble());
        f_longitude->setValue(modelData.value("lon").toDouble());
        f_latitude_scr->setValue(modelData.value("lat_scr").toDouble());
        f_longitude_scr->setValue(modelData.value("lon_scr").toDouble());
        f_alt->setValue(modelData.value("alt").toDouble());
        f_descr->setValue(modelData.value("descr").toString());
        f_vehicle->setValue(modelData.value("vehicle").toString());
        f_filename->setValue(modelData.value("filename").toString());

        // calculate sk42 coordinates, don't stored in DB
        if (!modelData.contains("sk42x_scr") || !modelData.contains("sk42y_scr")){
            MPoint geoCoord(modelData.value("lon_scr").toDouble(), modelData.value("lat_scr").toDouble());
            auto sk42coord = AppGcs::instance()->mapInterface()->geoWgs84ToGaussSk42(geoCoord);
            modelData["sk42x_scr"] = std::round(sk42coord.y);
            modelData["sk42y_scr"] = std::round(sk42coord.x);
        }
        f_sk42x_scr->setValue(modelData.value("sk42x_scr").toInt());
        f_sk42y_scr->setValue(modelData.value("sk42y_scr").toInt());


        if (!modelData.contains("sk42x") || !modelData.contains("sk42y")){
            MPoint geoCoord(modelData.value("lon").toDouble(), modelData.value("lat").toDouble());
            auto sk42coord = AppGcs::instance()->mapInterface()->geoWgs84ToGaussSk42(geoCoord);
            modelData["sk42x"] = std::round(sk42coord.y);
            modelData["sk42y"] = std::round(sk42coord.x);
        }
        f_sk42x->setValue(modelData.value("sk42x").toInt());
        f_sk42y->setValue(modelData.value("sk42y").toInt());

        QGeoCoordinate coordinate;
        coordinate.setLatitude(modelData.value("lat").toDouble());
        coordinate.setLongitude(modelData.value("lon").toDouble());
        const auto altitudeMatrix = AltitudeMatrix::instance();
        if (altitudeMatrix->isLoaded() &&
            altitudeMatrix->getAltitudeAt(coordinate)){
            f_alt_label->setValue(altitudeMatrix->getAltitudeAt(coordinate));
        }else{
            f_alt_label->setValue(f_alt->value());
        }
       // emit sig_use_label_coo_changed();
    }
    blockUpdateItemData = false;
}

void SnapshotEdit::rollBackToModelData()
{
    f_vehicle->setValue(modelData["vehicle"]);
    f_filename->setValue(modelData["filename"]);
}
void SnapshotEdit::saveToModelData()
{
    if (blockUpdateItemData)
        return;

    QString title = f_title->text();
    double lat = f_latitude->value().toDouble();
    double lon = f_longitude->value().toDouble();
    int alt = f_alt->value().toInt();
    QString descr = f_descr->text();
    QString vehicle = f_vehicle->text();
    QString filename = f_filename->text();
    double lat_scr = f_latitude_scr->value().toDouble();
    double lon_scr = f_longitude_scr->value().toDouble();
    int sk42x = f_sk42x->value().toInt();
    int sk42y = f_sk42y->value().toInt();
    int sk42x_scr = f_sk42x_scr->value().toInt();
    int sk42y_scr = f_sk42y_scr->value().toInt();

    if (f_title->text().isEmpty()) {
        loadFromModelData();
        return;
    }
    if (std::isnan(lat) || std::isnan(lon) || lat == 0.0 || lon == 0.0) {
        loadFromModelData();
        return;
    }
    modelData["title"] = title;
    modelData["descr"] = descr;
    modelData["vehicle"] = vehicle;
    modelData["filename"] = filename;
    modelData["lat"] = lat;
    modelData["lon"] = lon;
    modelData["alt"] = alt;
    modelData["lat_scr"] = lat_scr;
    modelData["lon_scr"] = lon_scr;
    modelData["sk42x"] = sk42x;
    modelData["sk42y"] = sk42y;
    modelData["sk42x_scr"] = sk42x_scr;
    modelData["sk42y_scr"] = sk42y_scr;

    QDateTime dateTime = QDateTime::fromString(title,"dd_MM_yyyyThh_mm_ss_zzz");
    if(dateTime.isValid())
    {
        QString qs = dateTime.time().toString("hh:mm:ss.zzz");
        modelData["time"] = qs;
        qs = dateTime.date().toString("yyyy MMM d");
        modelData["date"] = qs;
    }

   // emit snapshotEdited(modelData);
}
void SnapshotEdit::setModelData(QVariantMap v)
{
    //qDebug() << "setModelData:" << v.value("key").toULongLong();
    if (modelData.value("key").toULongLong() != v.value("key").toULongLong())
        return;

    if(modelData.size() != v.size()){
        QVariantMap vm = v;

        MPoint geoCoord_scr(vm.value("lon_scr").toDouble(), vm.value("lat_scr").toDouble());
        auto sk42coord_scr = AppGcs::instance()->mapInterface()->geoWgs84ToGaussSk42(geoCoord_scr);
        vm["sk42x_scr"] = std::round(sk42coord_scr.y);
        vm["sk42y_scr"] = std::round(sk42coord_scr.x);

        MPoint geoCoord(vm.value("lon").toDouble(), vm.value("lat").toDouble());
        auto sk42coord = AppGcs::instance()->mapInterface()->geoWgs84ToGaussSk42(geoCoord);
        vm["sk42x"] = std::round(sk42coord.y);
        vm["sk42y"] = std::round(sk42coord.x);
        modelData = vm;
    }else{
        modelData = v;
    }
    loadFromModelData();
}
void SnapshotEdit::updateFromEditedModelData(int i, QVariantMap v)
{
    Q_UNUSED(i)
    setModelData(v);
    emit snapshotEdited(modelData);
}
//=============================================================================
void SnapshotEdit::lookupMissions()
{
    QGeoCoordinate c(modelData.value("lat").toDouble(), modelData.value("lon").toDouble());
    //f_missions->dbLookupMissionsByArea(c, modelData.value("title").toString());
}
void SnapshotEdit::onCoordinateSystemChanged()
{
    if (f_coordinateSystem) {
        int v = f_coordinateSystem->value().toInt();
        bool b = f_use_label_coo->value().toBool();

        switch (v) {
        case 1: // SK-42
            if(b){
                f_sk42x->setVisible(true);
                f_sk42y->setVisible(true);
                f_alt_label->setVisible(true);

                f_latitude_scr->setVisible(false);
                f_longitude_scr->setVisible(false);
                f_latitude->setVisible(false);
                f_longitude->setVisible(false);
                f_alt->setVisible(false);
                f_sk42x_scr->setVisible(false);
                f_sk42y_scr->setVisible(false);

            }else{
                f_sk42x_scr->setVisible(true);
                f_sk42y_scr->setVisible(true);
                f_alt->setVisible(true);

                f_alt_label->setVisible(false);
                f_latitude_scr->setVisible(false);
                f_longitude_scr->setVisible(false);
                f_latitude->setVisible(false);
                f_longitude->setVisible(false);
                f_sk42x->setVisible(false);
                f_sk42y->setVisible(false);
            }
            break;
        default:
            if(b){

                f_alt_label->setVisible(true);
                f_latitude->setVisible(true);
                f_longitude->setVisible(true);

                f_latitude_scr->setVisible(false);
                f_longitude_scr->setVisible(false);
                f_alt->setVisible(false);
                f_sk42x_scr->setVisible(false);
                f_sk42y_scr->setVisible(false);
                f_sk42x->setVisible(false);
                f_sk42y->setVisible(false);

            }else{

                f_alt->setVisible(true);
                f_latitude_scr->setVisible(true);
                f_longitude_scr->setVisible(true);

                f_alt_label->setVisible(false);
                f_latitude->setVisible(false);
                f_longitude->setVisible(false);

                f_sk42x_scr->setVisible(false);
                f_sk42y_scr->setVisible(false);
                f_sk42x->setVisible(false);
                f_sk42y->setVisible(false);
            }
            break;
        }
    }
}

//=============================================================================
//=============================================================================
