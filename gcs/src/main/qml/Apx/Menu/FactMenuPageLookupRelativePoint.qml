import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.3
import QtQml 2.12

import APX.Facts 1.0
import Apx.Common 1.0
import Apx.Map.Common 1.0

FactMenuPageListRelativePoint {
    id: control

    property bool filterEnabled: (fact)?true:false
    property var relativepoints: apx.tools.relativepoints
    property var parentFact: fact
    property string sort_order: fact.sort_order
    property int quantity: fact.quantity
    property int quantity_on_map: fact.quantity_on_map
    //property var listwidth: listView.width

    model: fact.dbModel
    delegate: Loader{
        id:loader
        asynchronous: true
        active: true
        visible: active
       // width: listView.width
       // implicitWidth: layout.implicitWidth
        height:active?MenuStyle.itemSize:0
        sourceComponent: Component {
            RowLayout{
                id: layout
                //anchors.fill: parent
                spacing: 2

                FactButton {
                    id: select_button
                    height: MenuStyle.itemSize
                    property var d: modelData
                    width: MenuStyle.itemSize
                    enabled: true
                    iconName: {
                        d.active?"select-all":"select-off"
                    }
                    onTriggered: {
                        parentFact.clickItem(modelData,!d.active);
                    }

                }

                FactButton {
                    id: disp_on_map_button
                    height: MenuStyle.itemSize
                    property var d: modelData
                    width: MenuStyle.itemSize
                    enabled: true
                    iconName: "bullseye"
                    checkable : true
                    checked: d.disp_on_map
                    onTriggered: {
                        if(d.disp_on_map){
                            d.disp_on_map = 0
                        }else{
                            d.disp_on_map = 1
                        }
                        relativepoints.editRelativePoint(d)
                    }
                }

                FactButton {
                    id: model_button
                    height: MenuStyle.itemSize * 10
                    width: listView.width  - select_button.width - disp_on_map_button.width - layout.spacing
                    property var d: modelData
                    title: d.title?
                               qsTr("#") + String(d.key) + " " + d.title:
                               qsTr("#") + String(d.key)
                    descr: d.descr?
                               d.descr:
                               ""

                    toolTip: ""
                    status: d.status?apx.distanceToString(d.status):""

                    //active: d.active?d.active:false
                    showEditor: false
                    //modified: d.active?d.active:false
                    /*onTriggered: {
                        snapshots.createEditor(d)
                    }*/
                    RelativePointToolTipArea{
                        id: rp_tool_area
                        property var d: modelData
                        tip {
                            text:{
                                model_button.title + "\n" +
                                       qsTr("Latitude") + ": " + apx.latToString(d.lat) + "\n" +
                                       qsTr("Longitude") + ": " +apx.lonToString(d.lon) /*+ "\n" +
                                       qsTr("Altitude") + ": " +d.alt*/
                            }
                            x:  select_button.width - disp_on_map_button.width - layout.spacing
                            y: -rp_tool_area.height /** 3*/
                        }
                    }

                    Timer {
                        id: updateTimer
                        interval: 100
                        running: true
                        repeat: true
                        onTriggered: {
                            if(model_button.width != (listView.width  - select_button.width - disp_on_map_button.width - spacing)){
                                model_button.width = (listView.width  - select_button.width - disp_on_map_button.width - spacing)
                            }
                        }
                    }
                   /* function getDescription()
                    {
                        var descr_ =  relativepoints.getFilterDescription(modelData)
                        return descr_
                    }*/
                }
            }
        }
        /*onLoaded: {
            model_button.width = listView.width - locate_button.width - select_button.width - layout.spacing - layout.spacing
        }*/
       /* FactButton {
            //checkable: true
            height: MenuStyle.itemSize
           // width: listwidth - locate_button.width - select_button.width - spacing - spacing
            width: listView.width;
            property var d: modelData
            title:{
                d.descr?qsTr("#") + String(d.key) + " " + d.descr:qsTr("#") + String(d.key)
            }
            descr: d.title
            toolTip: d.active? title + "\n" +
                               qsTr("Vehicle") + ": " + d.vehicle + "\n" +
                               qsTr("File name") + ": " + d.filename + "\n" +
                               qsTr("Latitude") + ": " + d.lat_scr + "\n" +
                               qsTr("Longitude") + ": " +d.lon_scr + "\n" +
                               qsTr("Altitude") + ": " +d.alt
                               : title
            status: d.status?d.status:""
            active: d.active?d.active:false
            showEditor: false
            onTriggered: {
               // parentFact.triggerItem(modelData)
               parentFact.clickItem(modelData,!d.active);
            }
        }*/
    }

    //filter
    header:ColumnLayout{
        id: layout_header
        //anchors.fill: parent
        spacing: 2

        TextField {
            id: filterText
            z: 100
            enabled: filterEnabled
            visible: enabled
            width:listView.width
            height: filterEnabled?implicitHeight:0
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: MenuStyle.titleFontSize
            placeholderText: qsTr("Search")+"..."
            background: Rectangle{
                border.width: 0
                color: Material.background
            }
            text: filterEnabled?fact.filter:""
            onTextEdited:{
                fact.filter=text
            }
            Timer {
                id: updateTimerText
                interval: 100
                running: true
                repeat: true
                onTriggered: {
                    if(filterText.width != listView.width){
                        filterText.width = listView.width
                    }
                }
            }

            Connections {
                //prefent focus change on model updates
                target: listView
                enabled: control.filterEnabled
                onCountChanged: filterText.forceActiveFocus()
            }
        }
    }
    Timer {
        //initial focus
        interval: 500
        running: true
        onTriggered: headerItem.forceActiveFocus()
    }
    Component.onCompleted: {
        fact.filter=""
        headerItem.forceActiveFocus()
    }
}
