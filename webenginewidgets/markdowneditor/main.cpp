#include "content.h"
#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QtWebEngineWidgets>
#include <QMainWindow>
#include <QWebEnginePage>
#include <QWebChannel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow window;
    {
        QFile defaultTextFile(":/default.md");
        defaultTextFile.open(QIODevice::ReadOnly);
        window.setMarkdownText(defaultTextFile.readAll());
    }

    window.show();

    return a.exec();
}
