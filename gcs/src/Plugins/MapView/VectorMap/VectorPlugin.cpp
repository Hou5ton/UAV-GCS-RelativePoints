#include "VectorPlugin.h"
#include "VectorTiledMappingManagerEngine.h"
#include <QtLocation/private/qgeotiledmappingmanagerengine_p.h>
//=============================================================================
Q_EXTERN_C Q_DECL_EXPORT const char *qt_plugin_query_metadata();
Q_EXTERN_C Q_DECL_EXPORT QT_PREPEND_NAMESPACE(QObject) * qt_plugin_instance();
//=============================================================================
void VectorPlugin::init()
{
    //load static plugin
    Q_IMPORT_PLUGIN(VectorPlugin)
}
//=============================================================================
const QT_PREPEND_NAMESPACE(QStaticPlugin) qt_static_plugin_VectorPlugin()
{
    QT_PREPEND_NAMESPACE(QStaticPlugin) plugin = {qt_plugin_instance, qt_plugin_query_metadata};
    return plugin;
}
//=============================================================================
QGeoCodingManagerEngine *VectorPlugin::createGeocodingManagerEngine(const QVariantMap &,
                                                                 QGeoServiceProvider::Error *,
                                                                 QString *) const
{
    //qDebug()<<"createGeocodingManagerEngine";
    return nullptr;
    //return new QGeoCodingManagerEngineAPX(parameters, error, errorString);
}
//=============================================================================
QGeoMappingManagerEngine *VectorPlugin::createMappingManagerEngine(const QVariantMap &parameters,
                                                                QGeoServiceProvider::Error *error,
                                                                QString *errorString) const
{
    //qDebug()<<"createMappingManagerEngine";
    return new VectorTiledMappingManagerEngine(parameters, error, errorString);
}
//=============================================================================
QGeoRoutingManagerEngine *VectorPlugin::createRoutingManagerEngine(const QVariantMap &,
                                                                QGeoServiceProvider::Error *,
                                                                QString *) const
{
    return nullptr;
}
//=============================================================================
QPlaceManagerEngine *VectorPlugin::createPlaceManagerEngine(const QVariantMap &,
                                                         QGeoServiceProvider::Error *,
                                                         QString *) const
{
    return nullptr;
}
//=============================================================================
