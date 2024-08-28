#include <QByteArray>
#include <QDebug>
#include <QTextStream>

#include <cstring>

#include <unistd.h>

#include <zmq.h>

int main(int argc, char *argv[])
{
    QTextStream in(stdin);

    void *context = zmq_ctx_new();
    Q_ASSERT(context);

    //Это сокет, которым мы отправляем сообщения.
    void *sender = zmq_socket(context, ZMQ_PUSH);
    Q_ASSERT(sender);
    int bindingResult = zmq_bind(sender, "tcp://*:4040");
    Q_ASSERT(bindingResult == 0);

    // Это сокет, которым мы отправляем пакетное сообщение.
    void *connector = zmq_socket(context, ZMQ_PUSH);
    Q_ASSERT(connector);
    int connectionResult = zmq_connect(connector, "tcp://localhost:5050");
    Q_ASSERT(connectionResult == 0);

    // Для синхронизации с работниками
    qDebug() << "Нажмите Enter, когда работники будут готовы...";
    in.readLine();
    qDebug() << "Отправка заданий работникам...";

    // Первое сообщение. Это также сигнальное начало сервера.
    QByteArray data = "-1";
    zmq_msg_t message;
    zmq_msg_init_size(&message, data.size());
    memcpy(zmq_msg_data(&message), data.data(), data.size());
    zmq_msg_send(&message, connector, 0);
    zmq_msg_close(&message);

    // Генерация рандомных чисел
    srandom(static_cast<unsigned>(time(NULL)));

    // Отправление задач
    int msec = 0;
    for (int count = 0; count < 100; ++count) {
        int load = 100 * random() / RAND_MAX;
        msec += load;
        QByteArray sendData;
        sendData.setNum(msec);
        zmq_msg_t msg;
        zmq_msg_init_size(&msg, sendData.size());
        memcpy(zmq_msg_data(&msg), sendData.data(), sendData.size());
        zmq_msg_send(&msg, sender, 0);
        zmq_msg_close(&msg);
    }
    qDebug() << QString("Общее количество: %1 мс").arg(msec);
    sleep(1);
    zmq_close(connector);
    zmq_close(sender);
    zmq_ctx_destroy(context);

    return 0;
}
