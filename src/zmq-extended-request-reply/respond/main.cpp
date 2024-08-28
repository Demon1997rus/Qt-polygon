#include <QByteArray>
#include <QDebug>

#include "unistd.h"
#include "zmq.hpp"

int main()
{
    const QByteArray data = "World";

    zmq::context_t context(1);

    zmq::socket_t responder(context, ZMQ_REP);
    responder.connect("tcp://localhost:5560");

    while (true) {
        // wait for next request from client
        zmq::message_t msg;
        if (responder.recv(&msg)) {
            const QByteArray string(reinterpret_cast<const char *>(msg.data()), msg.size());
            qDebug() << "Received request:" << string;
        }
        // Do same 'work'
        sleep(1);

        // Send reply back to client
        responder.send(data.data(), data.size());
    }
}
