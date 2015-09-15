#include "logfilepositionsource.h"

#include <QFile>
#include <QTimer>
#include <QDebug>

LogFilePositionSource::LogFilePositionSource(QObject *parent)
    : QGeoPositionInfoSource(parent),
      logFile(new QFile(this)),
      timer(new QTimer(this))
{
    connect(timer, SIGNAL(timeout()), this, SLOT(readNextPosition()));

    logFile->setFileName(":/simplelog.txt");
    if (!logFile->open(QIODevice::ReadOnly))
        qWarning() << "Error: cannot open source file" << logFile->fileName();
}

QGeoPositionInfo LogFilePositionSource::lastKnownPosition(bool /*fromSatellitePositioningMethodsOnly*/) const
{
    return lastPosition;
}

LogFilePositionSource::PositioningMethods LogFilePositionSource::supportedPositioningMethods() const
{
    return AllPositioningMethods;
}

int LogFilePositionSource::minimumUpdateInterval() const
{
    return 500;
}

void LogFilePositionSource::startUpdates()
{
    int interval = updateInterval();
    if (interval < minimumUpdateInterval())
        interval = minimumUpdateInterval();

    timer->start(interval);
}

void LogFilePositionSource::stopUpdates()
{
    timer->stop();
}

void LogFilePositionSource::requestUpdate(int /*timeout*/)
{
    // For simplicity, ignore timeout - assume that if data is not available
    // now, no data will be added to the file later
    if (logFile->canReadLine())
        readNextPosition();
    else
        emit updateTimeout();
}

void LogFilePositionSource::readNextPosition()
{
    QByteArray line = logFile->readLine().trimmed();
    if (!line.isEmpty()) {
        QList<QByteArray> data = line.split(' ');
        double latitude;
        double longitude;
        bool hasLatitude = false;
        bool hasLongitude = false;
        QDateTime timestamp = QDateTime::fromString(QString(data.value(0)), Qt::ISODate);
        latitude = data.value(1).toDouble(&hasLatitude);
        longitude = data.value(2).toDouble(&hasLongitude);

        if (hasLatitude && hasLongitude && timestamp.isValid()) {
            QGeoCoordinate coordinate(latitude, longitude);
            QGeoPositionInfo info(coordinate, timestamp);
            if (info.isValid()) {
                lastPosition = info;
                emit positionUpdated(info);
            }
        }
    }
}

QGeoPositionInfoSource::Error LogFilePositionSource::error() const
{
    return QGeoPositionInfoSource::NoError;
}
