#include "mainwindow.h"

#include <QApplication>
#include <QTextCodec>


int main(int argc, char *argv[])
{
    qputenv("QT_QPA_FONTDIR", "\\usr\\share\\fonts\\truetype\\dejavu");
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));

    MainWindow w;
    w.show();
    return a.exec();
}
