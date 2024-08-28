#include <chrono>
#include <iostream>
#include <thread>

#include "zmq.hpp"

/* Config */
namespace config {
constexpr const int io_threads = 1;
constexpr const char *address_router = "tcp://*:5671";
constexpr const char *address_dealer = "tcp://localhost:5671";
constexpr const int nbr_workers = 10;
}
/* End config */

void worker_task(int nbr_worker)
{
    std::string id = std::to_string(nbr_worker);
    zmq::context_t context(config::io_threads);

    zmq::socket_t worker(context, ZMQ_DEALER);
    worker.setsockopt(ZMQ_IDENTITY, id.data(), id.size());
    worker.connect(config::address_dealer);

    const std::string data = "Hello router, take task";
    int total = 0;
    while (true) {
        worker.send(data.data(), data.size());

        zmq::message_t msg_workload;
        worker.recv(&msg_workload);

        const std::string workload(msg_workload.data<const char>(), msg_workload.size());
        if (workload == "Fired!") {
            std::cout << "Выполнено: " << total << " заданий\n";
            break;
        }
        ++total;

        int value_random = 1 + rand() % 500;
        std::this_thread::sleep_for(std::chrono::milliseconds(value_random));
    }
}

/*!
 * \brief main - В то время как этот пример выполняется в одном процессе, это просто сделать
 * проще запустить и остановить пример. Каждый поток имеет свой собственный
 * контекст и концептуально действует как отдельный процесс.
 */
int main()
{
    using Threads = std::vector<std::thread>;

    zmq::context_t context(config::io_threads);

    zmq::socket_t broker(context, ZMQ_ROUTER);
    broker.bind(config::address_router);
    srandom(static_cast<unsigned>(time(nullptr)));
    Threads workers;
    workers.reserve(Threads::size_type(config::nbr_workers));
    for (int nbr_worker = 1; nbr_worker <= config::nbr_workers; ++nbr_worker) {
        workers.emplace_back(std::move(std::thread(worker_task, nbr_worker)));
    }

    auto end_point = std::chrono::high_resolution_clock::now() + std::chrono::seconds(5);
    int workers_fired = 0;
    while (true) {
        zmq::message_t msg_id;
        broker.recv(&msg_id);
        zmq::message_t msg_data;
        broker.recv(&msg_data);

        std::string identity(msg_id.data<const char>(), msg_id.size());
        broker.send(identity.data(), identity.size(), ZMQ_SNDMORE);

        if (std::chrono::high_resolution_clock::now() < end_point) {
            std::string data = "Work harder";
            broker.send(data.data(), data.size());
        } else {
            std::string data = "Fired!";
            broker.send(data.data(), data.size());
            if (++workers_fired == config::nbr_workers) {
                break;
            }
        }
    }

    for (Threads::value_type &worker : workers) {
        worker.join();
    }
    return 0;
}
