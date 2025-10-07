import qbs
import qbs.File
import qbs.FileInfo
import ApxApp

ApxApp.ApxLibrary {

    Depends { name: "MGRS" }

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


    Group {
        name: "Demming"
        prefix: name+"/"
        files: [
            "BmhConstants.h",
            "BmhDescriptor.cpp",
            "BmhDescriptor.h",
            "BmhMap.cpp",
            "BmhMap.h",
            "BmhSeek.cpp",
            "BmhSeek.h",
            "Dem.cpp",
            "Dem.h",
            "Demming.cpp",
            "Demming.h",
            "DemsTable.cpp",
            "DemsTable.h",
            "MappedFile.cpp",
            "MappedFile.h",
            "MBmhFILE.cpp",
            "MBmhFILE.h",
            "Memory.cpp",
            "Memory.h",
            "MemoryItem.cpp",
            "MemoryItem.h",
            "Ram.cpp",
            "Ram.h",

        ]
    }
    files: [
        "Array.cpp",
        "Array.h",
        "ArrBase.cpp",
        "ArrBase.h",
        "BaseCl.cpp",
        "BaseCl.h",
        "BaseClId.h",
        "BaseFun.cpp",
        "BaseFun.h",
        "Basics.cpp",
        "Basics.h",
        "Clipping.h",
        "Coo.cpp",
        "Coo.h",
        "DrawObj.cpp",
        "DrawObj.h",
        "File.cpp",
        "File.h",
        "fmt.cpp",
        "fmt.h",
        "Formula.cpp",
        "Formula.h",
        "Geodesic.cpp",
        "Geodesic.h",
        "Globals.cpp",
        "Globals.h",
        "HeigthsMatrixScaler.cpp",
        "HeigthsMatrixScaler.h",
        "Id.h",
        "Karta.cpp",
        "Karta.h",
        "KartaInterface.cpp",
        "KartaInterface.h",
        "Klassifikator.cpp",
        "Klassifikator.h",
        "Layer.cpp",
        "Layer.h",
        "mconfig.cpp",
        "mconfig.h",
        "MercatorTile.cpp",
        "MercatorTile.h",
        "MImage.cpp",
        "MImage.h",
        "MString.cpp",
        "MString.h",
        "Nadp.cpp",
        "Nadp.h",
        "NadpKriv.cpp",
        "NadpKriv.h",
        "Object.cpp",
        "Object.h",
        "PatternConvertor.cpp",
        "PatternConvertor.h",
        "plasti.cpp",
        "plasti.h",
        "PlotDevice.cpp",
        "PlotDevice.h",
        "privpoints.cpp",
        "privpoints.h",
        "Prosloiki.cpp",
        "Prosloiki.h",
        "QtDevice.cpp",
        "QtDevice.h",
        "Raster.cpp",
        "Raster.h",
        "Refglobal.h",
        "Relief.cpp",
        "Relief.h",
        "ReliefRaster.cpp",
        "ReliefRaster.h",
        "RoadGidroLay.cpp",
        "RoadGidroLay.h",
        "srezi.cpp",
        "srezi.h",
        "SysDefine.cpp",
        "SysDefine.h",
        "Table.cpp",
        "Table.h",
        "Topobject.cpp",
        "Topobject.h",
        "Varglobal.h",
        "Yaz.cpp",
        "Yaz.h",
        "ZnakSistema.cpp",
        "ZnakSistema.h",
    ]


}
