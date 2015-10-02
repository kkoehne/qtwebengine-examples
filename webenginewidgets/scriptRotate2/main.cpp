#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QWebEngineView>
#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QWebEngineScript>
#include <QWebEngineScriptCollection>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWebEngineView view;
    view.setUrl(QUrl("http://www.qtworldsummit.com"));

    { // Use QWebEnginePage::scripts

        QString src;
        QStringList jsFiles = {":/jquery.min.js", ":/rotate.js"};
        foreach (const QString &fileName, jsFiles) {
            QFile file(fileName);
            if (!file.open(QFile::ReadOnly))
                return 1;
            src += QTextStream(&file).readAll();
        }

        QWebEngineScript script;
        script.setInjectionPoint(QWebEngineScript::DocumentReady);
        script.setWorldId(QWebEngineScript::ApplicationWorld);
        script.setSourceCode(src);
        view.page()->scripts().insert(script);
    }

    view.show();
    view.resize(1024, 750);

    return a.exec();
}
