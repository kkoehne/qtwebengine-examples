#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QWebChannel>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->editor, &QPlainTextEdit::textChanged,
            [this]() { content.setText(ui->editor->toPlainText()); });
    connect(ui->editor->document(), &QTextDocument::modificationChanged,
            [this](bool changed) { modified = changed; });

    QWebChannel *channel = new QWebChannel(this);
    channel->registerObject(QStringLiteral("content"), &content);
    ui->preview->page()->setWebChannel(channel);
    ui->preview->setUrl(QUrl("qrc:/index.html"));

    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::onFileNew);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::onFileOpen);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::onFileSave);
    connect(ui->actionSaveAs, &QAction::triggered, this, &MainWindow::onFileSaveAs);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::onExit);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setMarkdownText(const QString &text)
{
    ui->editor->setPlainText(text);
}

void MainWindow::onFileNew()
{
    if (modified) {
        QMessageBox::StandardButton button = QMessageBox::question(this, windowTitle(),
                             tr("You have unsaved changes. Do you want to create a new document anyway?"));
        if (button != QMessageBox::Yes)
            return;
    }

    filePath.clear();
    ui->editor->setPlainText(tr("## New document ##"));
    ui->editor->document()->setModified(false);
}

void MainWindow::onFileOpen()
{
    if (modified) {
        QMessageBox::StandardButton button = QMessageBox::question(this, windowTitle(),
                             tr("You have unsaved changes. Do you want to open a new document anyway?"));
        if (button != QMessageBox::Yes)
            return;
    }

    QString path = QFileDialog::getOpenFileName(this,
        tr("Open MarkDown File"), "", tr("MarkDown File (*.md)"));
    if (path.isEmpty())
        return;

    QFile f(path);
    if (!f.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, windowTitle(),
                             tr("Could not open file %1: %2").arg(
                                 QDir::toNativeSeparators(path), f.errorString()));
        return;
    }
    filePath = path;
    ui->editor->setPlainText(f.readAll());
}

void MainWindow::onFileSave()
{
    if (filePath.isEmpty()) {
        onFileSaveAs();
        return;
    }

    QFile f(filePath);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))  {
        QMessageBox::warning(this, windowTitle(),
                             tr("Could not write to file %1: %2").arg(
                                 QDir::toNativeSeparators(filePath), f.errorString()));
        return;
    }
    QTextStream str(&f);
    str << ui->editor->toPlainText();

    ui->editor->document()->setModified(false);
}

void MainWindow::onFileSaveAs()
{
    QString path = QFileDialog::getSaveFileName(this,
        tr("Save MarkDown File"), "", tr("MarkDown File (*.md, *.markdown)"));
    if (path.isEmpty())
        return;
    filePath = path;
    onFileSave();
}

void MainWindow::onExit()
{
    if (modified) {
        QMessageBox::StandardButton button = QMessageBox::question(this, windowTitle(),
                             tr("You have unsaved changes. Do you want to exit anyway?"));
        if (button != QMessageBox::Yes)
            return;
    }
    close();
}
