TEMPLATE = app
HEADERS = imageanalyzer.h
SOURCES = imageanalyzer.cpp \
	main.cpp

CONFIG += c++11
QT += network webenginewidgets widgets concurrent webchannel

RESOURCES = resources/imageanalyzer.qrc

EXAMPLE_FILES += html/index.html ../webkit-bridge-tutorial.qdoc outline.txt
