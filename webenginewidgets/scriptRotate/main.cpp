#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QWebEngineView>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWebEngineView view;
    view.setUrl(QUrl("http://www.qtworldsummit.com"));

    { // Use QWebEnginePage::runJavaScript

        QString src;
        QStringList jsFiles = {":/jquery.min.js", ":/rotate.js"};
        foreach (const QString &fileName, jsFiles) {
            QFile file(fileName);
            if (!file.open(QFile::ReadOnly))
                return 1;
            src += QTextStream(&file).readAll();
        }

        QObject::connect(&view, &QWebEngineView::loadFinished,
                         [src, &view]() {
            view.page()->runJavaScript(src);
        });

    }

    view.show();
    view.resize(1024, 750);

    return a.exec();
}
