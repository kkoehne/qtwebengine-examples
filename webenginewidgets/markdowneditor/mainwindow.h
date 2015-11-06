#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "content.h"
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setMarkdownText(const QString &text);

private slots:
    void onFileNew();
    void onFileOpen();
    void onFileSave();
    void onFileSaveAs();

    void onExit();

private:
    Ui::MainWindow *ui;
    QString filePath;
    bool modified;
    Content content;
};

#endif // MAINWINDOW_H
