#include "Server.h"
#include <QJsonDocument>
#include <QJsonObject>
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

    if (!tcpServer->listen(QHostAddress("127.0.0.1"), 8082))
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
// QString FDAPI(QString word)
// {

//     // Create a network manager
//     QNetworkAccessManager manager;

//     // Define the API URL
//     QString apiUrl = "https://api.dictionaryapi.dev/api/v2/entries/en/";
//     QString url = apiUrl + word;
//     // Create a network request
//     QNetworkRequest request(url);

//     // Perform the GET request
//     QNetworkReply *reply = manager.get(request);

//     // Create an event loop to wait for the request to finish
//     QEventLoop loop;
//     QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
//     loop.exec();

//     // Check for errors
//     if (reply->error() != QNetworkReply::NoError)
//     {
//         qDebug() << "Error:" << reply->errorString();
//         return "1";
//     }

//     // Read and parse the JSON response
//     QByteArray responseData = reply->readAll();
//     QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);

//     // Check if the JSON parsing was successful
//     if (jsonResponse.isNull())
//     {
//         qDebug() << "Error parsing JSON response";
//         return "1";
//     }
//     QJsonArray titleArray;
//     QJsonValue titleValue = jsonResponse.array()[0].toObject()["title"];
//     if (titleValue.isArray()) {
//         titleArray = titleValue.toArray();
//     }

//     if (!titleArray.isEmpty()) {
//         return "no meaning found";
//     }
//     // Extract the first meaning
//     QJsonArray meanings = jsonResponse.array()[0].toObject()["meanings"].toArray();
//     if (!meanings.isEmpty())
//     {
//         QJsonObject firstMeaning = meanings[0].toObject();
//         QString partOfSpeech = firstMeaning["partOfSpeech"].toString();
//         qDebug() << "Part of Speech:" << partOfSpeech;

//         QJsonArray definitions = firstMeaning["definitions"].toArray();
//         if (!definitions.isEmpty())
//         {
//             QJsonObject firstDefinition = definitions[0].toObject();
//             QString definition = firstDefinition["definition"].toString();
//             QString example = firstDefinition["example"].toString();
//             qDebug() << "Definition:" << definition;
//             qDebug() << "Example:" << example;
//             return definition;
//         }
//         else
//         {
//             qDebug() << "No definitions found.";
//         }
//     }
//     else
//     {
//         qDebug() << "No meanings found.";
//     }

//     // Clean up
//     reply->deleteLater();
// }
QString Server::translate(const QString &text, int option)
{
    // option = 0 : sen
    // option = 1 :word
    QString result;
    if (option == 1)
    {
        result = database->getTargetWord(text);
        // if (result == "can't find the word")
        // {
        //     if (FDAPI(text) == "no meaning found")
        //     {
        //         return result;
        //     }
        //     else
        //     {
        //         return FDAPI(text);
        //     }
        // }
    }
    else if (option == 0)
    {
        QString command = "python3 ../../trans/test.py --en " + text;
        // Create a QProcess instance
        QProcess process;

        // Set the command to be executed
        process.start(command);

        // Wait for the process to finish (you can also connect signals for more control)
        process.waitForFinished();

        // Read the output of the process
        QString result = process.readAllStandardOutput();
    }
    return result;
}
