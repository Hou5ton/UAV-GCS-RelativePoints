#ifndef POICONTROL_H
#define POICONTROL_H

#include <Fact/Fact.h>
#include <Vehicles/VehicleMandalaFact.h>
#include <QAbstractVideoSurface>
#include <QtCore>

class Vehicle;

class POIControl : public Fact
{
    Q_OBJECT
   // Q_PROPERTY(KmlPolygonsModel *kmlPolygons READ getKmlPolygons CONSTANT);
   // Q_PROPERTY(QGeoCoordinate center READ getCenter NOTIFY centerChanged)

public:
   enum SourceType { wgsDMS, wgsDD };

    explicit POIControl(Fact *parent = nullptr);
    Q_INVOKABLE void createPOIFromVideo();
    Fact *f_latitude;
    Fact *f_longitude;
    Fact *f_altitude;
    Fact *f_matrix_altitude;
    Fact *f_sk42x;
    Fact *f_sk42y;
    Fact *f_coordinateSystem;
    Fact *f_cooType;
    Fact *f_setPOI;
    Fact *f_setAltFromMatrix;

    Fact *f_latitudeBinding;
    Fact *f_longitudeBinding;
    Fact *f_altitudeBinding;

    VehicleMandalaFact *m_latitudeFact;
    VehicleMandalaFact *m_longitudeFact;
    VehicleMandalaFact *m_altitudeFact;
private:
    void fillPOI();
    Vehicle *m_vehicle = nullptr;
    //QList<Vehicle*> m_list_vehicle;

private slots:
    void onLongitudeBindingChanged();
    void onLatitudeBindingChanged();
    void onAltitudeBindingChanged();
    void onCooTypeChanged();
    void onAltitudeChanged();
    void onMatrixAltitudeChanged();
    void onLatLonChanged();
    void onLatLonSK42Changed();
    void onRemoveSettings();
    void onSetPOI();
    void onSetAltFromMatrix();
    void onTriggered();
    void onErrorOccured(const QString &) const;
    void onInfoOccured(const QString &) const;
    void onVehicleSelected(Vehicle *vehicle);
    void onCoordinateSystemChanged();

signals:
    void setPOI(POIControl *);
    void setAltFromMatrix(POIControl *);

};

#endif //POICONTROL_H
