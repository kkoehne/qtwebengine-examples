TEMPLATE = app

QT += network webenginewidgets widgets concurrent webchannel
CONFIG += c++11

HEADERS += \
    content.h \
    mainwindow.h

SOURCES = main.cpp \
    content.cpp \
    mainwindow.cpp

RESOURCES = resources/markdowneditor.qrc

FORMS += \
    mainwindow.ui

DISTFILES +=
