TEMPLATE = lib
TARGET = dummydataplugin

CONFIG -= debug_and_release_target
debug_and_release {
    CONFIG(debug, debug|release) {
         mac: TARGET = $$join(TARGET,,,_debug)
         win32: TARGET = $$join(TARGET,,,d)
    }
}

CONFIG += plugin
QT += positioning

HEADERS += logfilepositionsource.h \
    factory.h

SOURCES += \
    logfilepositionsource.cpp \
    factory.cpp

RESOURCES += data.qrc

DISTFILES += plugin.json
