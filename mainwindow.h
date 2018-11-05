#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QLayout>
#include <QtWidgets>
#include <QtNetwork>
#include <QString>
#include <stdlib.h>
#include <QNetworkSession>
#include <QTcpServer>
#include <QMainWindow>

#include <server.h>

namespace Ui {
class MainWindow;
}

class ServerMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ServerMainWindow(QWidget *parent = 0);
    ~ServerMainWindow();

private:
    Ui::MainWindow *ui;

private:


private slots:


signals:


public slots:

    void slot_showServerLogMessage(QString message);
    void slot_showServerErrorMessage(QString message);
    void slot_showServerInfoMessage(QString message);

private:

};

#endif // MAINWINDOW_H
