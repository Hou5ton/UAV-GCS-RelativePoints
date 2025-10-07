#include <App/AppLog.h>
#include "VectorTiledMappingManagerEngine.h"
#include "VectorPlugin.h"
#include "VectorTileFetcher.h"
#include "VectorTileLoader.h"

#include <QtLocation/private/qgeocameracapabilities_p.h>
#include <QtLocation/private/qgeofiletilecache_p.h>
#include <QtLocation/private/qgeomaptype_p.h>
#include <QtLocation/private/qgeotiledmap_p.h>

#include <QDir>
#include <QStandardPaths>
#include <QtQml>
//=============================================================================
VectorTiledMap::VectorTiledMap(QGeoTiledMappingManagerEngine *engine, QObject *parent)
    : QGeoTiledMap(engine, parent)
{
    setPrefetchStyle(PrefetchTwoNeighbourLayers); //PrefetchTwoNeighbourLayers);//NoPrefetching);
}
//=============================================================================
VectorTiledMappingManagerEngine::VectorTiledMappingManagerEngine(const QVariantMap &parameters,
                                                           QGeoServiceProvider::Error *error,
                                                           QString *errorString)
    : QGeoTiledMappingManagerEngine()
{
    Q_UNUSED(parameters)

    //qmlRegisterUncreatableType<GeoTiledMap>("APX.Map", 1, 0, "GeoTiledMap", "Reference only");

    QGeoCameraCapabilities cameraCaps;
    cameraCaps.setMinimumZoomLevel(MIN_MAP_ZOOM);
    cameraCaps.setMaximumZoomLevel(MAX_MAP_ZOOM);
    cameraCaps.setSupportsBearing(true);
    cameraCaps.setSupportsTilting(true);
    cameraCaps.setSupportsRolling(true);
    cameraCaps.setOverzoomEnabled(true);
    cameraCaps.setMinimumFieldOfView(5);
    cameraCaps.setMaximumFieldOfView(75);
    cameraCaps.setMinimumTilt(0);
    cameraCaps.setMaximumTilt(85);
    setCameraCapabilities(cameraCaps);

    setTileSize(QSize(256, 256));
    setCacheHint(QAbstractGeoTileCache::MemoryCache);

    const QByteArray pname("SXF");
    //QGeoCameraCapabilities ccaps;

    setTileFetcher(new VectorTileFetcher(this));

    *error = QGeoServiceProvider::NoError;
    errorString->clear();
}
//=============================================================================
VectorTiledMappingManagerEngine::~VectorTiledMappingManagerEngine() {}
//=============================================================================
QGeoMap *VectorTiledMappingManagerEngine::createMap()
{
    return new VectorTiledMap(this);
}

void VectorTiledMappingManagerEngine::clearTileCache()
{
    QAbstractGeoTileCache * tCache = tileCache();
    if (tCache)
    {
        apxConsole() << tr("erased %1 bytes from memory, %2 bytes on disk")
                        .arg(tCache->memoryUsage())
                        .arg(tCache->diskUsage());
        tCache->clearAll();
    }
}
//=============================================================================
