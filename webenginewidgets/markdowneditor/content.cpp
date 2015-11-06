#include "content.h"

Content::Content(QObject *parent) : QObject(parent)
{
}

void Content::setText(const QString &text)
{
    if (text == m_text)
        return;
    m_text = text;
    emit textChanged(m_text);
}
