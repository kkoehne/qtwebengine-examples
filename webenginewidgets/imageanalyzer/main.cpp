#include "imageanalyzer.h"

#include <QApplication>
#include <QMainWindow>
#include <QtWebEngineWidgets>
#include <QWebEnginePage>
#include <QWebChannel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QMainWindow window;

    QWebEngineView view;
    view.setUrl(QUrl("qrc:/index.html"));

    {   // Inject js files
        QString src;
        QStringList jsFiles = {":/qwebchannel.js", ":/imageanalyzer.js" };
        foreach (const QString &fileName, jsFiles) {
            QFile file(fileName);
            if (!file.open(QFile::ReadOnly))
                return 1;
            src += QTextStream(&file).readAll();
        }

        QWebEngineScript script;
        script.setWorldId(QWebEngineScript::MainWorld);
        script.setSourceCode(src);
        view.page()->scripts().insert(script);
    }

    // Set up WebChannel
    ImageAnalyzer analyzer;
    QWebChannel channel;
    channel.registerObject(QStringLiteral("imageAnalyzer"), &analyzer);
    view.page()->setWebChannel(&channel);


    window.setCentralWidget(&view);
    window.show();
    window.resize(1024, 750);

    return a.exec();
}
