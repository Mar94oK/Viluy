#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPushButton>

ServerMainWindow::ServerMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->lblServerInfo->setTextInteractionFlags(Qt::TextBrowserInteraction);

    QPushButton *quitButton = new QPushButton(tr("Quit"));
    quitButton->setAutoDefault(false);
    connect(ui->btnExit, &QAbstractButton::clicked, this, &QWidget::close);

    setWindowTitle(QGuiApplication::applicationDisplayName());

    ui->lbl_TotalRooms->setText("Комнат не создано.");

    SetUpSignalSlotsConnections();
}

void ServerMainWindow::slot_showServerLogMessage(QString message)
{
    ui->txtEdit_ServerLog->append(message);
}

void ServerMainWindow::slot_showServerErrorMessage(QString message)
{
    ui->lbl_ServerErrorLog->setText(message);
}

void ServerMainWindow::slot_showServerInfoMessage(QString message)
{
    ui->lblServerInfo->setText(message);
}

void ServerMainWindow::SlotReportNewRoomsQuantity(unsigned int quantity)
{
    if (quantity)
        ui->lbl_TotalRooms->setText(_roomsQuantityBaseText + QString::number(quantity));
    else
        ui->lbl_TotalRooms->setText("Комнат не создано.");
}

void ServerMainWindow::SlotUpdateStatistic(const QStringList &statistic)
{
    //NAY-001:MARK_EXPECTED_IMPROVEMENT to pass here no the whole list, but only the number of string and the value to be changed.
    QString newStatistic;
    foreach (QString string, statistic)
    {
       newStatistic += string + "\n";
    }
    ui->lblStatistics->setText(newStatistic);
}

void ServerMainWindow::SlotRemoveClientFromRoomsBrowser(const QString &clientName)
{
    ui->wdgt_RoomBrowser->SlotDeleteClientFromRoom(clientName);
}


ServerMainWindow::~ServerMainWindow()
{
    delete ui;
}

void ServerMainWindow::SetUpSignalSlotsConnections()
{
    QObject::connect(ui->btnEnterOpponent, &QPushButton::clicked, this, &ServerMainWindow::DebugSlotSendOpponentEnteringRoomReport);

    QObject::connect(this, &ServerMainWindow::SignalAddNewRoom, ui->wdgt_RoomBrowser, &RoomBrowser::SlotAddNewRoom);
    QObject::connect(this, &ServerMainWindow::SignalUpdateExistingRoom, ui->wdgt_RoomBrowser, &RoomBrowser::SlotUpdateExisting);
    QObject::connect(this, &ServerMainWindow::SignalDeleteRoom, ui->wdgt_RoomBrowser, &RoomBrowser::SlotDeleteRoom);


}
