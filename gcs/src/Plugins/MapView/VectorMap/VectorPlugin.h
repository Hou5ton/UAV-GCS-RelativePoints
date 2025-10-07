#ifndef VectorPlugin_H
#define VectorPlugin_H
#include "VectorTileLoader.h"
#include <App/PluginInterface.h>
#include <QtCore/QObject>
#include <QtCore>
#include <QtLocation/QGeoServiceProviderFactory>
#include <QtPlugin>
//=============================================================================
class VectorPlugin : public PluginInterface, public QGeoServiceProviderFactory
{
    Q_OBJECT
    Q_INTERFACES(QGeoServiceProviderFactory)
    Q_INTERFACES(PluginInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.qt.geoservice.serviceproviderfactory/5.0" FILE "VectorPlugin.json")

public:
    void init() override;
    int flags() override { return Feature | Map; }
    QString title() override { return tr("Vector map"); }
    QString descr() override { return tr("Vector map provider"); }
    QObject *createControl() override { return new VectorTileLoader(); }

    //override
    QGeoCodingManagerEngine *createGeocodingManagerEngine(const QVariantMap &parameters,
                                                          QGeoServiceProvider::Error *error,
                                                          QString *errorString) const override;
    QGeoMappingManagerEngine *createMappingManagerEngine(const QVariantMap &parameters,
                                                         QGeoServiceProvider::Error *error,
                                                         QString *errorString) const override;
    QGeoRoutingManagerEngine *createRoutingManagerEngine(const QVariantMap &parameters,
                                                         QGeoServiceProvider::Error *error,
                                                         QString *errorString) const override;
    QPlaceManagerEngine *createPlaceManagerEngine(const QVariantMap &parameters,
                                                  QGeoServiceProvider::Error *error,
                                                  QString *errorString) const override;
};
//=============================================================================
#endif
