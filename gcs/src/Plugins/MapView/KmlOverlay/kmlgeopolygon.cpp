#include "kmlgeopolygon.h"

#include <cmath>
#include <QGeoRectangle>
#include <QPainter>

KmlGeoPolygon::KmlGeoPolygon(QQuickItem *parent)
    : QQuickPaintedItem(parent)
    , m_map(nullptr)
{
    setRenderTarget(QQuickPaintedItem::FramebufferObject);
    setAntialiasing(false);
    setMipmap(true);
}

void KmlGeoPolygon::registerQmlType()
{
    qmlRegisterType<KmlGeoPolygon>("KmlGeoPolygon", 1, 0, "KmlGeoPolygon");
}

void KmlGeoPolygon::clearPolygon()
{
    m_polygon.clear();
    m_holes.clear();
}

void KmlGeoPolygon::prepareForDrawing()
{
    QRegion clip(m_polygon);

    for (int i = 0; i < m_holes.size(); i++)
        clip -= QRegion(m_holes[i]);

    QRegion window(QRect(x(), y(), width(), height()));
    m_clip = clip; //.intersected(window);
}

void KmlGeoPolygon::setGeoPolygon(const QGeoPolygon &geoPolygon)
{
    m_geoPolygon = geoPolygon;
    recalcCoordinates();
    prepareForDrawing();
}

QGeoPolygon KmlGeoPolygon::getGeoPolygon() const
{
    return m_geoPolygon;
}

void KmlGeoPolygon::setMap(QObject *map)
{
    m_map = map;
    recalcCoordinates();
    prepareForDrawing();
}

QObject *KmlGeoPolygon::getMap() const
{
    return m_map;
}

QGeoShape KmlGeoPolygon::getArea() const
{
    return m_area;
}

void KmlGeoPolygon::setArea(const QGeoShape &area)
{
    m_area = area;
    if (m_map) {
        recalcCoordinates();
        prepareForDrawing();
        update();
    }
}

void KmlGeoPolygon::setColor(const QColor &color)
{
    if (m_color != color) {
        m_color = color;
        emit colorChanged();
        update();
    }
}

QColor KmlGeoPolygon::getColor() const
{
    return m_color;
}

void KmlGeoPolygon::setName(const QString &name)
{
    if (m_name != name) {
        m_name = name;
        emit nameChanged();
        update();
    }
}

QString KmlGeoPolygon::getName() const
{
    return m_name;
}

void KmlGeoPolygon::setDisplayName(const bool &display_name)
{
    if (m_display_name != display_name) {
        m_display_name = display_name;
        emit displayNameChanged();
        update();
    }
}
bool KmlGeoPolygon::getDisplayName() const
{
    return m_display_name;
}


/*void KmlGeoPolygon::paint(QPainter *painter)
{
    painter->setPen(m_color);

    painter->setClipRegion(m_clip);
    QPainterPath p;
    p.addPolygon(m_polygon);
    painter->fillPath(p, QBrush(m_color, Qt::SolidPattern));
}*/

void KmlGeoPolygon::paint(QPainter *painter)
{
    painter->setPen(m_color);
    painter->setClipRegion(m_clip);
    QPainterPath polygonPath;
    polygonPath.addPolygon(m_polygon);
    painter->fillPath(polygonPath, QBrush(m_color, Qt::SolidPattern));

    if (m_display_name &&
            !m_polygon.isEmpty()) {
        double area = 0.0;
        double cx = 0.0;
        double cy = 0.0;
        int n = m_polygon.size();

        // Вычисление центроида (геометрического центра) многоугольника
        for (int i = 0; i < n; ++i) {
            const QPoint &current = m_polygon[i];
            const QPoint &next = m_polygon[(i + 1) % n];
            double factor = current.x() * next.y() - next.x() * current.y();
            area += factor;
            cx += (current.x() + next.x()) * factor;
            cy += (current.y() + next.y()) * factor;
        }
        area /= 2.0;

        // Если площадь многоугольника равна нулю или очень мала, выход
        if (qFuzzyCompare(area, 0.0))
            return;

        cx /= (6 * area);
        cy /= (6 * area);
        QPoint centerPoint(qRound(cx), qRound(cy));

        // Подготовка текста для отрисовки
        QString text = m_name;
        // Вычисляем прямоугольник, занимаемый текстом, согласно текущим настройкам шрифта
        QRect textRect = painter->fontMetrics().boundingRect(text);
        // Центрируем полученный прямоугольник относительно вычисленного центроида
        textRect.moveCenter(centerPoint);

        // Проверяем, что область, где будет отображаться текст, имеет пересечение с клип-областью m_clip.
        // Если текстовая область пересекается с m_clip, текст будет видимым,
        // в противном случае текст не отрисовывается, так как находится вне клиповой области.
        if (m_clip.intersects(textRect)) {
            QPen textPen(Qt::black);
            painter->setPen(textPen);
            painter->drawText(textRect, Qt::AlignCenter, text);
        }
    }
}


QPoint KmlGeoPolygon::fromCoordinate(const QGeoCoordinate &c)
{
    QPointF point;
    QMetaObject::invokeMethod(m_map,
                              "fromCoordinate",
                              Q_RETURN_ARG(QPointF, point),
                              Q_ARG(QGeoCoordinate, c),
                              Q_ARG(bool, false));
    return point.toPoint();
}

void KmlGeoPolygon::recalcCoordinates()
{
    clearPolygon();
    auto visibleRegion = m_map->property("visibleRegion").value<QGeoShape>().boundingGeoRectangle();
    QRect vr(fromCoordinate(visibleRegion.topLeft()), fromCoordinate(visibleRegion.bottomRight()));
    for (int i = 0; i < m_geoPolygon.size(); i++) {
        QPoint point = fromCoordinate(m_geoPolygon.coordinateAt(i));
        if (std::isnan(point.x()) || std::isnan(point.y()))
            continue;

        m_polygon.append(point);
    }
    for (int j = 0; j < m_geoPolygon.holesCount(); j++) {
        QPolygon polyHole;
        for (int k = 0; k < m_geoPolygon.hole(j).size(); k++) {
            QPoint point = fromCoordinate(m_geoPolygon.hole(j)[k].value<QGeoCoordinate>());
            if (std::isnan(point.x()) || std::isnan(point.y()))
                continue;
            polyHole.append(point);
        }
        m_holes.append(polyHole);
    }

    setX(vr.x());
    setY(vr.y());
    setWidth(vr.width());
    setHeight(vr.height());
}
