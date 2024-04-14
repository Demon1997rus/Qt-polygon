#include <QCoreApplication>
#include <QDebug>
#include <QString>
#include <QFile>
#include <QByteArray>
#include <QDataStream>

#include <zmq.h>

inline QString searchConfig(const QString& name) { return QString(ROOT_PATH "data/%1").arg(name); }

QByteArray configData(const QString& configPath)
{
    QFile file(configPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qCritical() << Q_FUNC_INFO;
        qCritical() << "File opening failed" << configPath;
        return QByteArray();
    }
    return file.readAll();
}

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    return 0;
}
