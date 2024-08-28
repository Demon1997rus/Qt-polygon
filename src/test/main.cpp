#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>

int main()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("172.13.13.147");
    db.setPort(5432);
    db.setDatabaseName("pgs");
    db.setUserName("postgres");
    db.setPassword("");

    if (!db.open()) {
        qCritical() << db.lastError();
        return 1;
    }

    return 0;
}
