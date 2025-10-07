import QtQuick 2.5

MouseArea {
    property alias tip: tip
    property alias text: tip.text
    property alias source: tip.source
    property alias hideDelay: hideTimer.interval
    property alias showDelay: showTimer.interval
    id: mouseArea
   // acceptedButtons: Qt.NoButton
    anchors.fill: parent
    hoverEnabled: true
    propagateComposedEvents: true
    //x: parent.x
   // y: parent.y
    Timer {
        id:showTimer
        interval: 500
        running: mouseArea.containsMouse && !tip.visible
        onTriggered: tip.show();
    }
    Timer {
        id:hideTimer
        interval: 100
        running: !mouseArea.containsMouse && tip.visible
        onTriggered: tip.hide();
    }
    SnapshotToolTip{
        id:tip
    }
}
