#pragma once

#include <QtConcurrent/QtConcurrentRun>

class ThreadLoop {
public:
    ~ThreadLoop() { exit(); }

    ThreadLoop() {}

    template <typename Functor>
    void exec(Functor functor)
    {
        if (!status.isRunning()) {
            status = QtConcurrent::run(this, &ThreadLoop::loop<Functor>, functor);
        }
    }

    void exit()
    {
        running = false;
        if (status.isRunning()) {
            status.waitForFinished();
        }
    }

private:
    template <typename Functor>
    void loop(Functor functor)
    {
        while (running) {
            functor();
        }
    }

private:
    volatile bool running = true;
    QFuture<void> status;
};

