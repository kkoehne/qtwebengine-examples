#include "imageanalyzer.h"

#include <QApplication>
#include <QMainWindow>
#include <QtWebEngineWidgets>
#include <QWebEnginePage>
#include <QWebChannel>


class MyPage : public QWebEnginePage
{
    Q_OBJECT
public:
    explicit MyPage(QObject *parent = 0) : QWebEnginePage(parent) {}
    void javaScriptConsoleMessage(JavaScriptConsoleMessageLevel /*level*/, const QString &message, int lineNumber, const QString &sourceID) {
        qDebug() << sourceID << lineNumber << message;
    }
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QMainWindow window;

    auto view = new QWebEngineView(&window);
    view->setPage(new MyPage(view));
    view->setUrl(QUrl("qrc:/index.html"));


    { // Use QWebEnginePage::runJavaScript

        QString toInject;
        QStringList jsFiles = {":/qwebchannel.js", ":/imageanalyzer.js" };
        foreach (const QString &fileName, jsFiles) {
            QWebEngineScript script;
            script.setName(fileName);
            script.setWorldId(QWebEngineScript::MainWorld);

            QFile file(fileName);
            if (!file.open(QFile::ReadOnly))
                return 1;
            script.setSourceCode(QTextStream(&file).readAll());
            view->page()->scripts().insert(script);
        }

    }

    ImageAnalyzer analyzer;
    QWebChannel channel;
    channel.registerObject(QStringLiteral("imageAnalyzer"), &analyzer);
    view->page()->setWebChannel(&channel);

    window.setCentralWidget(view);
    window.show();
    window.resize(1024, 768);

    return a.exec();
}

#include "main.moc"
