#include <QApplication>
#include <QMainWindow>
#include <QtWebEngineWidgets>
#include <QWebEnginePage>
#include <QWebEngineScriptCollection>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QMainWindow window;

    auto view = new QWebEngineView(&window);
    view->setUrl(QUrl("http://www.qtworldsummit.com"));

    { // Use QWebEnginePage::scripts
        QWebEngineScript script;
        script.setName("rotate");
        script.setInjectionPoint(QWebEngineScript::DocumentReady);
        script.setWorldId(QWebEngineScript::ApplicationWorld);

        QStringList jsFiles = {":/jquery.min.js", ":/rotate.js"};
        foreach (const QString &fileName, jsFiles) {
            QFile file(fileName);
            if (!file.open(QFile::ReadOnly))
                return 1;
            script.setSourceCode(script.sourceCode() + QTextStream(&file).readAll());
        }

        view->page()->scripts().insert(script);
    }

    window.setCentralWidget(view);
    window.show();
    window.resize(1024, 768);

    return a.exec();
}
