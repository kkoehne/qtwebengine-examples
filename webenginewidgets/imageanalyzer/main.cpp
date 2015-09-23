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
        QStringList jsFiles = {":/qwebchannel.js", ":/imageanalyzer.js" };
        foreach (const QString &fileName, jsFiles) {
            QWebEngineScript script;
            script.setName(fileName);
            script.setWorldId(QWebEngineScript::MainWorld);

            QFile file(fileName);
            if (!file.open(QFile::ReadOnly))
                return 1;
            script.setSourceCode(QTextStream(&file).readAll());
            view.page()->scripts().insert(script);
        }
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
