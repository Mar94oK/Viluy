#ifndef UTILITES_H
#define UTILITES_H

#include <QObject>
#include <QTime>
#include <QCoreApplication>


class Utilites
{

public:

    static void delay_seconds(int seconds);
};

void Utilites::delay_seconds(int seconds)
{
    QTime dieTime= QTime::currentTime().addSecs(seconds);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}


#endif // UTILITES_H
