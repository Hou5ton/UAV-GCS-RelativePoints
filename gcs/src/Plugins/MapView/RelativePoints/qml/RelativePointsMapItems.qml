import QtQuick 2.5
import QtLocation 5.9
import QtPositioning 5.6
import QtQml 2.12
import QtQuick.Controls 2.12

import Apx.Map.Common 1.0

MapItemGroup {
    id: group

    property var relativepoints: apx.tools.relativepoints
    property bool showGroup: apx.tools && relativepoints
    property var map: ui.map
    property real lat: m.gps_lat.value
    property real lon: m.gps_lon.value

    readonly property color colorDefault: Style.cPoint
    readonly property color colorSelected: Style.cGreen

    z: 150

    MapItemView {
        id: relativePointsView
        z: 150
        model: group.showGroup?relativepoints.lookup.dbModel:0
        delegate: RelativePointItem { }
    }

    MapItemView {
        id: relativePointsLineView
        z: 100
        model: group.showGroup?relativepoints.lookup.dbModel:0
        delegate: RelativePointLine { }
    }

    Component.onCompleted: {
        relativepoints.lookup.area=Qt.binding(function(){return map.area})
        //map.addMapItemView(this/*group*/)
        map.addMapItemView(relativePointsView)
        map.addMapItemView(relativePointsLineView)
    }

    Timer {
        id: snTimer
        interval: 500
        repeat: true
        running: true
        triggeredOnStart: true;
        onTriggered: {
            for(var scr in relativePointsView.children){
                var fact_scr = relativePointsView.children[scr].fact

                for(var line in relativePointsLineView.children){
                    var fact_line = relativePointsLineView.children[line].fact

                    if(fact_scr.key === fact_line.key)
                    {
                        if(fact_line.key === relativepoints.currentKey){
                            relativePointsLineView.children[line].opacity = 1
                            if(lat && lon){
                                relativePointsLineView.children[line].visible = true
                            }else{
                                relativePointsLineView.children[line].visible = false
                            }
                        }else{
                            relativePointsLineView.children[line].opacity = 0.7
                            relativePointsLineView.children[line].visible = false
                        }
                    }
                }
            }

        }
    }
}
