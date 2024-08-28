#include <QByteArray>
#include <QDebug>

#include "zmq.h"

int main(int argc, char *argv[])
{
    void *context = zmq_ctx_new();
    Q_ASSERT(context);

    void *receiver = zmq_socket(context, ZMQ_PULL);
    Q_ASSERT(receiver);
    int bindingResult = zmq_bind(receiver, "tcp://*:5050");
    Q_ASSERT(bindingResult == 0);

    //Мы получаем первое сообщение и отбрасываем его, так как это
    //сигнал начала партии, который равен -1.
    zmq_msg_t reply;
    zmq_msg_init(&reply);
    zmq_msg_recv(&reply, receiver, 0);
    QByteArray data;
    data.resize(zmq_msg_size(&reply));
    memcpy(data.data(), zmq_msg_data(&reply), data.size());
    zmq_msg_close(&reply);

    for (int count = 1; count <= 100; ++count) {
        zmq_msg_t reply;
        zmq_msg_init(&reply);
        zmq_msg_recv(&reply, receiver, 0);
        QByteArray data;
        data.resize(zmq_msg_size(&reply));
        memcpy(data.data(), zmq_msg_data(&reply), data.size());
        zmq_msg_close(&reply);
        qDebug("Задача №%d решена", count);
        if (count % 10 == 0) {
            qDebug() << "10 Tasks have been processed.";
        }
    }
    zmq_close(receiver);
    zmq_ctx_destroy(context);

    return 0;
}
