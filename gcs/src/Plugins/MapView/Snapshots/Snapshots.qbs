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
        "SnapshotsPlugin.h",
        "Snapshots.cpp",
        "Snapshots.h",
        "SnapshotEdit.cpp",
        "SnapshotEdit.h",
        "LookupSnapshots.cpp",
        "LookupSnapshots.h",
        "filevalidator.cpp",
        "filevalidator.h",
    ]

    Depends { name: "qmlqrc" }
}
