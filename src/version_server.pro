#-------------------------------------------------
#
# Project created by QtCreator 2018-09-20T10:17:59
#
#-------------------------------------------------

QT		+= core network
QT		-= gui widgets

TARGET = mystatsd
TEMPLATE = app

CONFIG(debug, debug|release):CONFIGURATION=debug
CONFIG(release, debug|release):CONFIGURATION=release

build_pass:CONFIG(debug, debug|release) {
    unix: TARGET = $$join(TARGET,,,_debug)
    else: TARGET = $$join(TARGET,,,d)
}

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += APP_NAME=\\\"$$TARGET\\\"
DEFINES += SERVER_VERSION

CONFIG += c++11 console
CONFIG -= app_bundle

OBJECTS_DIR         = ../build/obj/$${CONFIGURATION}
MOC_DIR             = ../build/$${CONFIGURATION}
RCC_DIR             = ../build/rcc
UI_DIR              = ../build/ui
DESTDIR             = ../bin

win32|win64{
    RC_FILE=  index.rc
    OTHER_FILES+= index.rc
    DISTFILES += index.rc
}
QMAKE_CXXFLAGS += "-std=c++11"

SOURCES += main.cpp\
    hwmonitor.cpp \
    global.cpp \
    api.cpp \
#    ../../../HomeNET/client.cpp \
#    ../../../HomeNET/myproto.cpp \
#    ../../../HomeNET/searcher.cpp \
    myfunctions.cpp \
    core.cpp

HEADERS  += hwmonitor.h \
    global.h \
    api.h \
#    ../../../HomeNET/client.h \
#    ../../../HomeNET/myproto.h \
#    ../../../HomeNET/searcher.h \
    myfunctions.h \
    core.h \
    version.h

TRANSLATIONS = lang/ru_RU.ts

RESOURCES += \
    resources.qrc

exists(./gitversion.pri):include(./gitversion.pri)
exists(./myLibs.pri):include(./myLibs.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
