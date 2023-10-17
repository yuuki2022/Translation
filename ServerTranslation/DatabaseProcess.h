#ifndef DATABASEPROCESS_H
#define DATABASEPROCESS_H
#include <QObject>
#include <QStringList>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QTime>
#include <QString>

class DatabaseProcess:QObject
{
    Q_OBJECT
private:
    QSqlDatabase db;
public:
    static QStringList getAllDrivers();
    static QString createDbNameByTime();
    static QTime getTimeFromMsec(const int msec);

    bool createDbConnection(const QString& filePath,const QString& connectionName="qt_sql_default_connection");
    QString getTargetWord(const QString& sourceWord);
    bool insertWord(const QString& word);
    bool updateWord(const QString& word);
    QString getChineseWord(const QString& word);
    DatabaseProcess();
};

#endif // DATABASEPROCESS_H
