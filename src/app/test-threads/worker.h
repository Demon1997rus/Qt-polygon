#ifndef WORKER_H
#define WORKER_H

#include <QObject>

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(const QString &configPath, QObject *parent = nullptr);

public slots:
    void open();

private:
    QString m_configPath;
};

#endif // WORKER_H
