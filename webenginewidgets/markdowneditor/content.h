#ifndef CONTENT_H
#define CONTENT_H

#include <QFile>
#include <QObject>

class Content : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString text MEMBER m_text NOTIFY textChanged)
public:
    explicit Content(QObject *parent = 0);

    void setText(const QString &text);

signals:
    void textChanged(const QString &text);

private:
    QString m_text;
};

#endif // CONTENT_H
