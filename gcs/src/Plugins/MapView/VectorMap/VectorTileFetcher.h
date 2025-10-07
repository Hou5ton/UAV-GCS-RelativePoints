#ifndef VectorTileFetcher_H
#define VectorTileFetcher_H
#include <QtLocation/private/qgeotilefetcher_p.h>
class VectorTileLoader;
class QGeoTiledMappingManagerEngine;
//=============================================================================
class VectorTileFetcher : public QGeoTileFetcher
{
    Q_OBJECT
public:
    explicit VectorTileFetcher(QGeoTiledMappingManagerEngine *parent = 0);
    ~VectorTileFetcher();

    VectorTileLoader *loader;

protected:
    QGeoTiledMapReply *getTileImage(const QGeoTileSpec &spec);
};
//=============================================================================
#endif
