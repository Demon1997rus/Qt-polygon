#include <csignal>
#include <functional>
#include <iostream>

#include <zmq.hpp>

volatile bool interrupted = false;

void signal_handler(int)
{
    interrupted = true;
}

void catch_signals()
{
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);
    std::signal(SIGSEGV, signal_handler);
    std::signal(SIGABRT, signal_handler);
}

int main()
{
    zmq::context_t context(1);

    zmq::socket_t socket(context, ZMQ_REP);
    socket.bind("tcp://*:5555");

    catch_signals();
    while (true) {
        zmq::message_t msg;
        try {
            socket.recv(&msg);
        } catch (const zmq::error_t &e) {
            std::cout << e.what() << std::endl;
        }

        if (interrupted) {
            std::cout << "interrupt received, killing program..." << std::endl;
            break;
        }
    }

    return 0;
}
