import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.3
import QtQml 2.12

import APX.Facts 1.0
import Apx.Common 1.0
import Apx.Map.Common 1.0
import FileValidator 1.0

FactMenuPageListSnapshot {
    id: control

    property var snapshots: apx.tools.snapshots
    property var snapshot_filter: apx.tools.snapshots.snapshot_filter
    property var parentFact: fact
    property bool filterEnabled: (fact)?true:false
    property string sort_order: fact.sort_order
    property string sort_criteria: fact.sort_criteria
    property string filter_distance: fact.filter_distance
    property string filter_date_begin: fact.filter_date_begin
    property string filter_date_end: fact.filter_date_end
    property bool filter_date_enabled: fact.filter_date_enabled
    property string filter: fact.filter
    property int quantity: fact.quantity
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

               /* FactButton {
                    id: locate_button
                    height: MenuStyle.itemSize
                    property var d: modelData
                    width: MenuStyle.itemSize
                    enabled:{
                        d.status==="0m"?false:true
                    }
                    iconName: "map-marker-question-outline"

                    onTriggered: {
                        parentFact.triggerItem(modelData,!d.active)
                      // parentFact.clickItem(modelData,!d.active);
                    }
                }*/

                /*FactButton {
                    id: edit_button
                    height: MenuStyle.itemSize
                    property var snapshots: apx.tools.snapshots
                    property var d: modelData
                    width: MenuStyle.itemSize
                    enabled:{
                        true
                    }
                   // iconName: "file-document-edit"
                   // iconName: "database-edit"
                    iconName: "content-save-edit"


                    onTriggered: {
                        snapshots.createEditor(d);
                        snapshots.edit.trigger()
                      //  parentFact.triggerItem(modelData,!d.active)
                      // parentFact.clickItem(modelData,!d.active);
                    }
                }*/

                FactButton {
                    id: model_button
                    height: MenuStyle.itemSize * 10
                    width: listView.width  - select_button.width - layout.spacing
                    property var d: modelData
                    title:{
                        d.descr?qsTr("#") + String(d.key) + " " + d.descr:qsTr("#") + String(d.key)
                    }
                    descr: getDescription()

                    toolTip: ""
                    status: d.status?apx.distanceToString(d.status):""

                    //active: d.active?d.active:false
                    showEditor: false
                    //modified: d.active?d.active:false
                    /*onTriggered: {
                        snapshots.createEditor(d)
                    }*/
                    SnapshotToolTipArea{
                        id: snap_tool_area
                        property var d: modelData
                        //readonly property string image_path: snapshots.image_path + d.filename + ".png";
                        readonly property string image_path_preview: snapshots.image_path_preview + d.filename + ".png";
                        tip {
                            text:{
                                model_button.title + "\n" +
                                        qsTr("Vehicle") + ": " + d.vehicle + "\n" +
                                        qsTr("File name") + ": " + d.filename + "\n" +
                                        //qsTr("Latitude") + ": " + d.lat_scr + "\n" +
                                       // qsTr("Longitude") + ": " +d.lon_scr + "\n" +
                                       qsTr("Latitude") + ": " + apx.latToString(d.lat_scr) + "\n" +
                                       qsTr("Longitude") + ": " +apx.lonToString(d.lon_scr) + "\n" +
                                        qsTr("Altitude") + ": " +d.alt
                            }
                            source:{
                                image_path_preview
                            }

                            x:  select_button.width - layout.spacing
                            y: -snap_tool_area.height * 3
                        }
                    }

                    Timer {
                        id: updateTimer
                        interval: 100
                        running: true
                        repeat: true
                        onTriggered: {
                            if(model_button.width != (listView.width  - select_button.width - spacing)){
                                model_button.width = (listView.width  - select_button.width  - spacing)
                            }
                        }
                    }
                    function getDescription()
                    {
                        var descr_ =  snapshots.getFilterDescription(modelData)
                        return descr_
                    }
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

        FactButton {
            id: header_button1
            height: MenuStyle.itemSize
            width: listView.width
            enabled: true
            title: qsTr("Sorting Settings")
            descr: sort_criteria + " " + sort_order
            onTriggered: {
                snapshots.snapshot_sorting.trigger();
            }
            Timer {
                id: updateTimer2
                interval: 100
                running: true
                repeat: true
                onTriggered: {
                    if(header_button1.width != listView.width){
                        header_button1.width = listView.width
                    }
                }
            }
        }

        FactButton {
            id: header_button2
            height: MenuStyle.itemSize
            width: listView.width
           // anchors.left: header_button1
            enabled: true
            title: qsTr("Filter Settings")
            descr:{
                snapshot_filter.descr
            }
            onTriggered: {
                snapshots.snapshot_filter.trigger();
            }
            Timer {
                id: updateTimer3
                interval: 100
                running: true
                repeat: true
                onTriggered: {
                    if(header_button2.width != listView.width){
                        header_button2.width = listView.width
                    }
                }
            }
        }
        /*TextField {
            id: filterLabel
            z: 100
            enabled: true
            visible: enabled
            readOnly: true
            width:listView.width
            height: filterEnabled?implicitHeight:0
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 13
            //placeholderText: qsTr("Search")+"..."
            background: Rectangle{
                border.width: 0
                color: Material.background
            }
            text: {
                snapshot_filter.descr
            }
            Timer {
                id: updateTimerLabel
                interval: 100
                running: true
                repeat: true
                onTriggered: {
                    if(filterLabel.width != listView.width){
                        filterLabel.width = listView.width
                    }
                }
            }

        }*/

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
