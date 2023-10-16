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
    init();
    //define a client  自然语言模型
    client = new Client();
    client->initClient(ip, port);
    ui->textEdit->setReadOnly(true);
    this->setFixedSize(this->geometry().size());
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

void MainWindow::init()
{
    QString buttonQss = "QPushButton {"
                        "  background-color: #3498db;"
                        "  color: #ffffff;"
                        "  border: 1px solid #2980b9;"
                        "  border-radius: 10px;"
                        "  padding: 5px 10px;"
                        "  min-width: 50px;"
                        "  min-height: 20px;"
                        "}"
                        "QPushButton:hover {"
                        "  background-color: #2980b9;"
                        "}"
                        "QPushButton:pressed {"
                        "  background-color: #1f5a99;"
                        "}";

//    QString lineQss = "QLineEdit {"
//                      "background: #FDFDFD;"
//                      "selection-background-color: #8BF;"
//                      "border: 1px solid #999999;"
//                      "border-radius: 2px;"
//                      "border-style: inset;"
//                      "padding: 0 1px;"
//                      "}";

    QString textQss = "QTextEdit {"
                    "  border-radius: 5px;"
                    "  font-family: Arial;"
                    "  font-size: 14px;"
                    "}"
                    "QTextEdit:hover {"
                    "  border: 1px solid #3498db;"
                    "}"
                    "QTextEdit:focus {"
                    "  border: 1px solid #2980b9;"
                    "}";

    ui->sentenceButton->setStyleSheet(buttonQss);
    ui->wordButton->setStyleSheet(buttonQss);
    ui->lineEdit->setStyleSheet("QLineEdit{ background-color: rgba(255,240,255,120); }QLineEdit:focus{background-color: rgb(255,255,255)}");
    ui->textEdit->setStyleSheet(textQss);
}

