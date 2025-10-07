#include "poicontrol.h"

#include <App/App.h>
#include <App/AppDirs.h>
#include <App/AppLog.h>
#include <App/AppRoot.h>
#include <QCameraInfo>
#include <QVideoSurfaceFormat>
#include <QtQml>
#include <AltitudeMatrix/AltitudeMatrix.h>
#include <App/AppGcs.h>
#include <KartaInterface.h>

#include <Vehicles/Vehicle.h>
#include <Vehicles/VehicleMandala.h>
#include <Vehicles/Vehicles.h>

#include <iostream>

using namespace std::placeholders;

POIControl::POIControl(Fact *parent)
    : Fact(parent,
           QString(PLUGIN_NAME).toLower(),
           tr("Target"),
           tr("Target position tracking"),
           Group)
{

    setIcon("camera-metering-center");
    setVisible(true);

    f_cooType = new Fact(this,
                         "coo_type",
                         tr("Coordinate Type"),
                         "",
                         Enum | PersistentValue);
    f_cooType->setEnumStrings({"WGS D° M' S\"","WGS D.D°"});


    f_latitude = new Fact(this,
                          "latitude",
                          tr("Latitude"),
                          tr("Target latitude"),
                          Text | PersistentValue);

    f_latitude->setValue("0.0");

    f_longitude = new Fact(this,
                           "longitude",
                           tr("Longitude"),
                           tr("Target longitude"),
                           Text | PersistentValue);

    f_longitude->setValue("0.0");

    f_sk42x = new Fact(this, "sk42y", tr("X"), tr("Global postition X (SK42)"), Int);
    f_sk42x->setUnits("m");
    //f_sk42x->setEnabled(false);

    f_sk42y = new Fact(this, "sk42x", tr("Y"), tr("Global postition Y (SK42)"), Int);
    f_sk42y->setUnits("m");
    //f_sk42y->setEnabled(false);

    f_altitude = new Fact(this,
                          "altitude",
                          tr("Altitude"),
                          tr("Target altitude"),
                          Int | PersistentValue);
    f_altitude->setUnits("m");

    f_matrix_altitude = new Fact(this,
                                 "matrix_altitude",
                                 tr("Altitude Matrix"),
                                 tr("Target altitude from matrix"),
                                 Int | PersistentValue);

    //f_matrix_altitude->setDefaultValue("100");
    f_matrix_altitude->setUnits("m");
    f_matrix_altitude->setEnabled(false);


    f_setAltFromMatrix = new Fact(this,
                                  "set_from_matrix",
                                  tr("Set altitude"),
                                  tr("Set altitude from matrix"),
                                  Action /*| CloseOnTrigger*/);



    f_setPOI = new Fact(this,
                        "select_save",
                        tr("Apply"),
                        "",
                        Action | Apply | CloseOnTrigger);

    f_latitudeBinding = new Fact(this,
                                   "lat_binding",
                                   tr("Target latitude binding"),
                                   tr("Target latitude binding to mandala variable"),
                                   Mandala | PersistentValue,
                                   "link-plus");
    f_latitudeBinding->setDefaultValue("cam_lat");

    f_longitudeBinding = new Fact(this,
                                   "lon_binding",
                                   tr("Target longitude binding"),
                                   tr("Target longitude binding to mandala variable"),
                                   Mandala | PersistentValue,
                                   "link-plus");
    f_longitudeBinding->setDefaultValue("cam_lon");

    f_altitudeBinding = new Fact(this,
                                   "alt_binding",
                                   tr("Target altitude binding"),
                                   tr("Target altitude binding to mandala variable"),
                                   Mandala | PersistentValue,
                                   "link-plus");
    f_altitudeBinding->setDefaultValue("cam_hmsl");

    connect(Vehicles::instance(), &Vehicles::vehicleSelected, this, &POIControl::onVehicleSelected);
    m_vehicle = Vehicles::instance()->current();
    onVehicleSelected(m_vehicle);

    connect(f_latitude, &Fact::valueChanged, this, &POIControl::onLatLonChanged);
    connect(f_longitude, &Fact::valueChanged, this, &POIControl::onLatLonChanged);
    connect(f_altitude, &Fact::valueChanged, this, &POIControl::onAltitudeChanged);
    connect(f_matrix_altitude, &Fact::valueChanged, this, &POIControl::onMatrixAltitudeChanged);
    connect(f_cooType, &Fact::valueChanged, this, &POIControl::onCooTypeChanged);
    connect(f_setPOI, &Fact::triggered, this, &POIControl::onSetPOI);
    connect(f_setAltFromMatrix, &Fact::triggered, this, &POIControl::onSetAltFromMatrix);
    connect(this, &POIControl::triggered, this, &POIControl::onTriggered);

    connect(f_latitudeBinding, &Fact::valueChanged, this, &POIControl::onLatitudeBindingChanged);
    connect(f_longitudeBinding, &Fact::valueChanged, this, &POIControl::onLongitudeBindingChanged);
    connect(f_altitudeBinding, &Fact::valueChanged, this, &POIControl::onAltitudeBindingChanged);

    connect(f_sk42x, &Fact::valueChanged, this, &POIControl::onLatLonSK42Changed);
    connect(f_sk42y, &Fact::valueChanged, this, &POIControl::onLatLonSK42Changed);

    //coordinates system
    f_coordinateSystem = AppSettings::instance()->findChild("interface.coordinate_system");
    if(f_coordinateSystem){
        onCoordinateSystemChanged(); // set init state
        connect(f_coordinateSystem, &Fact::valueChanged, this, &POIControl::onCoordinateSystemChanged);
    }

    if(m_vehicle){
        m_latitudeFact = m_vehicle->f_mandala->factByName(f_latitudeBinding->defaultValue().toString());
        m_longitudeFact = m_vehicle->f_mandala->factByName(f_longitudeBinding->defaultValue().toString());
        m_altitudeFact = m_vehicle->f_mandala->factByName(f_altitudeBinding->defaultValue().toString());
    }

    onCooTypeChanged();
}
void POIControl::onCoordinateSystemChanged()
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
void POIControl::fillPOI()
{
    QString qsLat =  "";
    QString qsLon =  "";
    int iAlt =  0;

    if(m_latitudeFact != nullptr &&
            m_longitudeFact != nullptr &&
            m_altitudeFact != nullptr){

        if (f_cooType->value().toInt() == wgsDMS){
            qsLat =  AppRoot::latToString((m_latitudeFact->value().toDouble()));
            qsLon =  AppRoot::lonToString((m_longitudeFact->value().toDouble()));
        } else if (f_cooType->value().toInt() == wgsDD){
            qsLat =  AppRoot::latToStringDouble((m_latitudeFact->value().toDouble()));
            qsLon = AppRoot::lonToStringDouble((m_longitudeFact->value().toDouble()));
        }
        iAlt = m_altitudeFact->value().toInt();
    }

    if(qsLat.contains(" 0.")){
        onErrorOccured(tr("Can't fill target latitude"));
    }else if(qsLon.contains(" 0.")){
        onErrorOccured(tr("Can't fill target longitude"));
    }else{

        f_latitude->setValue(qsLat);
        f_longitude->setValue(qsLon);

        MPoint geoCoord(m_longitudeFact->value().toDouble(), m_latitudeFact->value().toDouble());
        auto sk42coord = AppGcs::instance()->mapInterface()->geoWgs84ToGaussSk42(geoCoord);
        f_sk42x->setValue( std::round(sk42coord.y));
        f_sk42y->setValue(std::round(sk42coord.x));
    }
    f_altitude->setValue(iAlt);

    onAltitudeChanged();
}

