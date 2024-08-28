#include <QByteArray>
#include <QDebug>

#include <iostream>

#include <zmq.hpp>

/* Config sub */
const int io_threads = 1;
const std::string address = "tcp://localhost:5561";
const std::string addressSync = "tcp://localhost:5562";

int main()
{
    zmq::context_t context(io_threads);

    // First, connect our subscriber socket
    zmq::socket_t subscriber(context, ZMQ_SUB);
    subscriber.connect(address);
    subscriber.setsockopt(ZMQ_SUBSCRIBE, nullptr, 0);

    // Second, synchronize with publisher
    zmq::socket_t synchronizer(context, ZMQ_REQ);
    synchronizer.connect(addressSync);

    // Send a synchronization request
    synchronizer.send(std::string("").data(), 0);

    // Wait for synchronization reply
    char rec[0];
    synchronizer.recv(rec, 0);

    // Third, get our updates and report how many we got
    int updateNbr = 0;

    while (true) {
        zmq::message_t msg;
        if (subscriber.recv(&msg)) {
            QByteArray data(reinterpret_cast<const char *>(msg.data()), msg.size());
            if (data == "END") {
                break;
            }
            ++updateNbr;
        }
    }
    std::cout << "Received " << updateNbr << " updates" << std::endl;

    return 0;
}
