#include <QGeoPositionInfoSourceFactory>
#include <QObject>

class LogFilePositionSourceFactory : public QObject, public QGeoPositionInfoSourceFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.qt.position.sourcefactory/5.0"
                      FILE "plugin.json")
    Q_INTERFACES(QGeoPositionInfoSourceFactory)
public:
    QGeoPositionInfoSource *positionInfoSource(QObject *parent);
    QGeoSatelliteInfoSource *satelliteInfoSource(QObject * /*parent*/) { return nullptr; }
    QGeoAreaMonitorSource *areaMonitor(QObject * /*parent*/) { return nullptr; }
};

