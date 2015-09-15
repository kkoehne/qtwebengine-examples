#include "factory.h"
#include "logfilepositionsource.h"


QGeoPositionInfoSource *LogFilePositionSourceFactory::positionInfoSource(QObject *parent)
{
    return new LogFilePositionSource(parent);
}
