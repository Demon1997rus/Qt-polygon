#include <QCoreApplication>
#include <QDebug>
#include <QString>
#include <QFile>
#include <QByteArray>
#include <QDataStream>

QString searchConfig(const QString& name)
{
    const QString configPath = QString(ROOT_PATH "data/%1").arg(name);
    return configPath;
}

QByteArray dataConfig(const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qCritical() << Q_FUNC_INFO;
        qCritical() << "File opening failed" << path;
        return QByteArray();
    }
    return file.readAll();
}

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);
    return app.exec();
}
