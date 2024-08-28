#include <QDebug>

#include "../weather/weather.h"
#include "zmq.hpp"

const std::string address = "tcp://localhost:5556";

int main()
{
    zmq::context_t context;
    zmq::socket_t subscriber(context, ZMQ_SUB);
    subscriber.connect(address);
    subscriber.setsockopt(ZMQ_SUBSCRIBE, 0, 0);

    while (true) {
        zmq::message_t reply;
        subscriber.recv(&reply);
        QByteArray data;
        data.resize(reply.size());
        memcpy(data.data(), reply.data(), reply.size());

        Weather weather(data);
        qDebug() << "Recv: " << weather;
    }
}
