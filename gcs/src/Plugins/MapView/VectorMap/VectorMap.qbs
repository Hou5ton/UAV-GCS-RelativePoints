import qbs
import ApxApp

ApxApp.ApxPlugin {

    Depends {
        name: "Qt";
        submodules: [
            "sql" ,
            "network" ,
            "location-private" ,
            "positioning-private" ,
        ]
    }

    Depends { name: "ApxCore" }
    Depends { name: "ApxData" }
    Depends { name: "Map" }

    files: [
        "VectorMapReply.cpp",
        "VectorMapReply.h",
        "VectorMapsDB.cpp",
        "VectorMapsDB.h",
        "VectorPlugin.cpp",
        "VectorPlugin.h",
        "VectorPlugin.json",
        "VectorTileFetcher.cpp",
        "VectorTileFetcher.h",
        "VectorTileLoader.cpp",
        "VectorTileLoader.h",
        "VectorTiledMappingManagerEngine.cpp",
        "VectorTiledMappingManagerEngine.h",
    ]
}
