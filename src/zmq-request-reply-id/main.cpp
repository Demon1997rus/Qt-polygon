#include <QByteArray>
#include <QDebug>

#include "zmq.hpp"

/* Config */
const int io_threads = 1;
const std::string address_router = "tcp://*:5556";
const std::string address_request = "tcp://localhost:5556";
const std::string data_anonymous = "ROUTER uses a generated 5 byte identity";
const std::string id_identified = "PEER2";
const std::string data_identified = "ROUTER socket uses REQ's socket identity";
/* End config */

/*!
 * \brief main - проверка личности клиента
 */
int main()
{

    zmq::context_t context(io_threads);
    zmq::socket_t router(context, ZMQ_ROUTER);
    router.setsockopt(ZMQ_ROUTER_MANDATORY, 1);
    router.bind(address_router);

    //! First allow 0MQ to set the identity
    zmq::socket_t anonymous(context, ZMQ_REQ);
    anonymous.connect(address_request);
    anonymous.send(data_anonymous.data(), data_anonymous.size());

    //! Then set the identity ourselves
    zmq::socket_t identified(context, ZMQ_REQ);
    identified.setsockopt(ZMQ_IDENTITY, id_identified.data(), id_identified.size());
    identified.connect(address_request);
    identified.send(data_identified.data(), data_identified.size());

    while (true) {
        zmq::message_t msg;
        if (router.recv(&msg)) {
            QByteArray data(msg.data<const char>(), msg.size());
            qDebug() << data;
        }
    }
}
