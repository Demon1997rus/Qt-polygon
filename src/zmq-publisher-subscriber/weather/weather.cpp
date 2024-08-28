#include "weather.h"

Weather::Weather(const QByteArray &data)
{
    deserialize(data);
}

QByteArray Weather::serialize()
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << *this;
    return data;
}

void Weather::deserialize(const QByteArray &data)
{
    QDataStream stream(data);
    stream >> *this;
}

QDebug operator<<(QDebug debug, const Weather &data)
{
    QDebugStateSaver saver(debug);
    Q_UNUSED(saver);
    debug << "zipcode = " << data.zipcode << "|";
    debug << "temperature = " << data.temperature << "|";
    debug << "relhumdity = " << data.relhumdity;
    return debug;
}

QDataStream &operator<<(QDataStream &stream, const Weather &data)
{
    stream.writeRawData(reinterpret_cast<const char *>(&data), sizeof(data));
    return stream;
}

QDataStream &operator>>(QDataStream &stream, Weather &data)
{
    stream.readRawData(reinterpret_cast<char *>(&data), sizeof(data));
    return stream;
}
