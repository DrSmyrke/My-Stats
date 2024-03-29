#-------------------------------------------------
#
# Project created by QtCreator 2018-09-20T10:17:59
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mystats
TEMPLATE = app

CONFIG(debug, debug|release):CONFIGURATION=debug
CONFIG(release, debug|release):CONFIGURATION=release

build_pass:CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,_debug)
}

CONFIG += c++11

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
        mainwindow.cpp \
    hwmonitorwidget.cpp \
    hwmonitor.cpp \
#    ../../../HomeNET/client.cpp \
#    ../../../HomeNET/myproto.cpp \
#    ../../../HomeNET/searcher.cpp \
    global.cpp

HEADERS  += mainwindow.h \
    hwmonitorwidget.h \
    hwmonitor.h \
#    ../../../HomeNET/client.h \
#    ../../../HomeNET/myproto.h \
#    ../../../HomeNET/searcher.h \
    global.h

TRANSLATIONS = lang/ru_RU.ts

RESOURCES += \
    resources.qrc

exists(./gitversion.pri):include(./gitversion.pri)
exists(./myLibs.pri):include(./myLibs.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
