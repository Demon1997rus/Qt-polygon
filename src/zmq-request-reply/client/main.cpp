#include <QByteArray>

#include <iostream>

#include "zmq.hpp"

const std::string address = "tcp://localhost:5555";
const QByteArray sendData = "Hello";

int main()
{
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REQ);
    socket.connect(address);

    while (true) {
        zmq::message_t msg(sendData.size());
        memcpy(msg.data(), sendData.data(), sendData.size());
        socket.send(msg);

        QByteArray recvData;
        zmq::message_t reply;
        socket.recv(&reply);
        recvData.resize(reply.size());
        memcpy(recvData.data(), reply.data(), reply.size());
        std::cout << "Recv data: " << recvData.data() << std::endl;
    }
}
