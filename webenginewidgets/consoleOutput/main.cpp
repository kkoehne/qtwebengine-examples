#include <QApplication>
#include <QMainWindow>
#include <QWebEngineView>
#include <QMessageLogger>

class MyPage : public QWebEnginePage
{
    Q_OBJECT
public:
    explicit MyPage(QObject *parent = 0) : QWebEnginePage(parent) {}
    void javaScriptConsoleMessage(JavaScriptConsoleMessageLevel /*level*/,
                                  const QString &message, int lineNumber,
                                  const QString &sourceID) {
        QMessageLogger(sourceID.toUtf8().constData(), lineNumber, "", "js").debug(
                    message.toUtf8().constData());
    }
};

int main(int argc, char *argv[])
{
    qSetMessagePattern("[%{category}] %{file}:%{line} %{message}");
    QApplication app(argc, argv);

    QMainWindow window;
    QWebEngineView view;
    MyPage page;

    page.setUrl(QUrl("qrc:///index.html"));
    view.setPage(&page);

    window.setCentralWidget(&view);
    window.show();
    window.resize(1024, 750);

    return app.exec();
}

#include "main.moc"
