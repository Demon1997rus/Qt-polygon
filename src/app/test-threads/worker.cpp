#include "worker.h"

#include <QDebug>
#include <QThread>
#include <thread>
#include <iostream>

Worker::Worker(const QString &configPath, QObject *parent) : QObject(parent), m_configPath(configPath)
{}

void Worker::open()
{

}
