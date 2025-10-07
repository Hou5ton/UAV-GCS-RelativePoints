import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.3

//import Apx.Common 1.0
//import APX.Facts 1.0

Rectangle {
    id:tooltip
    property alias text: textContainer.text
    property alias source: imageContainer.source
    property int verticalPadding: 1
    property int horizontalPadding: 5
    width: textContainer.width + imageContainer.width + horizontalPadding * 2
    height: textContainer.height  + verticalPadding * 2
    color: Material.backgroundColor
    //color: "#aa999999"

    RowLayout{
        id:control
        spacing: 2
        //Layout.minimumWidth: 150;
       // Layout.maximumWidth: 150;
       // Layout.preferredWidth: 150;
      //  height: parent.height
        Image {
            id:imageContainer
            source: "image_path_preview"
            Layout.preferredWidth: textContainer.height;//textContainer.width;
            Layout.preferredHeight: textContainer.height;
        }
        Text {
            //anchors.centerIn: parent
            id:textContainer
            text: "text"
            font.pixelSize: 14
            //font.family: font_narrow
            font.family:font_condenced
            color: Material.primaryTextColor
        }
    }

    NumberAnimation {
        id: fadein
        target: tooltip
        property: "opacity"
        easing.type: Easing.InOutQuad
        duration: 300
        from: 0
        to: 1
    }
    NumberAnimation {
        id: fadeout
        target: tooltip
        property: "opacity"
        easing.type: Easing.InOutQuad
        from: 1
        to: 0
        onStopped: visible = false;
    }
    visible:false
    onVisibleChanged: if(visible)fadein.start();
    function show(){
        visible = true;
        fadein.start();
    }
    function hide(){
        fadeout.start();
    }
}
