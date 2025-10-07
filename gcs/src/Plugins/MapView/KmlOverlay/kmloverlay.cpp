#include "kmloverlay.h"

#include "kmlgeopolygon.h"
#include "kmlparser.h"
#include <App/App.h>
#include <QFileDialog>

KmlOverlay::KmlOverlay(Fact *parent)
    : Fact(parent,
           QString(PLUGIN_NAME).toLower(),
           tr("KML Overlay"),
           tr("KML objects overlay"),
           Group)
    , m_kmlPolygons(new KmlPolygonsModel())
{
    KmlGeoPolygon::registerQmlType();

    setIcon("google-earth");

    f_open = new Fact(this, "open", tr("Open..."), tr("Open KML file"));
    f_open->setIcon("import");

    f_visible = new Fact(this, "visible", tr("Show overlay"), "", Fact::Bool);
    f_visible->setIcon("check");
    f_visible->setValue(true);

    f_use_old = new Fact(this, "use_old", tr("Use old version"), "", Fact::Bool);
    f_use_old->setIcon("check");
    f_use_old->setValue(false);
    f_use_old->setVisible(false); //do not display


    f_display_name = new Fact(this, "display_name", tr("Display name"), "", Fact::Bool);
    f_display_name->setIcon("check");
    f_display_name->setValue(true);


    f_opacity = new Fact(this, "opacity", tr("Opacity"), "", Fact::Float);
    f_opacity->setMin(0);
    f_opacity->setMax(1);
    f_opacity->setValue(1);

    connect(f_open, &Fact::triggered, this, &KmlOverlay::onOpenTriggered);
    connect(f_visible, &Fact::valueChanged, this, &KmlOverlay::onOverlayVisibleValueChanged);

    loadQml("qrc:/" PLUGIN_NAME "/KmlOverlayPlugin.qml");
}

KmlPolygonsModel *KmlOverlay::getKmlPolygons() const
{
    return m_kmlPolygons;
}

QGeoCoordinate KmlOverlay::getCenter() const
{
    return m_center;
}

void KmlOverlay::updateKmlModels(const QGeoShape &shape)
{
    QRectF bb(gc2p(shape.boundingGeoRectangle().topLeft()),
              gc2p(shape.boundingGeoRectangle().bottomRight()));
    m_kmlPolygons->setBoundingBox(bb);
}

QPointF KmlOverlay::gc2p(const QGeoCoordinate &c)
{
    return QPointF(c.latitude(), c.longitude());
}

QGeoCoordinate KmlOverlay::p2gc(const QPointF &p)
{
    return QGeoCoordinate(p.x(), p.y());
}

void KmlOverlay::onOpenTriggered()
{
    QString path = QFileDialog::getOpenFileName(nullptr,
                                                tr("Open kml"),
                                                QDir::homePath(),
                                                "KML (*.kml)");
    if (!path.isEmpty()) {
        QFile file(path);
        file.open(QIODevice::ReadOnly);

        setDescr(path);

        m_parser.parse(file.readAll());
        file.close();

        m_center = p2gc(m_kmlPolygons->setPolygons(m_parser.getPolygons()));
        emit centerChanged();

        f_visible->setValue(true);
    }
}

void KmlOverlay::onOverlayVisibleValueChanged()
{
    if (f_visible->value().toBool())
        m_kmlPolygons->setPolygons(m_parser.getPolygons());
    else
        m_kmlPolygons->setPolygons({});
}


