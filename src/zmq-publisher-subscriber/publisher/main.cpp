#include <QDebug>
#include <QTextStream>

#include "unistd.h"
#include "../weather/weather.h"
#include "zmq.hpp"

const std::string address = "tcp://*:5556";

int main()
{
    QTextStream in(stdin);
    zmq::context_t context;
    zmq::socket_t publisher(context, ZMQ_PUB);
    publisher.bind(address);
    srandom(static_cast<unsigned>(time(NULL)));
    in.readLine();

    while (true) {
        Weather weather;
        weather.zipcode = 1 + random() % 100000;
        weather.temperature = random() % 36;
        weather.relhumdity = random() % 50;

        QByteArray data = weather.serialize();

        zmq::message_t msg(data.size());
        memcpy(msg.data(), data.data(), data.size());
        publisher.send(msg);
        qDebug() << "Send: " << weather;
        sleep(1);
    }
}
