import qbs
import qbs.File
import qbs.FileInfo
import ApxApp

ApxApp.ApxLibrary {
    Depends {
        name: "Qt";
        submodules: [
            "widgets",
            "xml",
            "quick",
            "quickwidgets",
            "quickcontrols2",
            "svg",
            "sql",
            "network",
            "serialport",
            "multimedia",
            "opengl",
            "location-private",
            "positioning-private",
        ]
    }

    Depends { name: "version_hpp" }

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: product.sourceDirectory
    }

    Depends { name: "cpp" }
    Depends { name: "sdk"; submodules: [ "libs", "headers" ] }

    cpp.defines: base.concat([
        'RELATIVE_PLUGIN_PATH="' + FileInfo.relativePath('/' + app.app_bin_path, '/' + app.app_plugin_path) + '"',
        'RELATIVE_DATA_PATH="' + FileInfo.relativePath('/' + app.app_bin_path, '/' + app.app_data_path) + '"',
        'RELATIVE_LIB_PATH="' + FileInfo.relativePath('/' + app.app_bin_path, '/' + app.app_library_path) + '"',
    ])

    property string libsPath: FileInfo.cleanPath(FileInfo.joinPaths(path, ".."))

    cpp.includePaths: [
        product.sourceDirectory,
        libsPath,
        "App/qtsingleapplication",
        "App/qtlockedfile",
    ]

    files: [
        "datepicker_common.h",
        "datepicker.h",
        "datepickerpopupfooter.h",
        "datepickerpopup.h",
        "datepickercalendar.h",
        "datepickercalendarnavigator.h",
        "datepickercalendarmonthview.h",
        "datepickercalendaryearview.h",
        "datepickercalendardecadeview.h",
        "datepickerabstractformater.h",
        "datepickerstandardformater.h",
        "datepickerhumanreadableformater.h",
        "datepickertimeedit.h",
        "datepicker.cpp",
        "datepickerpopupfooter.cpp",
        "datepickerpopup.cpp",
        "datepickercalendar.cpp",
        "datepickercalendarnavigator.cpp",
        "datepickercalendarmonthview.cpp",
        "datepickercalendaryearview.cpp",
        "datepickercalendardecadeview.cpp",
        "datepickerabstractformater.cpp",
        "datepickerstandardformater.cpp",
        "datepickerhumanreadableformater.cpp",
        "datepickertimeedit.cpp",
    ]


}