void POIControl::onLongitudeBindingChanged()
{
    if(m_vehicle)
        m_longitudeFact = m_vehicle->f_mandala->factByName(f_longitudeBinding->text());
}
void POIControl::onLatitudeBindingChanged()
{
    if(m_vehicle)
        m_latitudeFact = m_vehicle->f_mandala->factByName(f_latitudeBinding->text());
}
void POIControl::onAltitudeBindingChanged()
{
    if(m_vehicle){
        m_altitudeFact = m_vehicle->f_mandala->factByName(f_altitudeBinding->text());
    }
}
void POIControl::onCooTypeChanged()
{
   fillPOI();
}
void POIControl::onAltitudeChanged()
{
    if(f_matrix_altitude->value() == f_altitude->value())
    {
        f_matrix_altitude->setVisible(false);
        f_setAltFromMatrix->setVisible(false);
    }
    else
    {
        f_matrix_altitude->setVisible(true);
        f_setAltFromMatrix->setVisible(true);
    }
}

void POIControl::onMatrixAltitudeChanged()
{
    if(f_matrix_altitude->value().toInt() != 0)
        onSetAltFromMatrix();
    if(f_matrix_altitude->value() == f_altitude->value())
    {
        f_matrix_altitude->setVisible(false);
        f_setAltFromMatrix->setVisible(false);
    }
    else
    {
        f_matrix_altitude->setVisible(true);
        f_setAltFromMatrix->setVisible(true);
    }
}
void POIControl::onLatLonChanged()
{
    if (f_coordinateSystem) {
        int v = f_coordinateSystem->value().toInt();
        if (v != 1) {
            double latitude = 0;
            double longitude = 0;

            if (f_cooType->value().toInt() == wgsDMS){
                latitude =  AppRoot::latFromString(f_latitude->value().toString());
                longitude =  AppRoot::lonFromString(f_longitude->value().toString());
            } else if (f_cooType->value().toInt() == wgsDD){
                latitude = AppRoot::latFromStringDouble(f_latitude->value().toString());
                longitude = AppRoot::lonFromStringDouble(f_longitude->value().toString());
            }

            QGeoCoordinate point(latitude, longitude);

            MPoint geoCoord(longitude, latitude);
            auto sk42coord = AppGcs::instance()->mapInterface()->geoWgs84ToGaussSk42(geoCoord);
            f_sk42x->setValue( std::round(sk42coord.y));
            f_sk42y->setValue(std::round(sk42coord.x));

            double altitude = AltitudeMatrix::instance()->getAltitudeAt(point);
            f_matrix_altitude->setValue(altitude);
        }else{
            double latitude = 0;
            double longitude = 0;

            int x = f_sk42x->value().toInt();
            int y = f_sk42y->value().toInt();

            MPoint sk42coord(y, x);
            auto geocoord = AppGcs::instance()->mapInterface()->gaussSk42ToGeoWgs84(sk42coord);

            longitude = geocoord.x;
            latitude =  geocoord.y;

            QString qsLat =  "";
            QString qsLon =  "";


            if (f_cooType->value().toInt() == wgsDMS){
                qsLat =  AppRoot::latToString((latitude));
                qsLon =  AppRoot::lonToString((longitude));
            } else if (f_cooType->value().toInt() == wgsDD){
                qsLat =  AppRoot::latToStringDouble((latitude));
                qsLon = AppRoot::lonToStringDouble((longitude));
            }
            if(qsLat.contains(" 0.")){
                onErrorOccured(tr("Can't fill target latitude"));
            }else if(qsLon.contains(" 0.")){
                onErrorOccured(tr("Can't fill target longitude"));
            }else{
                f_latitude->setValue(qsLat);
                f_longitude->setValue(qsLon);
            }
        }
    }

}

