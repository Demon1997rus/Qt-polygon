#include "controller.h"

#include <QDir>
#include <QDebug>

#include "worker.h"

Controller::Controller(QObject *parent) : QObject(parent)
{
    // Путь до файла конфига
    const QDir dir(ROOT_PATH "/data/templates", "*.json");

    // Получаем список файлов
    const QFileInfoList templatesList = dir.entryInfoList(QDir::Files);

    // Получаем идеальное число потоков для нашего процесса и добавляем их в список
    const int idealThreadCount = qMin(templatesList.size(), QThread::idealThreadCount());
    workerThreads.reserve(idealThreadCount);
    for (int i = 0; i < idealThreadCount; ++i) {
        workerThreads.append(new QThread);
    }

    // Создание воркеров и справедливое распределение по потокам
    for (int index = 0; index < templatesList.size(); ++index) {
        QThread *workerThread = workerThreads[index % idealThreadCount];
        Worker *worker = new Worker(templatesList.at(index).filePath());
        worker->moveToThread(workerThread);
        connect(workerThread, &QThread::started, worker, &Worker::open);
        connect(workerThread, &QThread::finished, worker, &Worker::deleteLater);

    }

    for (QThread *threadWorker : workerThreads) {
        threadWorker->start();
    }
}

Controller::~Controller()
{
    for (QThread *workerThread : workerThreads) {
        workerThread->quit();
        workerThread->wait();
    }
    qDeleteAll(workerThreads);
}
