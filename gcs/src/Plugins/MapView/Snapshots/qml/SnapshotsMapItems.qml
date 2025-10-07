import QtQuick 2.5
import QtLocation 5.9
import QtPositioning 5.6
import QtQml 2.12

import Apx.Map.Common 1.0

MapItemGroup {
    id: group
    property var snapshots: apx.tools.snapshots
    property bool showGroup: apx.tools && snapshots
    property var map: ui.map

    readonly property color colorDefault: Style.cWhite
    readonly property color colorMoved: Style.cYellow
    readonly property color colorSelected: Style.cGreen

    z: 150

    MapItemView {
        id: snapshotsView
        z: 150
        model: group.showGroup?snapshots.lookup.dbModel:0
        delegate: SnapshotItem { }
    }

    MapItemView {
        id: snapshotsLineView
        z: 100
        model: group.showGroup?snapshots.lookup.dbModel:0
        delegate: SnapshotLine { }
    }

    Component.onCompleted: {
        snapshots.lookup.area=Qt.binding(function(){return map.area})
        //map.addMapItemView(this/*group*/)
        map.addMapItemView(snapshotsView)
        map.addMapItemView(snapshotsLineView)
    }

    Timer {
        id: snTimer
        interval: 1000
        repeat: true
        running: true
        triggeredOnStart: true;
        onTriggered: {
            for(var scr in snapshotsView.children){
                var fact_scr = snapshotsView.children[scr].fact

                for(var line in snapshotsLineView.children){
                    var fact_line = snapshotsLineView.children[line].fact

                    if(fact_scr.key === fact_line.key){
                        if(snapshotsView.children[scr].hover){
                            snapshotsLineView.children[line].line.width = 2
                            snapshotsLineView.children[line].opacity = 1
                        }else{
                                snapshotsLineView.children[line].line.width = 1
                                snapshotsLineView.children[line].opacity = 0.7
                        }
                    }

                    if(fact_line){
                        if(fact_line.active){
                            if(snapshotsLineView.children[line].line.color !== colorSelected){
                                snapshotsLineView.children[line].line.color = colorSelected
                            }
                        }else if(fact_line.lat !== fact_line.lat_scr ||
                                fact_line.lon !== fact_line.lon_scr){
                            if(snapshotsLineView.children[line].line.color !== colorMoved){
                                snapshotsLineView.children[line].line.color = colorMoved
                            }
                        }else{
                            if(snapshotsLineView.children[line].line.color !== colorDefault){
                                snapshotsLineView.children[line].line.color = colorDefault
                            }
                        }
                    }
                }

                if(fact_scr){
                    if(fact_scr.active){
                        if(snapshotsView.children[scr].color !== colorSelected){
                            snapshotsView.children[scr].color = colorSelected
                        }
                    }else if(fact_scr.lat !== fact_scr.lat_scr ||
                            fact_scr.lon !== fact_scr.lon_scr){
                        if(snapshotsView.children[scr].color !== colorMoved){
                            snapshotsView.children[scr].color = colorMoved
                        }
                    }else{
                        if(snapshotsView.children[scr].color !== colorDefault){
                            snapshotsView.children[scr].color = colorDefault
                        }
                    }
                }
            }
        }
    }
}
