import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtLocation 5.12
import QtPositioning 5.12
import Apx.Map 1.0
import Apx.Map.Common 1.0
import Apx.Common 1.0
import APX.Vehicles 1.0 as APX


MapItemGroup {
    id: mapGroup

    // Простейшая модель данных для таблицы
    ListModel {
        id: myModel
    }
    property var relativepoints: apx.tools.relativepoints
    property APX.Vehicle vehicle: apx.vehicles.current
    property var vm: vehicle.mandala
    property real f_altitude: vm.altitude.value
    property var coord: QtPositioning.coordinate(vehicle.coordinate.latitude,
                                                 vehicle.coordinate.longitude,
                                                 f_altitude)

    property var tableGeoCoordinate: map ?
        map.toCoordinate(Qt.point(map.fromCoordinate(coord).x + tableHolder.x,
                                     map.fromCoordinate(coord).y + tableHolder.y))
        : coord

    property var columns_count: 4

    // Здесь предполагается, что modelDataExternal – внешняя модель данных
    property var modelDataExternal: relativepoints.lookup.dbModel

    onCoordChanged: {
        //console.log("coord изменилось:", coord);
        parceModelData();
        if (isValidCoordinate(coord) && isValidCoordinate(tableGeoCoordinate)) {
            dynamicLine.path = [coord, tableGeoCoordinate];
        } else {
            console.warn("Invalid coordinates detected");
        }
    }

    onTableGeoCoordinateChanged: {
        if (isValidCoordinate(coord) && isValidCoordinate(tableGeoCoordinate)) {
            dynamicLine.path = [coord, tableGeoCoordinate];
        } else {
            console.warn("Invalid coordinates detected");
        }
    }


    MapQuickItem {
        id: tableContainer
        coordinate: coord
        // Выравнивание относительно содержимого (с учётом фиксированного отступа)
        anchorPoint.x: tableHolder.width / 2
        anchorPoint.y: tableHolder.height

        // Все визуальные элементы находятся внутри одного элемента sourceItem
        sourceItem: Rectangle {
            id: tableHolder
            color: "white"
            border.color: "black"
            border.width: 1
            // Отступы по 10 пикселей вокруг таблицы
            implicitWidth: gridTable.implicitWidth + 10
            implicitHeight: gridTable.implicitHeight + 10

            // Добавляем MouseArea для перетаскивания и обработки двойного клика
            MouseArea {
                id: dragArea
                anchors.fill: parent
                drag.target: parent
                // Обработчик двойного клика – сбрасываем смещение
                onDoubleClicked: {
                    // При желании можно добавить анимацию для плавного возвращения
                    tableHolder.x = 0;
                    tableHolder.y = 0;
                    console.log("Возврат таблицы на исходное место");
                }
            }

            GridLayout {
                id: gridTable
                columns: columns_count
                rowSpacing: 4
                columnSpacing: 4

                // Заголовок таблицы (первая строка)
                Label {
                    text: qsTr("Key")
                    font.bold: true
                    color: "black"
                    Layout.alignment: Qt.AlignHCenter
                }
                Label {
                    text: qsTr("Az")
                    font.bold: true
                    color: "black"
                    Layout.alignment: Qt.AlignHCenter
                }
                Label {
                    text: qsTr("Dist")
                    font.bold: true
                    color: "black"
                    Layout.alignment: Qt.AlignHCenter
                }
                Label {
                    text: qsTr("Alt")
                    font.bold: true
                    color: "black"
                    Layout.alignment: Qt.AlignHCenter
                }

                // Используем Repeater для создания ячеек данных.
                // Всего ячеек: количество строк (myModel.count) * columns_count
                Repeater {
                    // Количество ячеек равно количеству элементов * columns_count
                    model: myModel.count * columns_count
                    delegate: Label {
                        // cellIndex: 0 - ключ
                        property int cellIndex: index % columns_count
                        // rowIndex вычисляется как индекс строки; Math.floor используется для округления вниз.
                        property int rowIndex: Math.floor(index / columns_count)
                        text: {
                            var dataItem = myModel.get(rowIndex)
                            if (!dataItem)
                                return ""
                            switch (cellIndex) {
                                case 0: return dataItem.key
                                case 1: return dataItem.azimuth
                                case 2: return dataItem.distance
                                case 3: return dataItem.altitude
                                default: return ""
                            }
                        }
                        color: "black"
                        Layout.alignment: Qt.AlignHCenter
                    }
                }
            }

            Component.onCompleted: {
                parceModelData();
            }
        }
    }

    MapPolyline {
        id: dynamicLine
        line.width: 2
        line.color: "black"

        /*path: [
            coord,
            tableGeoCoordinate
        ]*/
    }

    function isValidCoordinate(coord) {
        return coord && !isNaN(coord.latitude) && !isNaN(coord.longitude);
    }

    function parceModelData() {
        //console.log("Обновление данных. modelDataExternal.count: " + modelDataExternal.count);

        // 1. Удаление элементов из myModel, которых уже нет в modelDataExternal
        // Итерируем в обратном порядке, чтобы безопасно удалять элементы
        for (var j = myModel.count - 1; j >= 0; j--) {
            var exists = false;
            var currentItem = myModel.get(j);
            // Проверяем, существует ли запись с таким же ключом в modelData
            for (var i = 0; i < modelDataExternal.count; i++) {
                var dataItem = modelDataExternal.get(i);
                if (dataItem.disp_on_map && dataItem.key === currentItem.key) {
                    exists = true;
                    break;
                }
            }
            if (!exists) {
                console.log("Удаляем элемент из myModel: " + currentItem.key);
                myModel.remove(j);
            }
        }

        // 2. Обновление существующих элементов или добавление новых из modelDataExternal
        for (var i = 0; i < modelDataExternal.count; i++) {
            var item = modelDataExternal.get(i);
            if (item.disp_on_map) {
                var found = false;
                for (var j = 0; j < myModel.count; j++) {
                    var currentItem = myModel.get(j);
                    if (currentItem.key === item.key) {
                        var p1 = QtPositioning.coordinate(item.lat,
                                                          item.lon,
                                                          item.alt);
                        var p2 = QtPositioning.coordinate(vehicle.coordinate.latitude,
                                                          vehicle.coordinate.longitude,
                                                          f_altitude);
                        var azimuth = p1.azimuthTo(p2).toFixed() + "°";
                        var distance = Apx.formatDistance(p1.distanceTo(p2));
                        var altitude = Apx.formatDistance(f_altitude);

                        // Обновляем данные
                        myModel.set(j, { "key": item.key, "azimuth": azimuth, "distance": distance, "altitude": altitude });
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    var p1 = QtPositioning.coordinate(item.lat,
                                                      item.lon,
                                                      item.alt);
                    var p2 = QtPositioning.coordinate(vehicle.coordinate.latitude,
                                                      vehicle.coordinate.longitude,
                                                      f_altitude);
                    var azimuth = p1.azimuthTo(p2).toFixed() + "°";
                    var distance = Apx.formatDistance(p1.distanceTo(p2));
                    var altitude = Apx.formatDistance(f_altitude);

                    myModel.append({
                        "key": item.key,
                        "azimuth": azimuth,
                        "distance": distance,
                        "altitude": altitude
                    });
                }
            }
        }
    }
}
