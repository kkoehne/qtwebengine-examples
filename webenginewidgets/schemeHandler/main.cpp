#include <QApplication>
#include <QWebEngineUrlRequestJob>
#include <QWebEngineUrlSchemeHandler>
#include <QWebEngineView>
#include <QWebEngineProfile>

#include <QDebug>

class SchemeHandler : public QWebEngineUrlSchemeHandler
{
  Q_OBJECT
public:
  SchemeHandler(QObject *parent = nullptr)
    : QWebEngineUrlSchemeHandler(parent) {}

  void requestStarted(QWebEngineUrlRequestJob *job);
};

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

void SchemeHandler::requestStarted(QWebEngineUrlRequestJob *job)
{
  QUrl newUrl("http://doc.qt.io");
  newUrl.setPath(job->requestUrl().path());
  qDebug() << "Redirecting" << job->requestUrl() << "to" << newUrl;
  job->redirect(newUrl);
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWebEngineView view;
    MyPage page;
    view.setPage(&page);

    SchemeHandler handler;
    view.page()->profile()->installUrlSchemeHandler("help", &handler);

    view.setUrl(QUrl("qrc:/index.html"));
    view.show();
    view.resize(1024, 750);

    return app.exec();
}

#include "main.moc"
