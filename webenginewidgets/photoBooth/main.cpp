#include <QApplication>
#include <QFile>
#include <QMainWindow>
#include <QWebEnginePage>
#include <QWebEngineView>
#include <QDebug>


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
    QApplication a(argc, argv);

    QMainWindow window;
    QWebEngineView view;

    MyPage page;
    page.setUrl(QUrl("qrc:/index.html"));

    QObject::connect(&page, &QWebEnginePage::featurePermissionRequested,
                     [&page](const QUrl &origin, QWebEnginePage::Feature feature) {
        qDebug() << "Permission requested";
        page.setFeaturePermission(origin, feature, QWebEnginePage::PermissionGrantedByUser);
    });

    view.setPage(&page);

    window.setCentralWidget(&view);
    window.show();
    window.resize(1024, 750);

    return a.exec();
}

#include "main.moc"
