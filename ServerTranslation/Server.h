#ifndef SERVER_H
#define SERVER_H
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QTcpServer>
#include <QObject>
#include "DatabaseProcess.h"

class Server:public QObject
{
Q_OBJECT
private:
    QTcpServer *tcpServer;
    DatabaseProcess *database;
public:
    Server();
    void initSocket();
    ~Server();
    QString soundToString(QByteArray data);//sound api
    QString translate(const QString& text,int option);//translate
public slots:
    void newConnection()
     {
         QTcpSocket* client = tcpServer->nextPendingConnection();
         connect(client, &QTcpSocket::readyRead, this, &Server::readData);
     }

     void readData()
     {
         QTcpSocket* client = qobject_cast<QTcpSocket*>(sender());
         if (client) {
             QByteArray data = client->readAll();
             qDebug() << "Received data from client: " << data;
         }
     }
};

#endif // SERVER_H