void POIControl::onLatLonSK42Changed()
{
    if (f_coordinateSystem) {
        int v = f_coordinateSystem->value().toInt();
        if (v == 1) {
            double latitude = 0;
            double longitude = 0;

            int x = f_sk42x->value().toInt();
            int y = f_sk42y->value().toInt();

            MPoint sk42coord(y, x);
            auto geocoord = AppGcs::instance()->mapInterface()->gaussSk42ToGeoWgs84(sk42coord);

            longitude = geocoord.x;
            latitude =  geocoord.y;

            QString qsLat =  "";
            QString qsLon =  "";

            if (f_cooType->value().toInt() == wgsDMS){
                qsLat =  AppRoot::latToString((latitude));
                qsLon =  AppRoot::lonToString((longitude));
            } else if (f_cooType->value().toInt() == wgsDD){
                qsLat =  AppRoot::latToStringDouble((latitude));
                qsLon = AppRoot::lonToStringDouble((longitude));
            }
            if(qsLat.contains(" 0.")){
                onErrorOccured(tr("Can't fill target latitude"));
            }else if(qsLon.contains(" 0.")){
                onErrorOccured(tr("Can't fill target longitude"));
            }else{
                f_latitude->setValue(qsLat);
                f_longitude->setValue(qsLon);
            }

            QGeoCoordinate point(latitude, longitude);

            double altitude = AltitudeMatrix::instance()->getAltitudeAt(point);
            f_matrix_altitude->setValue(altitude);
        }else{

            MPoint geoCoord(m_longitudeFact->value().toDouble(), m_latitudeFact->value().toDouble());
            auto sk42coord = AppGcs::instance()->mapInterface()->geoWgs84ToGaussSk42(geoCoord);
            f_sk42x->setValue( std::round(sk42coord.y));
            f_sk42y->setValue(std::round(sk42coord.x));
        }
    }
}

