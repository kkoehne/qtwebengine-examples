#include <QApplication>
#include <QMainWindow>
#include <QWebEngineView>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow window;
    QWebEngineView view;

    view.setUrl(QUrl("http://www.qtworldsummit.com"));

    window.setCentralWidget(&view);
    window.show();
    window.resize(1024, 768);

    return app.exec();
}
