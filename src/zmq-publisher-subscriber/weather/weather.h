#ifndef WEATHER_H
#define WEATHER_H

#include <QByteArray>
#include <QDataStream>
#include <QDebug>

class Weather {
public:
    qint32 zipcode = 0;
    qint32 temperature = 0;
    qint32 relhumdity = 0;

public:
    Weather() = default;
    explicit Weather(const QByteArray &data);

    QByteArray serialize();
    void deserialize(const QByteArray &data);

    friend QDataStream &operator>>(QDataStream &stream, Weather &data);
    friend QDataStream &operator<<(QDataStream &stream, const Weather &data);
    friend QDebug operator<<(QDebug debug, const Weather &data);
};

#endif // WEATHER_H
