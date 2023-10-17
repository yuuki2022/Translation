#include "Server.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QCoreApplication>
#include <QBuffer>
#include <QAudioFormat>
#include <QAudioOutput>
#include <QFile>
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
                    QString content = soundToString(data); //record to text
                    QString result =  translate(content,0);//translate Chinese to English
                    QString response=result;//remember to change "answer" to your result
                    clientSocket->write(response.toUtf8());
                }


            });
        } });
}

Server::~Server()
{
    delete tcpServer;
    delete database;
}

void writeWavHeader(QIODevice &device, const QAudioFormat &format, qint64 dataSize)
{
    // Write the WAV file header
    device.write("RIFF");
    device.write(reinterpret_cast<const char *>(&dataSize), sizeof(qint32)); // File size excluding the first 8 bytes
    device.write("WAVE");
    device.write("fmt ");
    device.write(reinterpret_cast<const char *>(&format), sizeof(QAudioFormat));
    device.write("data");
    device.write(reinterpret_cast<const char *>(&dataSize), sizeof(qint32)); // Data size
}

QString Server::saveAudioFile(const QByteArray &audioData, const QString &filename = "audio.wav")
{
    // Set up the audio format
    QAudioFormat format;
    format.setSampleRate(16000); // 16kHz sample rate
    format.setChannelCount(1);   // 1 channel (mono)
    format.setSampleSize(16);    // 16-bit sample size
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    // Open a file for writing
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Could not open file for writing:" << file.errorString();
        return nullptr;
    }

    // Write the WAV file header
    writeWavHeader(file, format, audioData.size());

    // Write audio data to the file
    file.write(audioData);

    qDebug() << "WAV file saved:" << filename;
    return filename;
}

QString Server::soundToString(QString filename)
{

    // Define the audio format (adjust these parameters based on your audio data)
    QString command = "../../whisper.cpp/main ../Translation/ServerTranslation" + filename + "-otxt";
    Server::execBash(command);
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Could not open file for reading:" << file.errorString();
        return "-1";
    }

    // Create a QTextStream to read the file
    QTextStream in(&file);

    // Read the full content of the file
    QString fileContent = in.readAll();

    return fileContent;
}
QString FDAPI(QString word)
{
    // Specify the URL of the API
    QString apiUrl = "https://api.dictionaryapi.dev/api/v2/entries/en/" + word; // Replace with the actual API URL

    // Create a QNetworkAccessManager for making the request
    QNetworkAccessManager manager;

    // Create a request object with the API URL
    QNetworkRequest request(apiUrl);

    // Send the GET request
    QNetworkReply *reply = manager.get(request);

    QString definition = "-1";
    // Handle the response
    QObject::connect(reply, &QNetworkReply::finished, [&]()
                     {
        if (reply->error() != QNetworkReply::NoError)
        {
            qDebug() << "Error: " << reply->errorString();
            return;
        }

        // Read the response data
        QByteArray responseData = reply->readAll();

        // Parse the JSON response
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        QJsonObject jsonObject = jsonDoc.object();
        if (jsonObject.contains("title") && jsonObject["title"].toString() == "No Definitions Found")
        {
            QString errorMessage = jsonObject["message"].toString();
            qDebug() << "Error: " << errorMessage;// can not find the meaning
            definition    = "-1";
        }
        else{
            
            // Extract the first definition
            QJsonArray meanings = jsonObject["meanings"].toArray();
            if (!meanings.isEmpty() && meanings[0].isObject())
            {
                QJsonObject firstMeaning = meanings[0].toObject();
                QJsonArray definitions = firstMeaning["definitions"].toArray();
                if (!definitions.isEmpty() && definitions[0].isObject())
                {
                    QJsonObject firstDefinition = definitions[0].toObject();
                    QString definition = firstDefinition["definition"].toString();
                    qDebug() << "Definition: " << definition;
                }
            }
        }

            // Clean up
            reply->deleteLater(); });
    return definition;
}
QString Server::translate(const QString &text, int option)
{
    // option = 0 : sen
    // option = 1 :word
    QString result;
    if (option == 1)
    {
        result = database->getTargetWord(text);
        if (result == "can't find the word")
        {
            if (FDAPI(text) == "-1")
            {
                return result;
            }
            else
            {
                return FDAPI(text);
            }
        }
    }
    else if (option == 0)
    {
        qDebug()<<"text" + text;
        QString arg = "\"";
        arg = arg + text +"\" ";
        QString command = "python3 ../../trans/test.py --en " + arg;
        // Create a QProcess instance dynamically (on the heap)
        QProcess *process = new QProcess();

        // Set the command to be executed
        process->start(command);
        qDebug()<<command;
        qDebug() << "python running";

        // Wait for the process to finish (you can also connect signals for more control)
        process->waitForFinished();

        qDebug() << "python runned";

        // Read the output of the process
        result = process->readAllStandardOutput();

        // Delete the QProcess object when you're done with it
        process->deleteLater();

    }
    qDebug()<<"result : "<<result;
    return result;
}
