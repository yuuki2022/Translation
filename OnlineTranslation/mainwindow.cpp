#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
const QString ip = "127.0.0.1";
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
        jsonObject["options"] = 0;  //0:word,1:sentence
        jsonObject["content"] = ui->lineEdit->text();

        QJsonDocument jsonDocument(jsonObject);
        QString jsonString = jsonDocument.toJson();
        qDebug() << jsonString << "\n";
        client->sendMessage(jsonString);
        //client->sendMessage(jsonString);
    });//click to send a word to its

    connect(client,&Client::response,this,[=](){
        ui->textEdit->setText(client->getResultMessage());
    });
}

MainWindow::~MainWindow()
{
    delete ui;

}

