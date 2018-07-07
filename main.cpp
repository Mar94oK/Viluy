#include "mainwindow.h"
#include <QApplication>
#include <QTime>

void delay()
{
    QTime dieTime= QTime::currentTime().addSecs(1);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ServerMainWindow w;

    w.show();

    Server serv(&w);
//    QObject::connect(&serv, &Server::sig_serverLogReport, &w, &ServerMainWindow::slot_showServerLogMessage);
//    QObject::connect(&serv, &Server::sig_serverErrorReport, &w, &ServerMainWindow::slot_showServerErrorMessage);
//    QObject::connect(&serv, &Server::sig_serverInfoReport, &w, &ServerMainWindow::slot_showServerInfoMessage);

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    return a.exec();
}