void POIControl::onVehicleSelected(Vehicle *vehicle)
{
    if(m_vehicle)
        m_vehicle = nullptr;
    m_vehicle = vehicle;

   /* VehicleMandala *m = vehicle->f_mandala;
    for (int i = 0; i < f_tools->size(); ++i) {
        Fact *f = f_tools->child(i);
        f->bind(m->child(f->name()));
    }*/
}

void POIControl::onSetPOI()
{
    double dLat = 0.0;
    double dLon = 0.0;

    if (f_coordinateSystem) {
        int v = f_coordinateSystem->value().toInt();
        if (v == 1) {
            int iX = f_sk42x->value().toInt();
            int iY = f_sk42y->value().toInt();
            if(iX != 0 && iY != 0){

                MPoint sk42coord(iY, iX);
                auto geocoord = AppGcs::instance()->mapInterface()->gaussSk42ToGeoWgs84(sk42coord);
                dLon = geocoord.x;
                dLat = geocoord.y;
            }

        }else{
            QString qsLat = f_latitude->value().toString();
            QString qsLon = f_longitude->value().toString();

            if(qsLat != "" && qsLon != ""){
                if(f_cooType->value() == POIControl::SourceType::wgsDMS){
                    dLat = AppRoot::latFromString(qsLat);
                    dLon = AppRoot::lonFromString(qsLon);
                } else if(f_cooType->value() == POIControl::SourceType::wgsDD){
                    dLat = AppRoot::latFromStringDouble(qsLat);
                    dLon = AppRoot::lonFromStringDouble(qsLon);
                }
            }
        }
    }

    if(dLat == 0.0){
        onErrorOccured(tr("Can't set target latitude"));
    }else if(dLon == 0.0){
        onErrorOccured(tr("Can't set target longitude"));
    }else if(!m_vehicle){
        onErrorOccured(tr("Wrong vehicle"));
    }else{
        QString qs = m_latitudeFact->name();
        qs+="=";
        qs+=QString::number(dLat);
        onInfoOccured(qs);

        qs = m_longitudeFact->name();
        qs+="=";
        qs+=QString::number(dLon);
        onInfoOccured(qs);
        qs = m_altitudeFact->name();
        qs+="=";
        qs+= QString::number(f_altitude->value().toInt());
        onInfoOccured(qs);

        QGeoCoordinate m_coordinates;
        m_coordinates.setLatitude(dLat);
        m_coordinates.setLongitude(dLon);
        if(m_vehicle->protocol && m_coordinates.isValid()){
            m_vehicle->lookHere(m_coordinates);
        }else{
            m_latitudeFact->setValue(dLat);
            m_longitudeFact->setValue(dLon);
        }
        m_altitudeFact->setValue(f_altitude->value());
    }

}
void POIControl::onSetAltFromMatrix()
{
    f_altitude->setValue(f_matrix_altitude->value());
}

void POIControl::createPOIFromVideo()
{
    if (f_coordinateSystem) {
        int v = f_coordinateSystem->value().toInt();
        if (v != 1) {
            f_cooType->setVisible(true);
            f_latitude->setVisible(true);
            f_longitude->setVisible(true);
            f_sk42x->setVisible(false);
            f_sk42y->setVisible(false);
        }else{
            f_cooType->setVisible(false);
            f_latitude->setVisible(false);
            f_longitude->setVisible(false);
            f_sk42x->setVisible(true);
            f_sk42y->setVisible(true);
        }
    }

    f_altitude->setVisible(true);
   // f_matrix_altitude->setVisible(true);
    f_setAltFromMatrix->setVisible(true);
    f_setPOI->setVisible(true);

    f_latitudeBinding->setVisible(false);
    f_longitudeBinding->setVisible(false);
    f_altitudeBinding->setVisible(false);
    fillPOI();
}

void POIControl::onTriggered()
{
    f_cooType->setVisible(false);
    f_latitude->setVisible(false);
    f_longitude->setVisible(false);
    f_sk42x->setVisible(false);
    f_sk42y->setVisible(false);
    f_altitude->setVisible(false);
   // f_matrix_altitude->setVisible(false);
    f_setAltFromMatrix->setVisible(false);
    f_setPOI->setVisible(false);

    f_latitudeBinding->setVisible(true);
    f_longitudeBinding->setVisible(true);
    f_altitudeBinding->setVisible(true);
}

void POIControl::onErrorOccured(const QString &error) const
{
    apxMsgW() << error;
}
void POIControl::onInfoOccured(const QString &info) const
{
    apxMsg() << info;
}
