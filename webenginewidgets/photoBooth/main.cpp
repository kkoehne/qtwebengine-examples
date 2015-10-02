#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QWebEnginePage>
#include <QWebEngineView>


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

    QWebEngineView view;

    MyPage page;
    page.setUrl(QUrl("qrc:/index.html"));

    QObject::connect(&page, &QWebEnginePage::featurePermissionRequested,
                     [&page](const QUrl &origin, QWebEnginePage::Feature feature) {
        qDebug() << "Permission requested";
        page.setFeaturePermission(origin, feature, QWebEnginePage::PermissionGrantedByUser);
    });

    view.setPage(&page);

    view.show();
    view.resize(1024, 750);

    return a.exec();
}

#include "main.moc"
