#include <QApplication>
#include <QMainWindow>
#include <QWebEngineProfile>
#include <QWebEngineView>
#include <QWebEngineUrlRequestInterceptor>

#include <QDebug>

class RequestInterceptor : public QWebEngineUrlRequestInterceptor
{
    Q_OBJECT
public:
    RequestInterceptor(QObject *parent = nullptr) : QWebEngineUrlRequestInterceptor(parent)
    {
    }

    bool interceptRequest(QWebEngineUrlRequestInfo &info)
    {
        if (!info.requestUrl().host().contains("qt")) {
            qDebug() << "blocking" << info.requestUrl();
            info.block(true);
            return true;
        }
        return false;
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow window;
    QWebEngineView view;

    view.setUrl(QUrl("http://www.qtworldsummit.com"));

    RequestInterceptor interceptor;
    view.page()->profile()->setRequestInterceptor(&interceptor);
    window.setCentralWidget(&view);
    window.show();
    window.resize(1024, 750);

    return app.exec();
}

#include "main.moc"
