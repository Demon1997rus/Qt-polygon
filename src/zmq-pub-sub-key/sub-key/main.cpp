#include <QByteArray>
#include <QDebug>

#include <zmq.hpp>

/* Config sub key */
const int io_threads = 1;
const std::string address = "tcp://localhost:5563";
const std::string filter = "B";
/* Config end sub key */

int main()
{
    zmq::context_t context(io_threads);
    zmq::socket_t subscriber(context, ZMQ_SUB);
    subscriber.connect(address);
    subscriber.setsockopt(ZMQ_SUBSCRIBE, filter.data(), filter.size());

    while (true) {
        // Read envelope with address
        zmq::message_t msgKey;
        if (subscriber.recv(&msgKey)) {
            zmq::message_t msgData;
            if (subscriber.recv(&msgData)) {
                QByteArray key(reinterpret_cast<const char *>(msgKey.data()), msgKey.size());
                QByteArray data(reinterpret_cast<const char *>(msgData.data()), msgData.size());
                qDebug() << "key:" << key << "|"
                         << "data:" << data;
            }
        }
    }

    return 0;
}
