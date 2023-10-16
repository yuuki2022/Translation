#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileDialog>
#include <QAudioDecoder>
#include <QBuffer>
#include <QMessageBox>

const QString ip = "172.27.35.96";
//const QString ip = "127.0.0.1";
const qint16 port = 8082;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    //define a client
    client = new Client();
    client->initClient(ip, port);
    
    connect(ui->wordButton,&QPushButton::clicked,this,[=](){
        QJsonObject jsonObject;
        jsonObject["options"] = 1;  //1:word,0:sentence
        jsonObject["content"] = ui->lineEdit->text();
        
        QJsonDocument jsonDocument(jsonObject);
        QString jsonString = jsonDocument.toJson();
        qDebug() << jsonString << "\n";
        client->sendMessage(jsonString);
        //client->sendMessage(jsonString);
    });//click to send a word to its
    
    connect(ui->sentenceButton,&QPushButton::clicked,this,[=](){
        QJsonObject jsonObject;
        jsonObject["options"] = 0;  //1:word,0:sentence
        jsonObject["content"] = ui->lineEdit->text();

        QJsonDocument jsonDocument(jsonObject);
        QString jsonString = jsonDocument.toJson();
        qDebug() << jsonString << "\n";
        client->sendMessage(jsonString);
        //client->sendMessage(jsonString);
    });//click to send a word to its

    connect(client,&Client::response,this,[=](){
        qDebug()<<"response result::\n";
        ui->textEdit->setText(client->getResultMessage());
    });
    
    connect(ui->sound,&QPushButton::clicked,this,[=](){
        QString filePath = QFileDialog::getOpenFileName(this, "open audio", "", "audio type(*.wav *.mp3 *.ogg);;all files(*.*)");
        if (!filePath.isEmpty()) {
            QFile audioFile(filePath);
            
            if (audioFile.open(QIODevice::ReadOnly)) {
                while (!audioFile.atEnd()) {
                      QByteArray data = audioFile.read(1024); // 从文件读取数据块
                      client->sendMessage(data);
                  }
                audioFile.close();


            } else {
                QMessageBox::warning(this, "warning", "please selece");
            }
        } else {
            return;
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
    
}

