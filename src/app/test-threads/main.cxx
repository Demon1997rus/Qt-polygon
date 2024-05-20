#include <QCoreApplication>

#include "controller.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    new Controller(&app);
    return app.exec();
}
