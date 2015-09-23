/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkDiskCache>
#include <QtConcurrentMap>
#include "imageanalyzer.h"

ImageAnalyzer::ImageAnalyzer(QObject* parent)
    : QObject(parent), m_outstandingFetches(0)
{
    m_watcher = new QFutureWatcher<QRgb>(this);

    QObject::connect(&m_network, &QNetworkAccessManager::finished,
                     this, &ImageAnalyzer::handleReply);
    QObject::connect(m_watcher, &QFutureWatcher<QRgb>::finished,
                     this, &ImageAnalyzer::doneProcessing);
    QObject::connect(m_watcher, &QFutureWatcher<QRgb>::progressValueChanged,
                     this, &ImageAnalyzer::progressStatus);
}

ImageAnalyzer::~ImageAnalyzer()
{
    delete(m_watcher);
}

float ImageAnalyzer::red() const
{
    return qRed(m_rgb) / 2.55;
}

float ImageAnalyzer::green() const
{
    return qGreen(m_rgb) / 2.55;
}

float ImageAnalyzer::blue() const
{
    return qBlue(m_rgb) / 2.55;
}

void ImageAnalyzer::progressStatus(int newstat)
{
    emit updateProgress(newstat, m_watcher->progressMaximum());
}

bool ImageAnalyzer::isBusy() const
{
    return m_watcher->isRunning();
}

void ImageAnalyzer::startAnalysis(const QStringList & urls)
{
    m_urlQueue = urls;
    while (!m_urlQueue.isEmpty())
    {
        QUrl url = QUrl(m_urlQueue.takeFirst());
        m_outstandingFetches++;
        QNetworkRequest request = QNetworkRequest(url);
        request.setRawHeader("User-Agent", "The Qt Company Ltd. - Custom Qt app");
        m_network.get(request);
    }
}

void ImageAnalyzer::handleReply(QNetworkReply * pReply)
{
    m_outstandingFetches--;
    if (pReply->error()) {
        qDebug() << "Error code" << pReply->error();
        qDebug() << "Http code" << pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        return;
    }
    QImage image;
    image.load(pReply, 0);
    pReply->deleteLater();
    if (image.isNull()) {
        qDebug() << "bad image";
        qDebug() << pReply->rawHeaderList();
        foreach(QByteArray element, pReply->rawHeaderList()) {
            qDebug() << element << " = " << pReply->rawHeader(element);
        }
        return;
    }
    queueImage(image);
}

void ImageAnalyzer::doneProcessing()
{
    int rTot = 0;
    int bTot = 0;
    int gTot = 0;
    int count = m_watcher->future().results().size();
    foreach (const QRgb & triplet, m_watcher->future().results()) {
        rTot += qRed(triplet);
        bTot += qBlue(triplet);
        gTot += qGreen(triplet);
    }
    if (count)
        m_rgb = qRgb(rTot/count, bTot/count, gTot/count);
    else
        m_rgb = QRgb();

    emit busyChanged();
    emit redChanged(red());
    emit greenChanged(green());
    emit blueChanged(blue());
}

void ImageAnalyzer::queueImage(QImage img)
{
    if (!img.isNull())
        m_imageQueue << img;

    if (m_outstandingFetches == 0 && m_urlQueue.isEmpty()) {
        emit busyChanged();
        m_watcher->setFuture(QtConcurrent::mapped(m_imageQueue, averageRGB));
        m_imageQueue.clear();
    }
}

QRgb averageRGB(const QImage &img)
{
    int pixelCount = img.width() * img.height();
    int rAvg, gAvg, bAvg;

    // We waste some time here:
    for (int timeWaster=0; timeWaster < 100; timeWaster++) {
        quint64 rTot = 0;
        quint64 gTot = 0;
        quint64 bTot = 0;
        for (int i=0; i < img.width(); i++) {
            for (int j=0; j < img.height(); j++) {
                QRgb pixel = img.pixel(i,j);
                rTot += qRed(pixel);
                gTot += qGreen(pixel);
                bTot += qBlue(pixel);
            }
        }
        rAvg = (rTot)/(pixelCount);
        gAvg = (gTot)/(pixelCount);
        bAvg = (bTot)/(pixelCount);
    }
    return qRgb(rAvg, gAvg, bAvg);
}
