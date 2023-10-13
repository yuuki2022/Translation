#include "Client.h"
#include <QString>



void Client::initClient(const QString& ip,quint16 port)
{
      QHostAddress serverAddress(ip); // 服务器的IP地址
      quint16 serverPort = port; // 服务器的端口号

      client->connectToHost(ip, serverPort); // 连接到服务器地址和端口

      connect(client, &QTcpSocket::connected, this, [=](){
          qDebug() << "Connected to server!";
      });
      connect(client, &QTcpSocket::readyRead, this, &Client::readData);
}

void Client::sendMessage(const QString &mes)
{
    client->write(mes.toUtf8());
    qDebug() << "sendMessage: "<<mes;
}



Client::~Client()
{
    delete client;
}
