#ifndef VectorTiledMappingManagerEngine_H
#define VectorTiledMappingManagerEngine_H
#include <QtLocation/QGeoServiceProvider>
#include <QtLocation/private/qgeotiledmap_p.h>
#include <QtLocation/private/qgeotiledmappingmanagerengine_p.h>
#define MIN_MAP_ZOOM (2.0)
#define MAX_MAP_ZOOM (20.0)
//=============================================================================
class VectorTiledMap : public QGeoTiledMap
{
    Q_OBJECT
public:
    VectorTiledMap(QGeoTiledMappingManagerEngine *engine, QObject *parent = nullptr);
};
//=============================================================================
class VectorTiledMappingManagerEngine : public QGeoTiledMappingManagerEngine
{
    Q_OBJECT
public:
    VectorTiledMappingManagerEngine(const QVariantMap &parameters,
                                 QGeoServiceProvider::Error *error,
                                 QString *errorString);
    ~VectorTiledMappingManagerEngine();
    QGeoMap *createMap();
public slots:
    void clearTileCache();
};
//=============================================================================
#endif
