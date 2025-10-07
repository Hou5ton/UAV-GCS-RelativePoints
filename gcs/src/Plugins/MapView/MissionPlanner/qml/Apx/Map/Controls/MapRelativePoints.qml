import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtLocation 5.12
import QtPositioning 5.12
import Apx.Map 1.0
import Apx.Map.Common 1.0
import Apx.Common 1.0
import APX.Vehicles 1.0 as APX

RowLayout {

    readonly property int iconSize: key.implicitHeight*0.8
    property var relativepoints: apx.tools.relativepoints
    property APX.Vehicle vehicle: apx.vehicles.current
    property var vm: vehicle.mandala
    property real f_altitude: vm.altitude.value
    property var coord: QtPositioning.coordinate(vehicle.coordinate.latitude,
                                                 vehicle.coordinate.longitude,
                                                 f_altitude)
    property var modelDataExternal: relativepoints.lookup.dbModel
    property var currentIndex: -1
    property var currentKey: 0

    ListModel {
        id: myModel
    }

    onCoordChanged: {
        parceModelData();
    }

    onModelDataExternalChanged: {
        parceModelData();
    }

    function appendSorted(key,azimuth,distance,altitude) {
        var insertIndex = 0;
        for (; insertIndex < myModel.count; insertIndex++) {
            if (Number(myModel.get(insertIndex).key) > Number(key)) {
                break;
            }
        }

        myModel.insert(insertIndex, {
            "key": key,
            "azimuth": azimuth,
            "distance": distance,
            "altitude": altitude
        });
    }

    function setCurrents(key_, index_) {
        currentKey = key_;
        currentIndex = index_;
    }

    function nextItem() {
        var ind = currentIndex
        if( myModel.count){
            ind++;
            if(ind <= myModel.count-1){
                var currentItem = myModel.get(ind);
                setCurrents(currentItem.key,ind)
            }else{
                setCurrents(0, -1)
            }
        }else{
            setCurrents(0, -1)
            if(modelDataExternal.count){
                parceModelData();
            }
        }
        displayAddInfo();
       // console.log("current_index: " + currentIndex);
    }

     function displayAddInfo(){
         var prevKey = relativepoints.currentKey;
         var set = false;
         if(currentIndex != -1){
             if(myModel.count > currentIndex){
                var currentItem = myModel.get(currentIndex);
                 //console.log("currentKey_: " + currentKey);
                if(prevKey !== currentItem.key){
                    set = true;
                     if (relativepoints) {
                         relativepoints.currentKey = currentItem.key;
                     }
                }
             }
         }

         if(set === false){
             if(prevKey !== 0){
                relativepoints.currentKey = 0;
             }
         }
     }

    onCurrentIndexChanged: {
        fillData();
        //console.log("currentIndex: " + currentIndex);
    }
    onCurrentKeyChanged: {
      //console.log("currentKey: " + currentKey);
    }

    function fillData() {
        //console.log("current_index!!!!: " + currentIndex);
        if(currentIndex == -1){
            key.text = ""
            azimuth.text = ""
            distance.text = ""
            altitude.text = ""
        }else{
            if(myModel.count > currentIndex){
               var currentItem = myModel.get(currentIndex);
               var currentItemKey = currentItem.key;
               if(currentItemKey !== currentKey &&
                       currentKey !== 0){
                   setCurrents(0, -1)
                   displayAddInfo();

               }else{
                    key.text = currentItem.key
                    azimuth.text = currentItem.azimuth
                    distance.text = currentItem.distance
                    altitude.text = currentItem.altitude
               }
            }else{
                setCurrents(0, -1)
                displayAddInfo();
            }
        }
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
                //console.log("Удаляем элемент из myModel: " + currentItem.key);
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

                    if (p1.isValid && p2.isValid) {

                        var azimuth = p1.azimuthTo(p2).toFixed() + "°";
                        var distance = Apx.formatDistance(p1.distanceTo(p2));
                        var altitude = Apx.formatDistance(f_altitude);

                        appendSorted(item.key,azimuth,distance,altitude);

                        /*myModel.append({
                            "key": item.key,
                            "azimuth": azimuth,
                            "distance": distance,
                            "altitude": altitude
                        });*/
                    }
                }
            }
        }
        fillData();
    }

    MaterialIcon {
        name: "bullseye"
        size: iconSize
        opacity: currentIndex != -1?1:0.5
        //opacity:myModel.count>0?1:0.5
        //color: myModel.count !== modelDataExternal.count?"yellow":"#fff"
    }

    Text {
        id: text
        Layout.fillHeight: true
        Layout.fillWidth: true
        font.family: font_condenced
        color: "#fff"
        text:  qsTr("Off")
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        visible: currentIndex == -1
        /*ToolTipArea {
            text: qsTr("Key")
        }*/
    }

    Text {
        id: key
        Layout.fillHeight: true
        Layout.fillWidth: true
        font.family: font_condenced
        color: "#fff"
        text:  ""
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        visible: text !==""
        ToolTipArea {
            text: qsTr("Key")
        }
    }

    Text {
        id: azimuth
        Layout.fillHeight: true
        Layout.fillWidth: true
        font.family: font_condenced
        color: "#fff"
        text:  ""
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        visible: text !=="" && coord.isValid;
        ToolTipArea {
            text: qsTr("Azimuth")
        }
    }

    Text {
        id: distance
        Layout.fillHeight: true
        Layout.fillWidth: true
        font.family: font_condenced
        color: "#fff"
        text:  ""
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        visible: text !=="" && coord.isValid;
        ToolTipArea {
            text: qsTr("Distance")
        }
    }

    Text {
        id: altitude
        Layout.fillHeight: true
        Layout.fillWidth: true
        font.family: font_condenced
        color: "#fff"
        text:  ""
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        visible: text !=="" && coord.isValid;
        ToolTipArea {
            text: qsTr("Altitude")
        }
    }

    Text {
        id: blank
        width: iconSize
    }


}


