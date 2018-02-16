#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ServerMainWindow w;

    w.show();

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    return a.exec();
}
