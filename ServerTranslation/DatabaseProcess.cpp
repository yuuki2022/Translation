#include "DatabaseProcess.h"
#include <QDebug>
bool DatabaseProcess::createDbConnection(const QString &filePath, const QString &connectionName)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(filePath); //todo: change it to right path
    if (!db.open()) {
        qDebug()<< "failed to open database!!" << db.lastError().text();
        return false;
    }
    qDebug() <<"createDbConnection: "<< "successed to open database!!!" << "\n";
    return true;
}

QString DatabaseProcess::getTargetWord(const QString &sourceWord)
{
    //1: English to Chinese
    //0: Chinese to English
    QSqlQuery query;
    QString result;


    QString command = QString("SELECT translation FROM stardict  "
                              "WHERE word = '%1' "
                              "LIMIT 1;")
            .arg(sourceWord);
    if(!query.exec(command)){
        qDebug()<<query.lastError().text().toUtf8();
    }else{
        qDebug()<<"Yes";
    }
    qDebug() << command << "\n";

    while (query.next()) {
        result = query.value(0).toString();
    }
    //query.exec(command);


    if(result.isEmpty()){
        result = "can't find the word";
    }


    return result;
}

DatabaseProcess::DatabaseProcess()
{

}
