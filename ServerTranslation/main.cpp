#include <QCoreApplication>
#include "Server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Server serve;
    // serve.initSocket();

    return a.exec();
}
