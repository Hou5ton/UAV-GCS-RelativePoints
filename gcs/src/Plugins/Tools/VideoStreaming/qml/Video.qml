import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtMultimedia 5.13
import QtQuick.Controls.Material 2.12

import GstPlayer 1.0
import Apx.Common 1.0

// sample stream:
// rtsp://wowzaec2demo.streamlock.net/vod/mp4:BigBuckBunny_115k.mov

Control {
    id: control

    focus: true

    implicitWidth: 400
    implicitHeight: implicitWidth*3/4

    Component.onCompleted: {
        application.registerUiComponent(control, "video")
    }

    readonly property var plugin: apx.tools.videostreaming
    readonly property var snapshots: apx.tools.snapshots
    readonly property var altmatrix: apx.tools.altitudematrix
    readonly property var poicontrol: apx.tools.poicontrol
    readonly property var overlay: plugin?plugin.tune.overlay:undefined
    readonly property bool running: plugin?plugin.tune.running.value:false
    readonly property bool recording: plugin?plugin.tune.record.value:false
    readonly property var scale: overlay?overlay.scale:0.1
    readonly property var look: apx.tools.apx_coordinates?apx.tools.apx_coordinates.look:undefined
    readonly property int viewMode: plugin?plugin.tune.view_mode.value:false
    readonly property bool viewFull: viewMode>1

    readonly property bool alive: plugin.connectionState === GstPlayer.STATE_CONNECTED
    property var file_name: apx.tools.videostreaming.file_name
    property var file: apx.tools.videostreaming.file
    property var file_prev: apx.tools.videostreaming.file_prev
    readonly property real  screenshot_lat: apx.tools.videostreaming.screenshot_lat;
    readonly property real  screenshot_lon: apx.tools.videostreaming.screenshot_lon;
    readonly property real  screenshot_alt: apx.tools.videostreaming.screenshot_alt;

    function createEditorFromVideo(){
        snapshots.createEditorFromVideo(file_name,
                                        file,
                                        file_prev,
                                        apx.vehicles.current.name,
                                        screenshot_lat,
                                        screenshot_lon,
                                        screenshot_alt)
    }

    Connections {
        target: gstplayer
        onFinishedScreenshotSaving: {
            createEditorFromVideo()
        }
    }

    background: Rectangle {
        border.width: 0
        color: "#000"
        VideoOutput {
            id: videoOutput
            anchors.fill: parent
            anchors.leftMargin: viewFull?0:control.leftPadding
            anchors.rightMargin: viewFull?0:control.rightPadding
            anchors.topMargin: viewFull?0:control.topPadding
            anchors.bottomMargin: viewFull?0:control.bottomPadding
            source: plugin
            flushMode: VideoOutput.EmptyFrame
            fillMode: viewMode>0?VideoOutput.PreserveAspectCrop:VideoOutput.PreserveAspectFit
            Overlay {
                id: overlay
                anchors.fill: parent
                anchors.topMargin: viewFull?control.topPadding:0
                showNumbers: !pluginMinimized
                showAim: !pluginMinimized
                interactive: true
                frameRect: videoOutput.contentRect
                alive: control.alive
            }
        }
        Loader {
            anchors.centerIn: parent
            active: plugin.connectionState === GstPlayer.STATE_CONNECTING
            sourceComponent: BusyIndicator { }
        }
    }

    contentItem: Item {
        ColumnLayout {
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.margins: 3
            spacing: 5
            CleanButton {
                visible: running
                enabled: look?true:false
                iconName: "eye"
                toolTip: qsTr("Look at this target/position")
                Layout.alignment: Qt.AlignRight
                onTriggered: look.trigger()
            } 
            CleanButton {
                visible: running
                iconName: "record-rec"
                toolTip: plugin.tune.record.descr
                iconColor: recording ? Material.color(Material.DeepOrange) : Material.primaryTextColor
                Layout.alignment: Qt.AlignRight
                onTriggered: plugin.tune.record.value =! plugin.tune.record.value
            }
            CleanButton {
                visible: running
                iconName: "image"
                toolTip: qsTr("Video stream screenshot")
                Layout.alignment: Qt.AlignRight
                onTriggered: plugin.snapshot()
            }
            CleanButton {
                toolTip: plugin.tune.running.descr
                iconName: running?"cast-off":"cast"
                Layout.alignment: Qt.AlignRight
                onTriggered: plugin.tune.running.value=!plugin.tune.running.value
            }

            Item {
                height: 16
                width: height
            }

            Row {
                Layout.alignment: Qt.AlignRight

               CleanButton {
                    toolTip: poicontrol.descr
                    iconName:poicontrol.icon
                    onTriggered: {
                        poicontrol.trigger()
                        poicontrol.createPOIFromVideo()
                    }
                }

              /* Loader {
                   Layout.fillWidth: true
                   active: true
                   visible: active
                   sourceComponent: OverlayGimbalAxis {
                       value: m.cam_hmsl.value
                       type: OverlayGimbalAxis.AxisType.Down
                   }
               }*/


               /*FactValue {
                   title: "POI"
                   property real lat: m.cam_lat.value
                   property real lon: m.cam_lon.value
                   property real hmsl: m.cam_hmsl.value
                   visible: viewFull && lat!=0 && lon!=0// && (tposTimeout.running||active)
                   value: apx.latToString(lat)+" "+apx.lonToString(lon)+(hmsl!=0?" "+apx.distanceToString(hmsl, false) + "m":"")
               }*/


                Item {
                    height: 16
                    width: height
                }

                CleanButton {
                    toolTip: plugin.tune.sources.descr
                    iconName: plugin.tune.sources.icon
                    onTriggered: 
                        plugin.tune.sources.trigger()
                }

                Item {
                    height: 16
                    width: height
                }
                    
                CleanButton {
                    toolTip: plugin.tune.descr
                    iconName: "tune"
                    onTriggered: {
                        plugin.tune.trigger()
                    }
                }
            }
        }
    }
}
