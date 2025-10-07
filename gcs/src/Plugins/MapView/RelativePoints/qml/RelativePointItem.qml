import QtQuick 2.5
import QtLocation 5.9
import QtPositioning 5.6
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.12
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

import Apx.Map 1.0
import Apx.Map.Common 1.0
import Apx.Common 1.0
import APX.Vehicles 1.0 as APX
MapObject {

    id: control

    property APX.Vehicle vehicle: apx.vehicles.current
    property var fact: modelData
    //property string name: "shield-home"
    property string name: "bullseye"
    //property string name: fact &&fact.disp_on_map?"map-marker-question":"map-marker-question-outline"
    property int size: 16
    readonly property string full_title: fact?fact.title:""
    property int size_info: map.fontSize - 2

    property real lat: m.gps_lat.value
    property real lon: m.gps_lon.value
    property real hmsl: m.gps_hmsl.value

    property real f_altitude: m.altitude.value
    property var coord: QtPositioning.coordinate(vehicle.coordinate.latitude,
                                                 vehicle.coordinate.longitude,
                                                 f_altitude)



    property var p1: QtPositioning.coordinate(fact.lat,fact.lon,fact.alt)
    //property var p2: QtPositioning.coordinate(lat,lon)
    property var p2: coord
    property var azimuth:  p1.azimuthTo(p2)
    property var distance:  p1.distanceTo(p2)
    //property var altitude:  hmsl - fact.alt;
     property var altitude:  f_altitude /*- fact.alt*/;

    property var currentKey:  text_azimuth.active;

    implicitZ: -100
    implicitCoordinate: fact?QtPositioning.coordinate(fact.lat,fact.lon):QtPositioning.coordinate()
    title:  fact?fact.title:""
    visible: fact &&fact.disp_on_map?true:false

    textColor: "black"
    color: {
        color = colorDefault
        if(fact && fact.active){
            color = colorSelected
        }
    }

    hoverScaleFactor: 1
    opacity: ui.effects?((hover||selected||currentKey)?1:0.7):1

    font.family: "Material Design Icons"
    font.pixelSize: control.size
    //text: fact?application.materialIconChar(name) + fact.title:""
    text: fact?application.materialIconChar(name) + fact.key:""

    onTriggered: {
        relativepoints.edit.trigger() //({"pos":Qt.point(0,ui.window.height)})
    }

    contentsCenter: [
        Loader {
            id: text_title
            active: fact?(fact.title!=="" || fact.descr!=="") && (dragging||hover):false
            asynchronous: true
            anchors.centerIn: parent
            anchors.verticalCenterOffset: /*parent.height +*/ text_title.height
            sourceComponent: Component {
                MapText {
                    textColor: control.textColor
                    color: control.color
                    text:{
                        if(fact){
                            if(full_title != ""){
                                text = full_title
                                if(fact.descr !== ""){
                                    text += "\n"
                                    text += fact.descr
                                }
                            }else{
                                if(fact.descr !== ""){
                                    text = fact.descr
                                }else{
                                  text = "";
                                }
                            }

                        }else{
                            text = "";
                        }
                    }
                    font.pixelSize: map.fontSize
                    font.family: font_mono
                    font.bold: false
                }
            }
        }/*,
        Loader {
            id: text_descr
            active: fact?fact.descr!==""&& (dragging||hover):false
            asynchronous: true
            anchors.centerIn: parent
            anchors.verticalCenterOffset:text_title.height + text_descr.height
            sourceComponent: Component {
                MapText {
                    textColor: control.textColor
                    color: control.color
                    text: fact?fact.descr:""
                    font.pixelSize: map.fontSize
                    font.family: font_mono
                    font.bold: false
                }
            }
        }*/
    ]

    contentsTopRP: [
        Loader {
            id: text_azimuth
            active: {
                if(control.lat && control.lon){
                    active = true
                }else{
                    active = false
                }
            }
            asynchronous: true
            sourceComponent: Component {
                MapText {
                    textColor: control.textColor
                    color: control.color
                    text:   qsTr("Az") + ":" + control.azimuth.toFixed()  + "°"  +
                            "\n" +
                            qsTr("Dist") + ":" + Apx.formatDistance(control.distance)+
                             "\n" +
                            qsTr("Alt") + ":" + Apx.formatDistance(control.altitude)
                    font.pixelSize: size_info
                    font.family: font_mono
                    font.bold: false
                }
            }
        }/*,
        Loader {
            id: text_distance
            active: {
                if(control.lat && control.lon){
                    active = true
                }else{
                    active = false
                }
            }
            asynchronous: true
            sourceComponent: Component {
                MapText {
                    textColor: control.textColor
                    color: control.color
                    text:  Apx.formatDistance(control.distance)
                    font.pixelSize: map.fontSize
                    font.family: font_mono
                    font.bold: false
                }
            }
        },
        Loader {
            id: text_altitude
            active: {
                if(control.hmsl){
                    active = true
                }else{
                    active = false
                }
            }
            asynchronous: true
            sourceComponent: Component {
                MapText {
                    textColor: control.textColor
                    color: control.color
                    text:  Apx.formatDistance(control.hmsl)
                    font.pixelSize: map.fontSize
                    font.family: font_mono
                    font.bold: false
                }
            }
        }*/

    ]
    //dragging support
    onMovingFinished: {
        if(selected && fact){
            var d=fact
            d.lat=coordinate.latitude
            d.lon=coordinate.longitude
            relativepoints.editRelativePoint(d);
        }
    }
    draggable: selected

    onSelectedChanged: {
        if(selected && fact){
            relativepoints.createEditor(fact)
        }else{
            relativepoints.destroyEditor(fact)
        }

        if(fact){
            if(fact.active){
                control.color = colorSelected
            }else{
                control.color = colorDefault
            }
        }else{
            control.color = colorDefault
        }
    }

    function moveMap(key){
       var value = key;
    }

    Connections {
        target: relativepoints
        onMoveMap:{
            if(fact &&
                    key === fact.key){
                move_to_coo()
            }
        }
    }

   /* Timer {
        id: mpTimer
        interval: 500
        repeat: true
        running: true
        onTriggered: {

            if(control.lat && control.lon &&
                    fact.key === relativepoints.currentKey){
                if(!text_azimuth.active){
                    text_azimuth.active = true
                }
            }else{
                if(text_azimuth.active){
                    text_azimuth.active = false
                }
            }

        }
    }*/

    Timer {
        id: mpTimer
        interval: 500
        repeat: true
        running: true
        onTriggered: {

            if(control.lat && control.lon &&
                    (fact.key === relativepoints.currentKey || hover)){
                if(!text_azimuth.active){
                    text_azimuth.active = true
                }
            }else{
                if(text_azimuth.active){
                    text_azimuth.active = false
                }
            }
           // text_azimuth.anchors.horizontalCenterOffset =  parent.width * 2

        }
    }
}
