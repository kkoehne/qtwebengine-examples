#include <QApplication>
#include <QFile>
#include <QMainWindow>
#include <QWebEngineView>
#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QWebEngineScript>
#include <QWebEngineScriptCollection>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QMainWindow window;

    QWebEngineView view;
    view.setUrl(QUrl("http://www.qtworldsummit.com"));

    { // Use QWebEnginePage::scripts

        QStringList jsFiles = {":/jquery.min.js", ":/rotate.js"};
        foreach (const QString &fileName, jsFiles) {
            QFile file(fileName);
            if (!file.open(QFile::ReadOnly))
                return 1;
            QString content = QTextStream(&file).readAll();

            QWebEngineScript script;
            script.setName(fileName);
            script.setInjectionPoint(QWebEngineScript::DocumentReady);
            script.setWorldId(QWebEngineScript::ApplicationWorld);
            script.setSourceCode(content);
            view.page()->scripts().insert(script);
        }
    }

    window.setCentralWidget(&view);
    window.show();
    window.resize(1024, 768);

    return a.exec();
}
