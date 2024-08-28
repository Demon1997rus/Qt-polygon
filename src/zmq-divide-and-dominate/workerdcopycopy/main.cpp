#include <QByteArray>
#include <QDebug>
#include <QtMath>

#include <unistd.h>

#include "zmq.h"

double square(double x)
{
    return qPow(x, 2);
}

double average(double x, double y)
{
    return (x + y) / 2.0;
}

double good_enough(double guess, double x)
{
    return qAbs(square(guess) - x) < 0.000001;
}

double improve(double guess, double x)
{
    return average(guess, x / guess);
}

double sqrt_inner(double guess, double x)
{
    if (good_enough(guess, x)) {
        return guess;
    }
    return sqrt_inner(improve(guess, x), x);
}

double newton_sqrt(double x)
{
    return sqrt_inner(1.0, x);
}

int main(int argc, char *argv[])
{
    void *context = zmq_ctx_new();
    Q_ASSERT(context);

    // Инициализируем сокет для получения сообщений.
    void *receiver = zmq_socket(context, ZMQ_PULL);
    Q_ASSERT(receiver);
    int connectReceiver = zmq_connect(receiver, "tcp://localhost:4040");
    Q_ASSERT(connectReceiver == 0);

    // Инициализируем сокет для отправки сообщений.
    void *sender = zmq_socket(context, ZMQ_PUSH);
    Q_ASSERT(sender);
    int connectSender = zmq_connect(sender, "tcp://localhost:5050");
    Q_ASSERT(connectSender == 0);

    int count = 1;
    while (true) {
        zmq_msg_t reply;
        zmq_msg_init(&reply);
        zmq_msg_recv(&reply, receiver, 0);
        QByteArray recvData;
        recvData.resize(zmq_msg_size(&reply));
        memcpy(recvData.data(), zmq_msg_data(&reply), recvData.size());
        zmq_msg_close(&reply);

        double value = recvData.toDouble();
        qDebug() << QString("%1: %2 %3").arg(value).arg(newton_sqrt(value)).arg(count++);
        //        sleep(1);

        zmq_msg_t message;
        QByteArray sendData("T");
        zmq_msg_init_size(&message, sendData.size());
        memcpy(zmq_msg_data(&message), sendData.data(), sendData.size());
        zmq_msg_send(&message, sender, 0);
        zmq_msg_close(&message);
    }
    zmq_close(receiver);
    zmq_close(sender);
    zmq_ctx_destroy(context);

    return 0;
}
