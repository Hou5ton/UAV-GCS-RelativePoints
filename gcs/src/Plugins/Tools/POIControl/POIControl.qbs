import qbs
import ApxApp


ApxApp.ApxPlugin {

    Depends {
        name: "Qt";
        submodules: [
            "core",
            "location",
            "xml",
            "widgets" //for QFileDialog
        ]
    }

    Depends { name: "ApxData" }
    Depends { name: "ApxGcs" }
    Depends { name: "qmlqrc" }
    Depends { name: "VideoStreaming" }

    files: [
        "poicontrolplugin.h",
        "poicontrol.cpp","poicontrol.h",
    ]
}
