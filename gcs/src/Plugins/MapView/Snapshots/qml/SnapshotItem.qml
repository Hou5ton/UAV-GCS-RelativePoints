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

import FileValidator 1.0

MapObject {

    id: control
    property var fact: modelData
    property string name: "monitor-screenshot"
    property int size: 16
    readonly property string time: fact?fact.date + " " + fact.time:""
    readonly property string full_title: fact?fact.vehicle + ":" + fact.title:""

    //readonly property string image_path: fact?snapshots.image_path + fact.filename + ".png":"";
    readonly property string image_path_preview: fact?snapshots.image_path_preview + fact.filename + ".png":"";
    readonly property string image_path_default:  "../icons/gcu.svg"

    implicitZ: -100
    implicitCoordinate: fact?QtPositioning.coordinate(fact.lat,fact.lon):QtPositioning.coordinate()
    title:  fact?fact.title:""

    FileValidator{
        id: validator
        //url: image_path
        url: image_path_preview
        treatAsImage: true;
    }

    textColor: "black"
    color: {
        color = colorDefault
        if(fact && fact.active){
            color = colorSelected
        }else if(fact && (fact.lat !== fact.lat_scr ||
                fact.lon !== fact.lon_scr)){
            color = colorMoved
        }
    }

    hoverScaleFactor: 1
    opacity: ui.effects?((hover||selected)?1:0.7):1

    font.family: "Material Design Icons"
    font.pixelSize: control.size
    text: fact?application.materialIconChar(name) + fact.key:""

    onTriggered: {
        snapshots.edit.trigger() //({"pos":Qt.point(0,ui.window.height)})
    }

    contentsTop: [
        Loader {
            active: fact?fact.date!==""&& fact.time!=="" && (dragging||hover):false
            asynchronous: true
            sourceComponent: Component {
                MapText {
                    textColor: control.textColor
                    color: control.color
                    text: time
                    font.pixelSize: map.fontSize
                    font.family: font_mono
                    font.bold: false
                }
            }
        }
    ]
    contentsRight: [
        Loader {
            active: fact?fact.vehicle!==""&& fact.title!=="" && (dragging||hover):false
            asynchronous: true
            sourceComponent: Component {
                MapText {
                    textColor: control.textColor
                    color: control.color
                    text:{
                        if(fact){
                           text = time!==fact.title?full_title:fact.vehicle
                        }else{
                            text = "";
                        }
                    }
                    font.pixelSize: map.fontSize
                    font.family: font_mono
                    font.bold: false
                }
            }
        },
        Loader {
            active: fact?fact.descr!==""&& (dragging||hover):false
            asynchronous: true
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
        }

    ]

    contentsCenterImage: [
        Loader {
            active:fact?fact.filename!==""&& (dragging||hover):false
            asynchronous: true
            sourceComponent: Component {
                Rectangle {
                    id: blueRectangle
                    width: 150
                    height: 150

                    RowLayout{
                        id:control
                        Layout.minimumWidth: 150;
                        Layout.maximumWidth: 150;
                        Layout.preferredWidth: 150;
                        height: parent.height
                        Image {
                            //source: validator.fileValid ? image_path : image_path_default
                            source: validator.fileValid ? image_path_preview : image_path_default
                            Layout.preferredWidth: 150;
                            Layout.preferredHeight: 150;
                        }
                    }
                }
            }
        }
    ]
    //dragging support
    onMovingFinished: {
        if(selected && fact){
            var d=fact
            d.lat=coordinate.latitude
            d.lon=coordinate.longitude
            snapshots.moveSnapshot(d);

            if(d.active){
                control.color = colorSelected
            }else if(d.lat !== d.lat_scr ||
                    d.lon !== d.lon_scr){
                control.color = colorMoved
            }else{
                control.color = colorDefault
            }
        }
    }
    draggable: selected

    onSelectedChanged: {
        if(selected && fact){
            snapshots.createEditor(fact)
        }else{
            snapshots.destroyEditor(fact)
        }

        if(fact){
            if(fact.active){
                control.color = colorSelected
            }else if(fact.lat !== fact.lat_scr ||
                    fact.lon !== fact.lon_scr){
                control.color = colorMoved
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
        target: snapshots
        onMoveMap:{
            if(fact &&
                    key === fact.key){
                move_to_coo()
            }
        }
    }

    /*Timer {
        id: mpTimer
        interval: 500
        repeat: false
        onTriggered: {
            deselect()
            selectAndCenter()
        }
    }*/
}
