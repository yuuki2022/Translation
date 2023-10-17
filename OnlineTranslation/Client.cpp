#include "Client.h"
#include <QString>
#include <QTime>


void Client::initClient(const QString& ip,quint16 port)
{
      QHostAddress serverAddress(ip); // 服务器的IP地址
      quint16 serverPort = port; //

      QDateTime currentDateTime = QDateTime::currentDateTime();
      QString formattedTime = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");
      qDebug() << "connecting...   " << formattedTime;
      client->connectToHost(ip, serverPort);


      connect(client, &QTcpSocket::connected, this, [=](){
          qDebug() << "Connected to server!";
      });
      connect(client, &QTcpSocket::readyRead, this, &Client::readData);
}

void Client::sendMessage(const QString &mes)
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString formattedTime = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");
    client->write(mes.toUtf8());
    qDebug() << formattedTime  << ":" <<"sendMessage: "<<mes;
}

void Client::sendMessage(const QByteArray &mes)
{
    client->write(mes);
}



Client::~Client()
{
    delete client;
}
