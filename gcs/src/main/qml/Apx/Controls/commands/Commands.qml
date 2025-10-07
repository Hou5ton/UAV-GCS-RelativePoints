import QtQuick 2.11
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.2

import Apx.Common 1.0
import Apx.Menu 1.0


Rectangle {
    id: root
    border.width: 0
    color: "#000"
    implicitWidth: 400
    implicitHeight: 400

    onWidthChanged: implicitHeight=width
    onHeightChanged: implicitWidth=height

    readonly property int margins: 3

    property var vehicle: apx.vehicles.current

    //sizes
    readonly property int buttonHeight: width*0.1//,32)

    property int size: Math.max(50,width)*0.08

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: root.margins
        spacing: buttonHeight/4

        // placeholder
        Rectangle {
            Layout.fillHeight: true
            color: root.color
        }

        //menus
        Ctr {
            Layout.fillWidth: true
            Layout.preferredHeight: buttonHeight
        }

    }
}
