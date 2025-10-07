import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

import Apx.Common 1.0
import Apx.Controls 1.0

import APX.Facts 1.0

Item {
    id: control

    property bool interactive: false
    property bool alive: true

    property int numberItemSize: Math.min(22,Math.max(12,height/15))
    property int overlayItemSize: numberItemSize

    property int margins: Math.max(1,numberItemSize*0.1)

    Component.onCompleted: {
        if(!interactive)return
        for(var i in overlays){
            var overlay=overlays[i]
            var c=numbersMenuC.createObject(control,{"overlay": overlay})
            c.parentFact=plugin.tune.overlay
            plugin.overlayNumbersChanged.connect(overlay.model.loadSettings)
        }
    }
    Component {
        id: numbersMenuC
        NumbersMenu {
            property var overlay
            defaults: overlay.defaults
            settingsName: overlay.settingsName
            destroyOnClose: false
            onAccepted: plugin.overlayNumbersChanged()
        }
    }

    property var overlays: [overlay_left]

    /*NumbersBar {
        id: overlay_bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: control.margins
        showEditButton: false
        itemSize: numberItemSize
        settingsName: "video_bottom"
        defaults: [
            //{"bind": "altitude", "title": "ALT", "prec": "0"},
        ]
    }*/

    NumbersBox {
        id: overlay_left
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: control.margins
        margins: 0
        showEditButton: false
        itemSize: numberItemSize
        //model.minimumWidth: 500
        color: "#00000000"
        settingsName: "video"
        defaults: [
            {"bind": "gps_hmsl", "title": "MSL", "prec": "0"},
            {"bind": "altitude", "title": "ALT", "prec": "0"},
        ]
    }

    Connections {
        enabled: !interactive
        target: application
        onLoadingFinished: {
            //plugin is available
            for(var i in overlays){
                var overlay=overlays[i]
                plugin.overlayNumbersChanged.connect(overlay.model.loadSettings)
            }
        }
    }


    RowLayout {
        id: timeLayout
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.margins: control.margins
        spacing: overlayItemSize/2
        height: overlayItemSize
        FactValue {
            id: timeItem
            Layout.fillHeight: true
            fact: m.gps_time
            title: gps?qsTr("GPS"):qsTr("LOCAL")
            value: apx.dateToString(time)
            property bool gps: false
            property int time: gps?gpsTime:localTime

            property int gpsTime: fact.value
            property int localTime: 0
            Timer {
                running: !timeItem.gps
                interval: 500
                repeat: true
                onTriggered: {
                    var date = new Date;
                    timeItem.localTime=date.getTime()/1000
                }
            }
            onGpsTimeChanged: {
                gpsTimeout.restart()
                timeItem.gps=gpsTime>0
            }
            Timer {
                id: gpsTimeout
                interval: 5000
                onTriggered: timeItem.gps=false
            }
            enabled: interactive
            onTriggered: plugin.tune.tools.trigger()
        }

        //frame cnt
        FactValue {
            id: frameCntItem
            Layout.fillHeight: true
            showTitle: false
            readonly property int v: plugin.frameCnt
            value: ("0"+v).slice(-2)
            visible: alive && v>0
            onValueChanged: frameTimeout.restart()
            warning: visible && !frameTimeout.running
            Timer {
                id: frameTimeout
                interval: 1000
                repeat: false
            }
        }
    }

    //cam track pos
    FactValue {
        id: tposItem
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: timeLayout.bottom
        anchors.margins: control.margins
        height: overlayItemSize
        showTitle: false
        property real lat: m.cam_lat.value
        property real lon: m.cam_lon.value
        property real hmsl: m.cam_hmsl.value
        visible: lat!=0 && lon!=0 && (tposTimeout.running||active)
        value: {
            MapInterface.coordinatesString(lat, lon, apx.settings.interface.coordinate_system.value) +
                    (hmsl!=0?" "+apx.distanceToString(hmsl, false):"")
        }

        onLatChanged: tposTimeout.restart()
        onLonChanged: tposTimeout.restart()
        onHmslChanged: tposTimeout.restart()
        Timer {
            id: tposTimeout
            interval: 10000
            repeat: false
        }
        enabled: interactive
        onTriggered: active=!active
    }



    //cam mode
    FactValue {
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: control.margins
        height: overlayItemSize
        fact: m.cam_mode
        showTitle: false
        valueText: qsTranslate("MandalaVars", fact.text)
        enabled: interactive
        onTriggered: popupC.createObject(this)
        Component{
            id: popupC
            Popup {
                id: popup
                width: 150
                height: contentItem.implicitHeight
                topMargin: 6
                bottomMargin: 6
                padding: 0
                margins: 0
                x: parent.width

                Component.onCompleted: open()
                onClosed: destroy()

                contentItem: ListView {
                    id: listView
                    implicitHeight: contentHeight
                    implicitWidth: contentWidth
                    model: m.cam_mode.enumStrings
                    highlightMoveDuration: 0
                    delegate: ItemDelegate {
                        text: qsTranslate("MandalaVars", modelData)
                        width: Math.max(listView.width,implicitWidth)
                        highlighted: modelData === m.cam_mode.text
                        onClicked: {
                            popup.close()
                            m.cam_mode.value = modelData
                        }
                    }
                    ScrollIndicator.vertical: ScrollIndicator { }
                }
            }
        }
    }

    //bottom cam opts and values
    RowLayout {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        //anchors.right: parent.right
        anchors.margins: control.margins
        height: overlayItemSize
        spacing: 3

        FactValue {
            Layout.fillHeight: true
            fact: m.power_payload
            showTitle: false
            valueText: qsTr("PYLD")
            visible: interactive
            active: fact.value
            enabled: true
            onTriggered: fact.value = !fact.value
        }

        FactValue {
            Layout.fillHeight: true
            showTitle: false
            property var f: plugin.tune.controls
            value: qsTr("CTR")
            toolTip: f.descr
            visible: interactive
            enabled: true
            active: f.value
            onTriggered: f.value=!f.value

        }

        FactValue {
            Layout.fillHeight: true
            fact: m.cam_zoom
            title: qsTr("ZOOM")
            valueText: (fact.value*100).toFixed()
            visible: value>0
            enabled: interactive
            onTriggered: fact.value=0
        }
        FactValue {
            Layout.fillHeight: true
            fact: m.cam_focus
            title: qsTr("FOCUS")
            valueText: (fact.value*100).toFixed()
            visible: value>0
            enabled: interactive
            onTriggered: fact.value=0
        }
        FactValue {
            Layout.fillHeight: true
            fact: m.cam_ch
            title: qsTr("CH")
            visible: value>0
            onValueChanged: if(value>0)visible=true
            enabled: interactive
            onTriggered: fact.value=0
        }

        Repeater {
            model: [
                m.cam_opt_PF,
                m.cam_opt_NIR,
                m.cam_opt_DSP,
                m.cam_opt_FMI,
                m.cam_opt_FM,
                m.cam_opt_laser,
            ]
            delegate: FactValue {
                Layout.fillHeight: true
                fact: modelData
                showTitle: false
                value: fact.value
                valueText: fact.title.slice(fact.title.lastIndexOf("_")+1)
                visible: value
                onValueChanged: if(value)visible=true
                enabled: interactive
                onTriggered: fact.value=value?0:1
                active: value
            }
        }


    }

}
