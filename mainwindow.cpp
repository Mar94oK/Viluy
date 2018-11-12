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


ServerMainWindow::~ServerMainWindow()
{
    delete ui;
}

void ServerMainWindow::SetUpSignalSlotsConnections()
{
    QObject::connect(ui->btnEnterOpponent, &QPushButton::clicked, this, &ServerMainWindow::DebugSlotSendOpponentEnteringRoomReport);
}
