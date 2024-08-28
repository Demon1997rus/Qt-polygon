#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDebug>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlError>

struct Options {
    QString password;
};

Options readCmdLineArgs(const QStringList &cmdLineArgs)
{
    // Settings cmd parser
    QCommandLineParser parser;

    const QCommandLineOption passwordOption(QStringList() << "p"
                                                          << "password",
                                            "Database Password", "password", "");

    const QCommandLineOption helpOption(QStringList() << "h"
                                                      << "help",
                                        "Prints help text and exit");

    parser.addOption(passwordOption);
    parser.addOption(helpOption);
    parser.parse(cmdLineArgs);

    if (parser.isSet(helpOption)) {
        printf("%s\n", parser.helpText().toLatin1().data());
        exit(EXIT_SUCCESS);
    }

    Options opts;
    opts.password = parser.value(passwordOption);
    return opts;
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    const auto &opts = readCmdLineArgs(app.arguments());

    const QString configPath = ROOT_PATH "/data/database/settings.ini";
    QSettings settings(configPath, QSettings::IniFormat, &app);

    settings.beginGroup("database");
    QSqlDatabase db = QSqlDatabase::addDatabase(settings.value("type").toString());
    db.setHostName(settings.value("hostname").toString());
    db.setPort(settings.value("port").toInt());
    db.setDatabaseName(settings.value("name").toString());
    db.setUserName(settings.value("username").toString());
    db.setPassword(opts.password);
    settings.endGroup();
    qInfo() << "* * * Settings database * * *";
    qInfo() << "Type:" << db.driverName();
    qInfo() << "Hostname:" << db.hostName();
    qInfo() << "Port:" << db.port();
    qInfo() << "Name:" << db.databaseName();
    qInfo() << "Username:" << db.userName();
    qInfo() << "Password:" << db.password();
    if (!db.open()) {
        qCritical() << db.lastError();
        return 1;
    }

    return app.exec();
}
