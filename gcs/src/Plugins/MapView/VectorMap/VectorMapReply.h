#ifndef VectorMapReply_H
#define VectorMapReply_H
#include <QtLocation/private/qgeotiledmapreply_p.h>
class VectorTileFetcher;
//=============================================================================
class VectorMapReply : public QGeoTiledMapReply
{
    Q_OBJECT
public:
    VectorMapReply(const QGeoTileSpec &spec, VectorTileFetcher *fetcher);
    ~VectorMapReply();
    void abort();

private slots:
    void tileLoaded(quint64 uid, QByteArray data);
    void tileError(quint64 uid, QString errorString);

private:
    quint64 _uid;

    QString getImageFormat(const QByteArray &image);

private:
    VectorTileFetcher *fetcher;
};
//=============================================================================
#endif
