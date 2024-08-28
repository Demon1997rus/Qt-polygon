#include "zmq.hpp"

/*!
 * \brief main - Правильный опрос нескольких сокетов в С++ zmq
 */
int main()
{
    zmq::context_t context(1);

    // Connect to task ventilator
    zmq::socket_t receiver(context, ZMQ_PULL);
    receiver.connect("tcp://localhost:5557");

    // Connect to weather server
    zmq::socket_t subscriber(context, ZMQ_SUB);
    subscriber.connect("tcp://localhost:5556");
    subscriber.setsockopt(ZMQ_SUBSCRIBE, "10001 ");

    // Initialize poll set
    zmq::pollitem_t items[] = {{receiver.operator void *(), 0, ZMQ_POLLIN, 0},
                               {subscriber.operator void *(), 0, ZMQ_POLLIN, 0}};

    // Process messages from both sockets
    while (true) {
        zmq::message_t message;

        // Если -1 то будет тут сидеть вечно пока не пройзодет событие в сокетах
        zmq::poll(items, 2, -1);

        if (items[0].revents & ZMQ_POLLIN) {
            if (receiver.recv(&message, ZMQ_DONTWAIT)) {
                // Process task
            }
        }
        if (items[1].revents & ZMQ_POLLIN) {
            if (subscriber.recv(&message, ZMQ_DONTWAIT)) {
                // Process weather update
            }
        }
    }

    return 0;
}
