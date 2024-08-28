#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include <zmq.hpp>

using Threads = std::vector<std::thread>;

/* Config */
const int io_threads = 1;
const Threads::size_type nbr_workers = 10;
#if (defined(WIN32))
const std::string address_router = "tcp://*:5671"; // "ipc" doesn't yet work on windows.
const std::string address_request = "tcp://localhost:5671";
#else
const std::string address_router = "ipc://routing.ipc";
const std::string address_request = "ipc://routing.ipc";
#endif
/* End config */

void worker_thread(Threads::size_type nbr_worker)
{
    //! Мы используем строковый идентификатор для простоты здесь
    const std::string id = std::to_string(nbr_worker);

    zmq::context_t context(io_threads);
    zmq::socket_t worker(context, ZMQ_REQ);
    worker.setsockopt(ZMQ_IDENTITY, id.data(), id.size());
    worker.connect(address_request);

    int total = 0;
    const std::string data_request = "Hello router";
    while (true) {
        //! Скажим брокеру, что мы готовы к работе
        worker.send(data_request.data(), data_request.size());

        //! Получить рабочую нагрузку от брокера до завершения
        zmq::message_t msg_router;
        worker.recv(&msg_router);
        const std::string workload(msg_router.data<const char>(), msg_router.size());
        if (workload == "Fired!") {
            std::cout << "Выполнено: " << total << " заданий работником №" << nbr_worker
                      << "\n";
            break;
        }
        ++total;

        // Имитация затраченного времени на задание
        int m_rand = 1 + rand() % 500;
        std::this_thread::sleep_for(std::chrono::milliseconds(m_rand + 1));
    }
}

int main()
{
    //! Создание ZeroMQ контекста
    zmq::context_t context(io_threads);

    //! Создание сокета роутера и его привязка
    zmq::socket_t broker(context, ZMQ_ROUTER);
    broker.bind(address_router);

    //! Создаем работников с уникальным номером
    Threads workers;
    workers.reserve(nbr_workers);
    for (Threads::size_type nbr_worker = 1; nbr_worker <= nbr_workers; ++nbr_worker) {
        workers.emplace_back(std::move(std::thread(worker_thread, nbr_worker)));
    }

    //! Продолжаем в течение пяти секунд, а затем скажем работникам закончить
    auto end_point = std::chrono::high_resolution_clock::now() + std::chrono::seconds(5);
    int workers_fired = 0;
    while (true) {
        //! Следующее сообщение содержит сведения о недавно использовавшемся работнике
        zmq::message_t msg_id;
        broker.recv(&msg_id);
        zmq::message_t msg_empty_delimeter;
        broker.recv(&msg_empty_delimeter);
        zmq::message_t msg_data;
        broker.recv(&msg_data);

        std::string identity(msg_id.data<const char>(), msg_id.size());
        broker.send(identity.data(), identity.size(), ZMQ_SNDMORE);
        broker.send(std::string("").data(), 0, ZMQ_SNDMORE);

        //! Поддерживать работников до тех пор, пока не наступит время их уволить
        if (std::chrono::high_resolution_clock::now() < end_point) {
            std::string data = "Work harder";
            broker.send(data.data(), data.size());
        } else {
            std::string data = "Fired!";
            broker.send(data.data(), data.size());
            if (++workers_fired == nbr_workers) {
                break;
            }
        }
    }

    for (Threads::value_type &worker : workers) {
        worker.join();
    }
    return 0;
}
