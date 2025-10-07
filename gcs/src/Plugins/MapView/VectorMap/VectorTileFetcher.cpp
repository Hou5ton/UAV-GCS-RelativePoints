#include "VectorTileFetcher.h"
#include "VectorMapReply.h"
#include "VectorTileLoader.h"
#include "VectorTiledMappingManagerEngine.h"
#include <QtLocation/private/qgeotilespec_p.h>
//=============================================================================
//=============================================================================
VectorTileFetcher::VectorTileFetcher(QGeoTiledMappingManagerEngine *parent)
    : QGeoTileFetcher(parent)
{
    //-- Check internet status every 30 seconds or so
    //connect(&_timer, &QTimer::timeout, this, &GeoTileFetcher::timeout);
    //_timer.setSingleShot(false);
    //_timer.start(30000);

    loader = VectorTileLoader::instance();
    connect(loader,
            &VectorTileLoader::requestEngieClearCache,
            dynamic_cast<VectorTiledMappingManagerEngine*>(parent),
            &VectorTiledMappingManagerEngine::clearTileCache);

    if (!loader) {
        loader = new VectorTileLoader();        
        connect(loader,
                &VectorTileLoader::requestEngieClearCache,
                dynamic_cast<VectorTiledMappingManagerEngine*>(parent),
                &VectorTiledMappingManagerEngine::clearTileCache);
    }
}
//=============================================================================
VectorTileFetcher::~VectorTileFetcher() {}
//=============================================================================
QGeoTiledMapReply *VectorTileFetcher::getTileImage(const QGeoTileSpec &spec)
{
    return new VectorMapReply(spec, this);
}
//=============================================================================
