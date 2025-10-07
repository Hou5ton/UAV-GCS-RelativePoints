import QtQuick 2.5
import QtLocation 5.9
import QtPositioning 5.6
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.12
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

import Apx.Map 1.0
import Apx.Map.Common 1.0
import Apx.Common 1.0

import FileValidator 1.0

MapLine {
    id: path
    property var fact: modelData

    //visible: snapshots.visible
    p1: QtPositioning.coordinate(fact.lat,fact.lon)
    p2: QtPositioning.coordinate(fact.lat_scr,fact.lon_scr)

    line.width: 1
    opacity: 0.5

    line.color: {
        line.color = colorDefault
        if(fact && fact.active){
            line.color = colorSelected
        }else if(fact && (fact.lat !== fact.lat_scr ||
                fact.lon !== fact.lon_scr)){
            line.color = colorMoved
        }
    }
}

