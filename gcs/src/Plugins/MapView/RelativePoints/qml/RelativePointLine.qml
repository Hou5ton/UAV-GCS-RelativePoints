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
    property real lat: m.gps_lat.value
    property real lon: m.gps_lon.value

    p1: QtPositioning.coordinate(fact.lat,fact.lon)
    p2: QtPositioning.coordinate(lat,lon)

    line.width: 2
    opacity: 1

    line.color: {
        line.color = colorDefault
        if(fact && fact.active){
            line.color = colorSelected
        }
    }

    visible: false
}

