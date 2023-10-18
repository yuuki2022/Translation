#include "Server.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QCoreApplication>
#include <QBuffer>
#include <QProcess>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QEventLoop>
#include <QDebug>

Server::Server()
{
    tcpServer = new QTcpServer();
    database = new DatabaseProcess();
}
QByteArray Server::execBash(QString &command)
{
    QProcess process;

    // Set the command to be executed
    process.start(command);

    // Wait for the process to finish (you can also connect signals for more control)
    process.waitForFinished();

    // Read the output of the process
    QByteArray result = process.readAllStandardOutput();

    return result;
}

void Server::initSocket()
{
    database->createDbConnection("stardict.db");
    if (!tcpServer->listen(QHostAddress("0.0.0.0"), 8082))
    {
        qDebug() << "Server could not start!";
    }
    else
    {
        qDebug() << "Server started on port 8082";
    }

    connect(tcpServer, &QTcpServer::newConnection, this, [this]()
            {
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
                                                                            //1:word 0:sentence
                        QString response = result;//remember to change "answer" to your result
                        clientSocket->write(response.toUtf8());
                    } else {
                        qDebug() << "JSON is not an object";
                    }
                } else {
                    qDebug() << "JSON can't be parsed: " << jsonParseError.errorString();



                }


            });
        } });
}

Server::~Server()
{
    delete tcpServer;
    delete database;
}

QString Server::translate(const QString &text, int option)
{
    // option = 0 : sen
    // option = 1 :word
    QString result;
    if (option == 1)
    {
        result = database->getTargetWord(text);
    }
    else if (option == 2)
    {
        qDebug() << "zh-en";
        result = database->getChineseWord(text);
    }
    else if (option == 0)
    {
        qDebug() << "text" + text;
        QString arg = "\"";
        arg = arg + text + "\" ";
        QString command = "python3 ../trans.py en " + arg;
        // Create a QProcess instance dynamically (on the heap)
        QProcess *process = new QProcess();

        // Set the command to be executed
        process->start(command);
        qDebug() << command;
        qDebug() << "python running";

        // Wait for the process to finish (you can also connect signals for more control)
        process->waitForFinished();

        qDebug() << "python runned";

        // Read the output of the process
        result = process->readAllStandardOutput();

        // Delete the QProcess object when you're done with it
        process->deleteLater();
    }
    else if (option == -1)
    {
        qDebug() << "text" + text;
        QString arg = "\"";
        arg = arg + text + "\" ";
        QString command = "python3 ../../trans/trans.py zh " + arg;
        // Create a QProcess instance dynamically (on the heap)
        QProcess *process = new QProcess();

        // Set the command to be executed
        process->start(command);
        qDebug() << command;
        qDebug() << "python running";

        // Wait for the process to finish (you can also connect signals for more control)
        process->waitForFinished();

        qDebug() << "python runned";

        // Read the output of the process
        result = process->readAllStandardOutput();

        // Delete the QProcess object when you're done with it
        process->deleteLater();
    }
    qDebug() << "result : " << result;
    return result;
}
