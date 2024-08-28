#include <QByteArray>
#include <QDebug>

#include "zmq.hpp"

int main()
{
    const QByteArray data = "Hello";
    const int requestSize = 1;

    zmq::context_t context(1);

    zmq::socket_t requester(context, ZMQ_REQ);
    requester.connect("tcp://localhost:5559");

    for (int request = 0; request < requestSize; ++request) {

        requester.send(data.data(), data.size());

        zmq::message_t msg;
        if (requester.recv(&msg)) {
            const QByteArray reply(reinterpret_cast<const char *>(msg.data()), msg.size());
            qDebug() << "Received reply" << request << "[" << reply << "]";
        }
    }
}
