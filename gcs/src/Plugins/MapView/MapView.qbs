﻿import qbs.FileInfo

Project {

    property var plugins: [
        "Location",
        "MissionPlanner",
        "RelativePoints",
        "Sites",
        "Snapshots",
        "KmlOverlay",
        "VectorMap"
    ]

    references: {
        var refList=[]
        for(var i in plugins){
            var n=plugins[i]
            refList.push(FileInfo.joinPaths(n,n+".qbs"))
        }
        return refList
    }
}
