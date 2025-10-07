#include "kmlparser.h"

#include <App/AppLog.h>
#include <QDomNodeList>

using namespace std::placeholders;

KmlParser::KmlParser() {}

void KmlParser::parse(const QByteArray &data)
{
    m_polygonId = 0;
    m_polygons.clear();
    m_style.clear();

    QString errorMessage;
    int errorLine;
    if (m_dom.setContent(data, &errorMessage, &errorLine)) {
        auto cb = std::bind(&KmlParser::styleMapCallback, this, _1);
        iterateOverChildrenElements(m_dom.documentElement(), "StyleMap", cb);
        auto cb_ = std::bind(&KmlParser::styleCallback, this, _1);
        iterateOverChildrenElements(m_dom.documentElement(), "gx:CascadingStyle", cb_);
        auto cb__ = std::bind(&KmlParser::placemarkCallback, this, _1);
        iterateOverChildrenElements(m_dom.documentElement(), "Placemark", cb__);
    } else
        apxMsgW() << QString("%1 at line %2").arg(errorMessage, errorLine);
}

QList<KmlPolygon> KmlParser::getPolygons()
{
    return m_polygons;
}

QList<KmlStyle> KmlParser::getStyles()
{
    return m_style;
}

QList<StyleMap> KmlParser::getStylesMap()
{
    return m_style_map;
}


void KmlParser::iterateOverChildrenElements(const QDomElement &parent,
                                            const QString &tagname,
                                            const IterateCallback &cb)
{
    auto children = parent.elementsByTagName(tagname);
    for (int i = 0; i < children.size(); i++) {
        auto c = children.at(i).toElement();
        if (!c.isNull())
            cb(c);
    }
}

void KmlParser::styleMapCallback(const QDomElement &el)
{
    StyleMap style;

    QDomNamedNodeMap attributes = el.attributes();
    for (int i = 0; i < attributes.count(); i++) {
        QDomNode attrNode = attributes.item(i);

        if (!attrNode.isNull()) {
            QDomAttr attr = attrNode.toAttr();
           // qDebug() << "attr.name():" << attr.name() << "=" << attr.value();
            style.id = "#" + attr.value();

            QDomNodeList pairList = el.elementsByTagName("Pair");
            for (int i = 0; i < pairList.count(); ++i) {
                QDomElement pairElem = pairList.at(i).toElement();
                if (!pairElem.isNull()) {
                    // Извлекаем значения из элементов <key> и <styleUrl>
                    QDomElement keyElem = pairElem.firstChildElement("key");
                    QDomElement styleUrlElem = pairElem.firstChildElement("styleUrl");
                    QString keyText = keyElem.text().trimmed();
                    QString styleUrlText = styleUrlElem.text().trimmed();

                    QPair<QString, QString> pair = qMakePair(keyText,styleUrlText);

                    style.info.append(pair);
                }
            }
            m_style_map.append(style);
        }
    }
}

void KmlParser::styleCallback(const QDomElement &el)
{
    KmlStyle style;

    //polygon style parser
    QDomNamedNodeMap attributes = el.attributes();
    for (int i = 0; i < attributes.count(); i++) {
        QDomNode attrNode = attributes.item(i);

        if (!attrNode.isNull()) {
            QDomAttr attr = attrNode.toAttr();
            //qDebug() << "attr.name():" << attr.name() << "=" << attr.value();
            style.id = "#" + attr.value();

            QColor polygonColor("red");
            auto styles = el.elementsByTagName("PolyStyle");
            if (!styles.isEmpty()) {
                auto color = styles.at(0).toElement().elementsByTagName("color");
                if (!color.isEmpty()){
                    polygonColor.setNamedColor("#" + color.at(0).toElement().text());
                }
                style.color = polygonColor;
                m_style.append(style);
            }
        }
    }
}

