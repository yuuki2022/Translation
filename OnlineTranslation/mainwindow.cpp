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
    this->setWindowTitle("Online Translation Platform");
    init();
    //define a client  自然语言模型
    client = new Client();
    client->initClient(ip, port);
    ui->textEdit->setReadOnly(true);
    this->setFixedSize(this->geometry().size());
    connect(ui->wordButton,&QPushButton::clicked,this,[=](){
        ui->textEdit->setText("");
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
        ui->textEdit->setText("");
        jsonObject["options"] = 0;  //2:zh-en 1:en-zh,0:sentence(en-zh) -1:sentence(zh-en)
        jsonObject["content"] = ui->lineEdit->text();

        QJsonDocument jsonDocument(jsonObject);
        QString jsonString = jsonDocument.toJson();
        qDebug() << jsonString << "\n";
        client->sendMessage(jsonString);
        //client->sendMessage(jsonString);
    });//click to send a word to its

    connect(ui->chSentenceButton,&QPushButton::clicked,this,[=](){
        QJsonObject jsonObject;
        ui->textEdit->setText("");
        jsonObject["options"] = -1;  //2:zh-en 1:en-zh,0:sentence(en-zh) -1:sentence(zh-en)
        jsonObject["content"] = ui->lineEdit->text();

        QJsonDocument jsonDocument(jsonObject);
        QString jsonString = jsonDocument.toJson();
        qDebug() << jsonString << "\n";
        client->sendMessage(jsonString);
        //client->sendMessage(jsonString);
    });//click to send a word to its
    connect(ui->chineseButton,&QPushButton::clicked,this,[=](){
        QJsonObject jsonObject;
        ui->textEdit->setText("");
        jsonObject["options"] = 2;  //2:zh-en 1:en-zh,0:sentence(en-zh) -1:sentence(zh-en)
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


    

}

MainWindow::~MainWindow()
{
    delete ui;
    
}

void MainWindow::init()
{
    QString buttonQss =   "QPushButton {"
                          "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #497BF0, stop:1 #1FB6F6);"
                          "    border: 2px solid #497BF0;"
                          "    border-radius: 5px;"
                          "    padding: 3px 2px;"
                          "    color: #ffffff;"
                          "}"
                          "QPushButton:hover {"
                          "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #1FB6F6, stop:1 #497BF0);"
                          "}"
                          "QPushButton:pressed {"
                          "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #0E56B3, stop:1 #1FB6F6);"
                          "}";

    QString textQss = "QTextEdit {"
                      "border: 1px solid #A9ACB8"
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
    ui->lineEdit->setStyleSheet(
            "QLineEdit {"
            "    border: 1px solid #A9ACB8;"
            "    border-radius: 5px;"
            "    padding: 3px;"
            "    background-color: transparent;"
            "    color: #000000;"
            "    selection-background-color: #3498db;"
            "    selection-color: #ffffff;"
            "}"
            "QLineEdit:hover {"
            "    border: 2px solid #2980b9;"
            "    background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #3498db, stop: 1 rgba(52, 152, 219, 100));"
            "}"
            "QLineEdit:focus {"
            "    border: 2px solid #1f5a99;"
            "    background-color: transparent;"
            "}"
        );
    ui->chSentenceButton->setStyleSheet(buttonQss);
    ui->chineseButton->setStyleSheet(buttonQss);

    ui->textEdit->setStyleSheet(textQss);
}

