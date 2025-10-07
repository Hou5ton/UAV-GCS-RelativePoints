import QtQuick 2.2
import "../common"
import "."

Item {
    id: hdg_window
    //instrument item
    property double animation_duration: 500
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.top: parent.top
    anchors.leftMargin: 2
    anchors.rightMargin: 2
    height: width*0.09
    property double bottomHeight: height*0.4
    clip: true
    property double value: apx.angle(m.yaw.value)
    Behavior on value { enabled: ui.smooth; RotationAnimation {duration: animation_duration; direction: RotationAnimation.Shortest; } }

    /*Rectangle {
        color: "#40000000"
        border.width: 0
        anchors.fill: parent
    }*/

    property double num2scaleWidth: svgRenderer.elementBounds(pfdImageUrl, "hdg-scale").width * strip_scale /90
    property double strip_scale: width/svgRenderer.elementBounds(pfdImageUrl, "hdg-scale").width
    property double valueShiftMax: width/num2scaleWidth/2

    Item {
        id: scale_top
        anchors.fill: parent
        anchors.bottomMargin: parent.height*0.2
        clip: true

        //horizontal sacle with numbers
        Item {
            id: hdg_scale
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.horizontalCenterOffset: -apx.angle(value)*num2scaleWidth
            width: parent.width*4*2
            height: parent.height

            //scale
            Repeater {
                model: 4*2
                PfdImage {
                    //smooth: ui.antialiasing
                    elementName: "hdg-scale"
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.leftMargin: index*width-tickWidth*strip_scale/2
                    width: elementBounds.width*strip_scale
                    height: elementBounds.height*strip_scale
                    property double tickWidth: 2
                }
            }
            //scale numbers
            Repeater {
                model: 36*2
                Text {
                    property int pos: (index-18*2)*10
                    property int num: apx.angle360(pos).toFixed()
                    //smooth: ui.antialiasing
                    text: num===0?qsTr("N"):
                          num===90?qsTr("E"):
                          num===180?qsTr("S"):
                          num===270?qsTr("W"):
                          ("00"+apx.angle360(pos).toFixed()).slice(-3)
                    //render as image
                    style: Text.Raised
                    styleColor: "transparent"
                    //color: "#A0FFFFFF"
                    color:(num===0||num===90||num===180||num===270)?"#B0FFFFFF":
                          "#80FFFFFF"
                    //color: "white"
                    font.family: font_condenced
                    font.bold: true
                    font.pixelSize: parent.height*0.6
                    //anchors.fill: parent
                    anchors.top: parent.top
                    //anchors.right: parent.right
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.horizontalCenterOffset: pos*num2scaleWidth
                    //anchors.topMargin: (index-2)*speed_window.num2scaleHeight
                }
            }
        }
        //gps course bug triangle
        PfdImage {
            id: hdg_crs_bug
            elementName: "hdg-crs-bug"
            property double value: apx.angle(m.course.value-m.yaw.value)
            Behavior on value { enabled: ui.smooth; RotationAnimation {duration: animation_duration; direction: RotationAnimation.Shortest; } }
            //smooth: ui.antialiasing
            border: 1
            fillMode: Image.PreserveAspectFit
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.horizontalCenterOffset: apx.limit(apx.angle(value),-valueShiftMax,valueShiftMax)*num2scaleWidth
            height: bottomHeight
            width: elementBounds.width*height/elementBounds.height
            ToolTipArea { text: m.course.descr }
        }
        //cmd course bug arrow
        PfdImage {
            id: hdg_cmd_bug
            elementName: "hdg-cmd-bug"
            property double value: apx.angle(m.cmd_course.value-m.yaw.value)
            Behavior on value { enabled: ui.smooth; RotationAnimation {duration: animation_duration; direction: RotationAnimation.Shortest; } }
            //smooth: ui.antialiasing
            border: 1
            fillMode: Image.PreserveAspectFit
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.horizontalCenterOffset: apx.limit(apx.angle(value),-valueShiftMax,valueShiftMax)*num2scaleWidth
            height: bottomHeight
            width: elementBounds.width*height/elementBounds.height
            ToolTipArea { text: m.cmd_course.descr }
        }
        //rw hdg bug arrow
        PfdImage {
            id: hdg_rw_bug
            visible:
                m.mode.value===mode_LANDING ||
                m.mode.value===mode_TAKEOFF ||
                m.mode.value===mode_TAXI ||
                (m.mode.value===mode_WPT && m.mtype.value===mtype_line)
            elementName: "hdg-rw-bug"
            property double value: apx.angle(m.tgHDG.value-m.yaw.value)
            Behavior on value { enabled: ui.smooth; RotationAnimation {duration: animation_duration; direction: RotationAnimation.Shortest; } }
            //smooth: ui.antialiasing
            border: 1
            fillMode: Image.PreserveAspectFit
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.horizontalCenterOffset: apx.limit(apx.angle(value),-valueShiftMax,valueShiftMax)*num2scaleWidth
            height: bottomHeight
            width: elementBounds.width*height/elementBounds.height
            ToolTipArea { text: m.tgHDG.descr }
        }
        //center number box
        PfdImage {
            id: hdg_box
            elementName: "hdg-box"
            //smooth: ui.antialiasing
            fillMode: Image.PreserveAspectFit
            anchors.topMargin: 1
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            height: parent.height
            width: elementBounds.width*height/elementBounds.height
            Rectangle {
                border.width: 0
                color: "#C0FF0000"
                anchors.fill: hdg_text
                anchors.leftMargin: parent.width*0.05
                anchors.rightMargin: anchors.leftMargin
                anchors.topMargin: anchors.leftMargin+1
                anchors.bottomMargin: parent.height*0.4
                visible: m.cmode_nomag.value
            }
            Text {
                id: hdg_text
                anchors.fill: parent
                anchors.topMargin: -1
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignTop
                property int v: value
                text: ("00"+apx.angle360(v).toFixed()).slice(-3)
                font.pixelSize: parent.height*0.75
                font.family: font_mono
                font.bold: true
                color: m.cmode_nomag.value?"yellow":"white"

            }
            ToolTipArea { text: m.yaw.descr }
        }
    }

    //turn rate bar
    PfdImage {
        id: hdg_turnrate
        elementName: "hdg-turnrate"
        //smooth: ui.antialiasing
        fillMode: Image.PreserveAspectFit
        anchors.fill: parent
        anchors.topMargin: scale_top.height
        property double maxW: 0.95*elementBounds.width*height/elementBounds.height/2
        property double valueW: apx.limit(turn_calc.derivative*num2scaleWidth,-hdg_turnrate.maxW,hdg_turnrate.maxW)
        Behavior on valueW { enabled: ui.smooth; PropertyAnimation {duration: 500; } }
        //derivative
        Item {
            id: turn_calc
            visible: false
            property double value: m.yaw.value
            property double derivative: 0
            property double time_s: 0
            property double value_s: 0
            onValueChanged: {
                var t=new Date().getTime();
                var dt=(t-time_s)/1000;
                time_s=t;
                var dv=apx.angle(value-value_s);
                value_s=value;
                if(dt>0.5 || dt<0.01)derivative=0;
                else if(Math.abs(dv>50))derivative=0;
                else derivative=derivative*0.5+(dv/dt)*0.5;
            }
        }
        Rectangle {
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.horizontalCenter
            anchors.topMargin: parent.height*0.15
            anchors.bottomMargin: anchors.topMargin
            border.width: 0
            antialiasing: true
            border.color: "white"
            color: "white" //"#C0FF00FF"
            visible: hdg_turnrate.valueW>0
            width: hdg_turnrate.valueW
        }
        Rectangle {
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.horizontalCenter
            anchors.topMargin: parent.height*0.15
            anchors.bottomMargin: anchors.topMargin
            border.width: 0
            antialiasing: true
            border.color: "white"
            color: "white" //"#C0FF00FF"
            visible: hdg_turnrate.valueW<0
            width: -hdg_turnrate.valueW
        }
        //steering yaw control
        property double valueR: apx.limit(m.ctr_rudder.value*hdg_turnrate.maxW,-hdg_turnrate.maxW,hdg_turnrate.maxW)
        Behavior on valueR { enabled: ui.smooth; PropertyAnimation {duration: 100; } }
        Rectangle {
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.horizontalCenter
            anchors.topMargin: parent.height*0.25
            anchors.bottomMargin: anchors.topMargin
            border.width: 0
            antialiasing: true
            color: "#80000000"
            visible: hdg_turnrate.valueR>0
            width: hdg_turnrate.valueR
        }
        Rectangle {
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.horizontalCenter
            anchors.topMargin: parent.height*0.25
            anchors.bottomMargin: anchors.topMargin
            border.width: 0
            antialiasing: true
            color: "#80000000"
            visible: hdg_turnrate.valueR<0
            width: -hdg_turnrate.valueR
        }

    }

    property bool isShiftControl:
        m.mode.value===mode_LANDING ||
        m.mode.value===mode_TAKEOFF ||
        m.mode.value===mode_TAXI ||
        (m.mode.value===mode_WPT && m.mtype.value===mtype_line) ||
        m.mode.value===mode_STBY

    MouseArea {
        anchors.fill: parent
        anchors.rightMargin: parent.width/2
        cursorShape: Qt.PointingHandCursor
        onClicked: {
            if(isShiftControl) m.rwAdj.setValue(m.rwAdj.value-1)
            else m.cmd_course.setValue(apx.angle(m.cmd_course.value-15))
        }
    }
    MouseArea {
        anchors.fill: parent
        anchors.leftMargin: parent.width/2
        cursorShape: Qt.PointingHandCursor
        onClicked: {
            if(isShiftControl) m.rwAdj.setValue(m.rwAdj.value+1)
            else m.cmd_course.setValue(apx.angle(m.cmd_course.value+15))
        }
    }
}
