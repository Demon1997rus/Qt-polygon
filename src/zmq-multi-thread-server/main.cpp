#include <csignal>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "zmq.hpp"

/* * * * * * * * * * *  Config * * * * * * * * * * * * */
const std::string client_address = "tcp://172.13.24.186:5555"; //! Адрес клиента
const std::string worker_address = "inproc://workers"; //! Связь с микросервисами
const int thread_count = 5; //! Количество микро-сервисов обрабатывающих запросы
const int io_threads = 1;                 //! Фоновые потоки отправки
const std::string respond_data = "World"; //! Ответ на запрос

void worker_routine(zmq::context_t &context)
{
    zmq::socket_t socket(context, ZMQ_REP);
    socket.connect(worker_address);

    while (true) {
        zmq::message_t request;
        socket.recv(&request);
        std::cout << "Received request: [" << reinterpret_cast<const char *>(request.data())
                  << "]\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
        socket.send(respond_data.data(), respond_data.size());
    }
}

int main()
{
    zmq::context_t context(io_threads);
    zmq::socket_t clients(context, ZMQ_ROUTER);
    clients.bind(client_address);

    zmq::socket_t workers(context, ZMQ_DEALER);
    workers.bind(worker_address);

    std::vector<std::thread> worker_threads;
    for (int thread_num = 0; thread_num < thread_count; ++thread_num) {
        worker_threads.emplace_back(std::thread(worker_routine, std::ref(context)));
    }

    zmq_proxy(static_cast<void *>(clients), static_cast<void *>(workers), nullptr);

    for (auto &worker_thread : worker_threads) {
        worker_thread.join();
    }

    return 0;
}
