#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QtNetwork/QUdpSocket>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>

//this is client
class Client:public QObject
{
    Q_OBJECT
private:
    QTcpSocket* client;
    QString resultMessage;
public:
    Client(QObject* parent = nullptr) : QObject(parent)
    {
        client = new QTcpSocket(this);
    }

    void initClient(const QString& ip, quint16 port);
    void sendMessage(const QString& mes);
    void sendMessage(const QByteArray &mes);
    ~Client();
public slots:
    void readData()
    {
        QByteArray data = client->readAll();
        resultMessage = QString(data);
        qDebug() << "Received data from server: " << data;

    }
    QString getResultMessage()
    {
        return resultMessage;
    }
signals:
    void response();
};

#endif // CLIENT_H
