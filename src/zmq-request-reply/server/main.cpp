#include <QByteArray>

#include <iostream>

#include "unistd.h"
#include "zmq.hpp"

const std::string address = "tcp://*:5555";

int main()
{
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REP);
    socket.bind(address);

    while (true) {
        // Ждем запроса от клиента
        zmq::message_t request;
        socket.recv(&request);

        QByteArray recvData;
        recvData.resize(request.size());
        memcpy(recvData.data(), request.data(), request.size());
        std::cout << "Recv data: " << recvData.data() << std::endl;

        sleep(1);

        QByteArray sendData("World");
        zmq::message_t reply(sendData.size());
        memcpy(reply.data(), sendData.data(), sendData.size());
        socket.send(reply);
    }
}
