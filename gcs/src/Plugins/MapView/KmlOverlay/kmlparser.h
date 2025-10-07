#ifndef KMLPARSER_H
#define KMLPARSER_H

#include <functional>
#include <QColor>
#include <QDomDocument>
#include <QGeoPolygon>

struct KmlPolygon
{
    uint64_t id;
    QColor color;
    QString name;
    QGeoPolygon data;
};

struct StyleMap {
    QString id;
    QList<QPair<QString, QString>> info;
};

struct KmlStyle
{
    QString id;
    QColor color;
};

class KmlParser
{
public:
    KmlParser();
    void parse(const QByteArray &data);
    QList<KmlPolygon> getPolygons();
    QList<KmlStyle> getStyles();
    QList<StyleMap> getStylesMap();

private:
    using IterateCallback = std::function<void(const QDomElement &)>;

    QList<KmlPolygon> m_polygons;
    QList<KmlStyle> m_style;
    QList<StyleMap> m_style_map;
    QDomDocument m_dom;
    uint64_t m_polygonId;

    void iterateOverChildrenElements(const QDomElement &parent,
                                     const QString &tagname,
                                     const IterateCallback &cb);

    void styleMapCallback(const QDomElement &el);
    void styleCallback(const QDomElement &el);
    void placemarkCallback(const QDomElement &el);
    void polygonCallback(const QDomElement &el,
                         const QColor &color,
                         const QString &name);
    void polygonOuterCallback(const QDomElement &el, KmlPolygon &polygon);
    void polygonInnerCallback(const QDomElement &el, KmlPolygon &polygon);
    void polygonCoordinatesCallback(const QDomElement &el, KmlPolygon &polygon);
    void polygonHolesCallback(const QDomElement &el, KmlPolygon &polygon);
    QList<QGeoCoordinate> parseCoordinates(const QString &text);
};

#endif // KMLPARSER_H
