#include <QApplication>
#include <QMainWindow>
#include <QtWebEngineWidgets>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QMainWindow window;

    QWebEngineView view;
    view.setUrl(QUrl("http://www.qtworldsummit.com"));

    { // Use QWebEnginePage::runJavaScript

        QString toInject;
        QStringList jsFiles = {":/jquery.min.js", ":/rotate.js"};
        foreach (const QString &fileName, jsFiles) {
            QFile file(fileName);
            if (!file.open(QFile::ReadOnly))
                return 1;
            toInject += QTextStream(&file).readAll();
        }

        QObject::connect(&view, &QWebEngineView::loadFinished,
                         [toInject, &view]() {
            view.page()->runJavaScript(toInject);
        });

    }

    window.setCentralWidget(&view);
    window.show();
    window.resize(1024, 768);

    return a.exec();
}
