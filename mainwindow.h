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

    void SetUpSignalSlotsConnections();


private slots:


signals:

    void DebugSignalOpponentEnteringRoomReport(unsigned int roomId);

public slots:

    void slot_showServerLogMessage(QString message);
    void slot_showServerErrorMessage(QString message);
    void slot_showServerInfoMessage(QString message);
    void SlotReportNewRoomsQuantity(unsigned int quantity);
    void DebugSlotSendOpponentEnteringRoomReport() { emit DebugSignalOpponentEnteringRoomReport(1); }

private:        

    QString _roomsQuantityBaseText = "Создано комнат на сервере: ";

};

#endif // MAINWINDOW_H
