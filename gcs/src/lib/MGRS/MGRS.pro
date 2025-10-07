QT +=  widgets
CONFIG += c++11 debug_and_release_target
TEMPLATE = lib
TARGET = MGRS
DEFINES += MGRS_LIBRARY
QMAKE_CXXFLAGS += -std=c++11

CONFIG	+= qt warn_on staticlib concurent core

DESTDIR = $$PWD/../lib
MOC_DIR += $$PWD/../.moc/$$basename(TARGET)/
OBJECTS_DIR += $$PWD/../.obj/$$basename(TARGET)/
UI_DIR += $$PWD/../.ui/$$basename(TARGET)/
RCC_DIR += $$PWD/../.rcc/$$basename(TARGET)/

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0



SOURCES += \
    BasicCoo.cpp \
    floatingwidget.cpp \
    mgrs.cpp \
    mgrsgridform.cpp

HEADERS += \
    BasicCoo.h \
    MGRS_global.h \
    floatingwidget.h \
    mgrs.h \
    mgrsgridform.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

FORMS += \
    mgrsgridform.ui \
    searchForm.ui

RESOURCES += \
    mgrs.qrc


#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../cpt-ui/Tools/lib/release/ -lTools
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../cpt-ui/Tools/lib/debug/ -lTools
#else:unix: LIBS += -L$$PWD/../../../cpt-ui/Tools/lib/ -lTools

#INCLUDEPATH += $$PWD/../../../cpt-ui/Tools/src
#DEPENDPATH += $$PWD/../../../cpt-ui/Tools/src

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../cpt-ui/Tools/lib/release/libTools.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../cpt-ui/Tools/lib/debug/libTools.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../cpt-ui/Tools/lib/release/Tools.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../cpt-ui/Tools/lib/debug/Tools.lib
#else:unix: PRE_TARGETDEPS += $$PWD/../../../cpt-ui/Tools/lib/libTools.a
