import QtQuick          2.12
import QtQuick.Layouts  1.12
import QtQuick.Controls 2.12

import Apx.Common 1.0

RowLayout {

    readonly property int iconSize: text.implicitHeight*0.8
    property var rulerPlugin: apx.tools.ruler

    MaterialIcon {
        name: "ruler"
        size: iconSize
        opacity:rulerPlugin.used?1:0.5
    }

    Text {
        id: text
        Layout.fillHeight: true
        Layout.fillWidth: true
        font.family: font_condenced
        color: "#fff"
        text: rulerPlugin.totalDistance
        visible: (rulerPlugin.totalDistance !== "" && rulerPlugin.used)
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        ToolTipArea {
            text: qsTr("Distance between points")
        }
    }
    Text {
        id: blank
        width: iconSize
    }

    function nextItem() {
        if(rulerPlugin.used){
            rulerPlugin.used = false;
        }else{
            rulerPlugin.used = true;
        }
        //console.log("rulerPlugin.used: " + rulerPlugin.used);
    }


}
