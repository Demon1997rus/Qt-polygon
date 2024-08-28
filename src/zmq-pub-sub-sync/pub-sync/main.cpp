#include <QDebug>

#include <unistd.h>
#include <zmq.hpp>

/* Config publisher */
const int io_threads = 1;
const std::string address = "tcp://*:5561";
const std::string addressSync = "tcp://*:5562";
const int subscribersExpected = 10;
const int amountNbr = 1000000;
const std::string dataPub = "Rhubarb";
const std::string dataPubEnd = "END";
/* Config end */

int main()
{
    zmq::context_t context(io_threads);

    // Socket to talk to clients
    zmq::socket_t publisher(context, ZMQ_PUB);
    publisher.setsockopt(ZMQ_SNDHWM, int(0));
    publisher.bind(address);

    // Socket to receive signals
    zmq::socket_t synchronizer(context, ZMQ_REP);
    synchronizer.bind(addressSync);

    // Get synchronization from subscribers
    int subscribers = 0;
    while (subscribers < subscribersExpected) {

        char rec[0];
        // Wait for synchronization request
        synchronizer.recv(rec, 0);

        synchronizer.send(std::string("").data(), 0);

        ++subscribers;
    }

    // Now broadcast exactly 1M updates followed by END
    for (int updateNbr = 0; updateNbr < amountNbr; ++updateNbr) {
        publisher.send(dataPub.data(), dataPub.size());
    }

    // End message
    publisher.send(dataPubEnd.data(), dataPubEnd.size());

    // Give 0MQ time to flush output
    sleep(1);

    return 0;
}
