#include <iostream>
#include <string>
#include <thread>

#include <zmq.hpp>

/*!
 * \brief Обратите внимание, что многопоточный код, использующий этот шаблон, не масштабируется
 * до процессов. Если вы используете inproc и пары сокетов, вы создаете жестко ограниченное
 * приложение, то есть такое, в котором ваши потоки структурно взаимозависимы. Делайте это,
 * когда низкая задержка действительно жизненно важна. Другой шаблон проектирования - это
 * приложение со слабыми связями, где потоки имеют свой собственный контекст и обмениваются
 * данными через ipc или tcp . Вы можете легко разбить слабо связанные потоки на отдельные
 * процессы.
 */

const int io_threads = 1;
const std::string step2_address = "inproc://step2";
const std::string step3_address = "inproc://step3";
const std::string data_ready = "READY";

void step1(zmq::context_t &context)
{
    // Connect to step2 and tell it we're ready
    zmq::socket_t xmitter(context, ZMQ_PAIR);
    xmitter.connect(step2_address);

    std::cout << "Step 1 ready, signaling step 2\n";
    xmitter.send(data_ready.data(), data_ready.size());
}

void step2(zmq::context_t &context)
{
    // Bind inproc socket before starting step1
    zmq::socket_t receiver(context, ZMQ_PAIR);
    receiver.bind(step2_address);

    std::thread th(step1, std::ref(context));

    // Wait for signal and pass it on
    zmq::message_t msg;
    receiver.recv(&msg);

    // connect to step3 and tell it we're ready
    zmq::socket_t xmitter(context, ZMQ_PAIR);
    xmitter.connect(step3_address);

    std::cout << "Step 2 ready, signaling step 3\n";
    xmitter.send(data_ready.data(), data_ready.size());
    th.join();
}

int main()
{
    zmq::context_t context(io_threads);

    // Bind inproc socket before starting step 2
    zmq::socket_t receiver(context, ZMQ_PAIR);
    receiver.bind(step3_address);

    std::thread th(step2, std::ref(context));

    // Wait for signal
    zmq::message_t msg;
    receiver.recv(&msg);

    std::cout << "Test successful\n";

    th.join();

    return 0;
}
