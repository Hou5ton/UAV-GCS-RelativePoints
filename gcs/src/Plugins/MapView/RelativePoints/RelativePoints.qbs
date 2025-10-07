import qbs
import ApxApp

ApxApp.ApxPlugin {

    Depends { name: "DatePicker" }
    Depends {
        name: "Qt";
        submodules: [
            "core",
            "gui",
            "location",
            "sql"
        ]
    }


    Depends { name: "ApxData" }
    Depends { name: "Qt.core" }

    files: [
        "LookupRelativePoints.cpp",
        "LookupRelativePoints.h",
        "RelativePointEdit.cpp",
        "RelativePointEdit.h",
        "RelativePoints.cpp",
        "RelativePoints.h",
        "RelativePointsPlugin.h",
    ]

    Depends { name: "qmlqrc" }
}
