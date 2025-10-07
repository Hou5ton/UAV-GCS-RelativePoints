QT += xml widgets network
TEMPLATE = lib
TARGET = Map
DEFINES += QT_LARGEFILE_SUPPORT
CONFIG	+= qt warn_on staticlib concurent core
DESTDIR = $$PWD/../lib
MOC_DIR += $$PWD/../.moc/$$basename(TARGET)/
OBJECTS_DIR += $$PWD/../.obj/$$basename(TARGET)/
#UI_DIR += $$PWD/../.ui/$$basename(TARGET)/
RCC_DIR += $$PWD/../.rcc/$$basename(TARGET)/

#INCLUDEPATH +=  $$PWD/../../TileReader/src \
#LIBS += -L$$PWD/../../TileReader/lib -lTileReader

INCLUDEPATH +=  $$PWD/../../MGRS/src \
LIBS += -L$$PWD/../../MGRS/lib -lMGRS

HEADERS	+= \
    $$PWD/ArrBase.h \
    $$PWD/Array.h \
    $$PWD/BaseCl.h \
    $$PWD/BaseClId.h \
    $$PWD/BaseFun.h \
    $$PWD/Basics.h \
    $$PWD/Clipping.h \
    $$PWD/Coo.h \
    $$PWD/DrawObj.h \
    $$PWD/fmt.h \
    $$PWD/File.h \
    $$PWD/Formula.h \
    $$PWD/Globals.h \
    $$PWD/Geodesic.h \
    $$PWD/HeigthsMatrixScaler.h \
    $$PWD/Id.h \
    $$PWD/Karta.h \
    $$PWD/KartaInterface.h \
    $$PWD/Klassifikator.h \
    $$PWD/Layer.h \
    $$PWD/MImage.h \
    $$PWD/MString.h \
    $$PWD/Nadp.h \
    $$PWD/NadpKriv.h \
    $$PWD/Object.h \
    $$PWD/PatternConvertor.h \
    $$PWD/PlotDevice.h \
    $$PWD/Prosloiki.h \
    $$PWD/QtDevice.h \
    $$PWD/Refglobal.h \
    $$PWD/Relief.h \
    $$PWD/ReliefRaster.h \
    $$PWD/Raster.h \
    $$PWD/RoadGidroLay.h \
    $$PWD/SysDefine.h \
    $$PWD/Table.h \
    $$PWD/Topobject.h \
    $$PWD/Varglobal.h \
    $$PWD/Yaz.h \
    $$PWD/ZnakSistema.h \
    $$PWD/mconfig.h \
    $$PWD/plasti.h \
    $$PWD/privpoints.h \
    $$PWD/srezi.h \
    $$PWD/Demming/BmhConstants.h \
    $$PWD/Demming/BmhDescriptor.h \
    $$PWD/Demming/BmhMap.h \
    $$PWD/Demming/BmhSeek.h \
    $$PWD/Demming/Dem.h \
    $$PWD/Demming/Demming.h \
    $$PWD/Demming/MappedFile.h \
    $$PWD/Demming/MBmhFILE.h \
    $$PWD/Demming/Memory.h \
    $$PWD/Demming/MemoryItem.h \
    $$PWD/Demming/Ram.h \
    $$PWD/Demming/DemsTable.h

SOURCES	+= \
    $$PWD/ArrBase.cpp \
    $$PWD/Array.cpp \
    $$PWD/BaseCl.cpp \
    $$PWD/BaseFun.cpp \
    $$PWD/Basics.cpp \
    $$PWD/Coo.cpp \
    $$PWD/DrawObj.cpp \
    $$PWD/fmt.cpp \
    $$PWD/File.cpp \
    $$PWD/Formula.cpp \
    $$PWD/Globals.cpp \
    $$PWD/Geodesic.cpp \
    $$PWD/HeigthsMatrixScaler.cpp \
    $$PWD/Karta.cpp \
    $$PWD/KartaInterface.cpp \
    $$PWD/Klassifikator.cpp \
    $$PWD/Layer.cpp \
    $$PWD/MImage.cpp \
    $$PWD/MString.cpp \
    $$PWD/Nadp.cpp \
    $$PWD/NadpKriv.cpp \
    $$PWD/Object.cpp \
    $$PWD/PatternConvertor.cpp \
    $$PWD/PlotDevice.cpp \
    $$PWD/Prosloiki.cpp \
    $$PWD/QtDevice.cpp \
    $$PWD/Relief.cpp \
    $$PWD/ReliefRaster.cpp \
    $$PWD/Raster.cpp \
    $$PWD/RoadGidroLay.cpp \
    $$PWD/SysDefine.cpp \
    $$PWD/Table.cpp \
    $$PWD/Topobject.cpp \
    $$PWD/Yaz.cpp \
    $$PWD/ZnakSistema.cpp \
    $$PWD/mconfig.cpp \
    $$PWD/plasti.cpp \
    $$PWD/privpoints.cpp \
    $$PWD/srezi.cpp \
    $$PWD/Demming/BmhDescriptor.cpp \
    $$PWD/Demming/BmhMap.cpp \
    $$PWD/Demming/BmhSeek.cpp \
    $$PWD/Demming/Dem.cpp \
    $$PWD/Demming/Demming.cpp \
    $$PWD/Demming/MappedFile.cpp \
    $$PWD/Demming/MBmhFILE.cpp \
    $$PWD/Demming/Memory.cpp \
    $$PWD/Demming/MemoryItem.cpp \
    $$PWD/Demming/Ram.cpp \
    $$PWD/Demming/DemsTable.cpp


TRANSLATIONS_FILE_NAME = $$basename(TARGET)

TRANSLATIONS += \
    $${TRANSLATIONS_FILE_NAME}_ru.ts

DISTFILES += \
    $${TRANSLATIONS_FILE_NAME}_ru.ts



