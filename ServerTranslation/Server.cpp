#include "Server.h"
#include <QJsonDocument>
#include <QJsonObject>

Server::Server()
{
    tcpServer = new QTcpServer();
    database = new DatabaseProcess();
}

void Server::initSocket()
{
    database->createDbConnection("stardict.db");

    if (!tcpServer->listen(QHostAddress("127.0.0.1"), 8082)) {
          qDebug() << "Server could not start!";
    } else {
          qDebug() << "Server started on port 8082";
    }

    connect(tcpServer, &QTcpServer::newConnection, this, [this]() {
        while (tcpServer->hasPendingConnections()) {
            QTcpSocket *clientSocket = tcpServer->nextPendingConnection();
            qDebug() << "listening.....";


            connect(clientSocket, &QTcpSocket::readyRead, this, [this, clientSocket]() {
                QByteArray data = clientSocket->readAll();
                qDebug() << "recevie: " << data;
                QJsonParseError jsonParseError;
                QString jsonString = QString(data);
                QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toUtf8(), &jsonParseError);
                //message can't chang to json / message can't be change to json(sound file)
                if (jsonParseError.error == QJsonParseError::NoError) {
                    if (jsonDocument.isObject()) {
                        QJsonObject jsonObject = jsonDocument.object();

                        int options = jsonObject["options"].toInt();
                        QString content = jsonObject["content"].toString();

                        QString result =  this->translate(content, options);//translate Chinese to English
                                                                            //0:word 1:sentence
                        QString response = "answer";//remember to change "answer" to your result
                        clientSocket->write(response.toUtf8());
                    } else {
                        qDebug() << "JSON is not an object";
                    }
                } else {
                    qDebug() << "JSON can't be parsed: " << jsonParseError.errorString();
                    QString content = soundToString(data); //record to text
                    QString result =  translate(content,0);//translate Chinese to English
                    QString response="answer";//remember to change "answer" to your result
                    clientSocket->write(response.toUtf8());
                }


            });
        }
    });
}

Server::~Server()
{
    delete tcpServer;
    delete database;
}

QString Server::soundToString(QByteArray data)
{
    return data;
}

QString Server::translate(const QString &text, int option)
{
    QString result;
    if(option==1){
        result = database->getTargetWord(text);
    }
    return result;
}
