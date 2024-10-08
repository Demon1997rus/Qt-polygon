#include "zmq.hpp"

int main(int argc, char *argv[])
{
    zmq::context_t context(1);

    zmq::socket_t frontend(context, ZMQ_ROUTER);
    zmq::socket_t backend(context, ZMQ_DEALER);

    frontend.bind("tcp://*:5559");
    backend.bind("tcp://*:5560");

    zmq_proxy(static_cast<void *>(frontend), static_cast<void *>(backend), nullptr);
    return 0;
}
