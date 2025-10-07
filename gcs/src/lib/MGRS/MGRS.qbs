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

    cpp.includePaths: [
        product.sourceDirectory,
        "App/qtsingleapplication",
        "App/qtlockedfile",
    ]

    files: [
            "BasicCoo.cpp",
            "floatingwidget.cpp",
            "mgrs.cpp",
            "mgrsgridform.cpp",
            "BasicCoo.h",
            "MGRS_global.h",
            "floatingwidget.h",
            "mgrs.h",
            "mgrsgridform.h",
            "mgrsgridform.ui",
            "searchForm.ui",
            "mgrs.qrc"
    ]


}