void KmlParser::placemarkCallback(const QDomElement &el)
{
    //polygon style parser
    QColor polygonColor("red");
    QString polygonName = "";
    auto styles = el.elementsByTagName("PolyStyle");
    if (!styles.isEmpty()) {
        auto color = styles.at(0).toElement().elementsByTagName("color");
        if (!color.isEmpty()){
            polygonColor.setNamedColor("#" + color.at(0).toElement().text());
        }else{
            styles = el.elementsByTagName("LineStyle");
            if (!styles.isEmpty()) {
                auto color = styles.at(0).toElement().elementsByTagName("color");
                if (!color.isEmpty()){
                    polygonColor.setNamedColor("#" + color.at(0).toElement().text());
                }
            }
        }

        if (!polygonColor.isValid())
            qDebug() << "not valid";
    }else{
        styles = el.elementsByTagName("styleUrl");
        if (!styles.isEmpty()) {
            QString style_ = styles.at(0).toElement().text();

            for (const StyleMap &style_map : getStylesMap()) {
                if(style_map.id == style_){
                    if(style_map.info.size()){
                        //tmp
                        QString style__ = style_map.info.at(0).second;
                        for (const KmlStyle &style_kml : getStyles()) {
                            if(style_kml.id == style__){
                                polygonColor = style_kml.color;
                            }
                        }
                    }
                }
            }
        }
    }

    auto names = el.elementsByTagName("name");
    if (!names.isEmpty()) {
        if (!names.isEmpty()) {
            polygonName = names.at(0).toElement().text();
        }
    }

    auto cb = std::bind(&KmlParser::polygonCallback, this, _1, polygonColor,polygonName);
    iterateOverChildrenElements(el, "Polygon", cb);
}

void KmlParser::polygonCallback(const QDomElement &el,
                                const QColor &color,
                                const QString &name)
{
    KmlPolygon polygon;
    polygon.color = color;
    polygon.name = name;
    polygon.id = m_polygonId++;
    auto cbouter = std::bind(&KmlParser::polygonOuterCallback, this, _1, std::ref(polygon));
    auto cbinner = std::bind(&KmlParser::polygonInnerCallback, this, _1, std::ref(polygon));
    iterateOverChildrenElements(el, "outerBoundaryIs", cbouter);
    iterateOverChildrenElements(el, "innerBoundaryIs", cbinner);

    m_polygons.append(polygon);
}

void KmlParser::polygonOuterCallback(const QDomElement &el, KmlPolygon &polygon)
{
    auto cb = std::bind(&KmlParser::polygonCoordinatesCallback, this, _1, std::ref(polygon));
    iterateOverChildrenElements(el, "coordinates", cb);
}

void KmlParser::polygonInnerCallback(const QDomElement &el, KmlPolygon &polygon)
{
    auto cb = std::bind(&KmlParser::polygonHolesCallback, this, _1, std::ref(polygon));
    iterateOverChildrenElements(el, "coordinates", cb);
}

void KmlParser::polygonCoordinatesCallback(const QDomElement &el, KmlPolygon &polygon)
{
    auto coordinates = parseCoordinates(el.text());
    for (auto &c : coordinates)
        polygon.data.addCoordinate(c);
}

void KmlParser::polygonHolesCallback(const QDomElement &el, KmlPolygon &polygon)
{
    auto coordinates = parseCoordinates(el.text());
    polygon.data.addHole(coordinates);
}

QList<QGeoCoordinate> KmlParser::parseCoordinates(const QString &text)
{
    QList<QGeoCoordinate> result;
    QStringList tuples = text.simplified().split(" ", QString::SkipEmptyParts);
    for (auto &t : tuples) {
        QStringList coordinates = t.split(",", QString::SkipEmptyParts);
        bool ok1, ok2;
        double lon = t.section(",", 0, 0).toDouble(&ok1);
        double lat = t.section(",", 1, 1).toDouble(&ok2);
        if (ok1 && ok2) {
            result.append(QGeoCoordinate(lat, lon));
        } else
            apxMsgW() << "Can't parse lat-lon from string " << t;
    }
    return result;
}
