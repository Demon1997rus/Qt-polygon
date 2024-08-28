#include <unistd.h>
#include <zmq.hpp>

/* Config pub key */
const int io_threads = 1;
const std::string address = "tcp://*:5563";
const std::string keyA = "A";
const std::string keyB = "B";
const std::string messageA = "We don't want to see this";
const std::string messageB = "We would like to see this";
/* End config pub key */

int main()
{
    zmq::context_t context(io_threads);
    zmq::socket_t publisher(context, ZMQ_PUB);
    publisher.bind(address);

    while (true) {
        publisher.send(keyA.data(), keyA.size(), ZMQ_SNDMORE);
        publisher.send(messageA.data(), messageA.size());
        publisher.send(keyB.data(), keyB.size(), ZMQ_SNDMORE);
        publisher.send(messageB.data(), messageB.size());
        sleep(1);
    }
    return 0;
}
