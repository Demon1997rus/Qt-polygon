#include <QByteArray>
#include <QCoreApplication>
#include <QDebug>

#include <memory>
#include <string>
#include <thread>

#include "zmq.hpp"

#define INIT_CONTEXT(...) std::make_shared<zmq::context_t>(__VA_ARGS__)

using SmartContext = std::shared_ptr<zmq::context_t>;

const std::string address = "inproc://somename";

void process_pub(SmartContext context)
{
    zmq::socket_t publisher(*context, ZMQ_PUB);
    publisher.bind(address);

    while (true) {
        quint32 cmd = 1;
        QByteArray data = "hello";

        zmq_send(publisher.operator void *(), &cmd, sizeof(cmd), ZMQ_SNDMORE);
        zmq_send(publisher.operator void *(), data.data(), data.size(), 0);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void process_sub(SmartContext context)
{
    zmq::socket_t subscriber(*context, ZMQ_SUB);
    subscriber.connect(address);
    subscriber.setsockopt(ZMQ_SUBSCRIBE, 0, 0);

    while (true) {
        zmq::message_t cmd;
        if (subscriber.recv(&cmd)) {
            Q_ASSERT(cmd.more());
            zmq::message_t msg;
            if (subscriber.recv(&msg)) {
                quint32 cmd_value = *reinterpret_cast<quint32 *>(cmd.data());
                QByteArray data(reinterpret_cast<const char *>(msg.data()), msg.size());
                qDebug() << "cmd:" << cmd_value << "| data:" << data;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    SmartContext context = INIT_CONTEXT(1);
    std::thread(&process_pub, context).detach();
    std::thread(&process_sub, context).detach();

    return app.exec();
}
