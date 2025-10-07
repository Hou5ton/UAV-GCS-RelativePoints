import QtQuick 2.13
import QtLocation 5.13
import QtPositioning 5.13
import QtQml 2.13
import KmlGeoPolygon 1.0

MapItemGroup {
    id: control

    property var map: ui.map
    property var area: apx.tools.missionplanner.area
    property var plugin: apx.tools.kmloverlay
    property var kmlCenter: plugin.center
    property var zoomLevel: map.zoomLevel
    property bool useOld: plugin.use_old.value

    onKmlCenterChanged: {
        ui.map.centerOn(kmlCenter)
    }
    onAreaChanged: {
        plugin.updateKmlModels(area)
    }

    onZoomLevelChanged: {
       // console.log("zoomLevel: " + zoomLevel);
    }

    // При смене режима очищаем и перезагружаем Loader
    onUseOldChanged: {
        // При необходимости можно очистить карту от предыдущих элементов здесь

        viewLoader.active = false;
        viewLoader.sourceComponent = useOld ? oldView : newViews;
        viewLoader.active = true;
    }

    Loader {
        id: viewLoader
        anchors.fill: parent
        active: true
        sourceComponent: useOld ? oldView : newViews
        onLoaded: {
            // Добавляем элемент(ы) на карту
            if (useOld) {
                map.addMapItemView(item);
            } else {
                // Для нового отображения используем алиасы, чтобы получить доступ к MapItemView внутри контейнера.
                map.addMapItemView(item.polygonView);
                map.addMapItemView(item.textView);
            }
        }
    }

    // Компонент для старого способа (useOld = true)
    Component {
        id: oldView
        MapItemView {
            id: borderPointsView
            model: plugin.kmlPolygons
            z: 1
            delegate: MapQuickItem {
                // Координата не влияет на отрисовку полигона, так как геометрия передаётся через geoPolygon
                coordinate: control.area.boundingGeoRectangle().topLeft
                sourceItem: KmlGeoPolygon {
                    id: kmlPolygon
                    area: control.area
                    map: ui.map
                    geoPolygon: polygon
                    color: polygonColor
                    name: polygonName
                    opacity: plugin.opacity.value
                    displayName: plugin.display_name.value
                }
            }
        }
    }

    // Компонент для нового отображения (useOld = false)
    Component {
        id: newViews
        Item {
            id: newContainer
            anchors.fill: parent

            // Алиасы для доступа к дочерним MapItemView
            property alias polygonView: polygonView
            property alias textView: textView

            // MapItemView для отрисовки контура полигона
            MapItemView {
                id: polygonView
                model: plugin.kmlPolygons
                z: 1
                delegate: MapPolygon {
                    path: model.polygon.perimeter
                    color: polygonColor
                    opacity: plugin.opacity.value
                }
            }
            // MapItemView для текстовой метки в центре полигона
            MapItemView {
                id: textView
                model: plugin.kmlPolygons
                z: 2
                delegate: MapQuickItem {
                    coordinate: control.computeCenter(polygon.perimeter)
                    sourceItem: Text {
                        text: polygonName
                        font.bold: true
                        font.pixelSize: map.fontSize
                        color: "black"
                        anchors.centerIn: parent
                        opacity: plugin.opacity.value
                    }
                }
                visible:  plugin.display_name.value && zoomLevel > 12
            }
        }
    }

    // Функция для вычисления центра полигона по массиву точек
    function computeCenter(points) {
        var latSum = 0;
        var lonSum = 0;
        for (var i = 0; i < points.length; i++) {
            latSum += points[i].latitude;
            lonSum += points[i].longitude;
        }
        return QtPositioning.coordinate(latSum / points.length, lonSum / points.length);
    }
}
