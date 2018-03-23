#include "gpswidget.h"
#include <QApplication>
#include <QTextCodec> // Support for chinese
#pragma execution_character_set("utf-8")

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8")); // Support for Chinese
    GPSWidget w;
    w.show();

    return a.exec();
}
