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
#include <QStringList>

#include <server.h>
#include <roombrowser.h>
#include <roomparameters.h>

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

    void SignalAddNewRoom(const Room& room);
    void SignalUpdateExistingRoom(const Room& room);
    void SignalDeleteRoom(uint32_t id);

public slots:

    void slot_showServerLogMessage(QString message);
    void slot_showServerErrorMessage(QString message);
    void slot_showServerInfoMessage(QString message);
    void SlotReportNewRoomsQuantity(unsigned int quantity);
    void DebugSlotSendOpponentEnteringRoomReport() { emit DebugSignalOpponentEnteringRoomReport(1); }
    void SlotUpdateStatistic(const QStringList& statistic);

    void SlotAddNewRoom(const Room& room)
    { emit SignalAddNewRoom(room); }
    void SlotUpdateExisitngRoom(const Room& room)
    { emit SignalUpdateExistingRoom(room); }
    void SlotDeleteRoom(uint32_t id)
    { emit SignalDeleteRoom(id); }

private:        

    QString _roomsQuantityBaseText = "Создано комнат на сервере: ";

};

#endif // MAINWINDOW_H
