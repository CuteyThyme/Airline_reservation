#include "homewindow.h"
#include <QApplication>
#include<QTextCodec>
#include<QDate>
#include "flights.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HomeWindow w;
    w.show();

    return a.exec();
}
