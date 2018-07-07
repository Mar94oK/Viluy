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
}

void ServerMainWindow::slot_showServerLogMessage(QString message)
{
    ui->lbl_ServerLog->setText(message);
}

void ServerMainWindow::slot_showServerErrorMessage(QString message)
{
    ui->lbl_ServerErrorLog->setText(message);
}

void ServerMainWindow::slot_showServerInfoMessage(QString message)
{
    ui->lblServerInfo->setText(message);
}




ServerMainWindow::~ServerMainWindow()
{
    delete ui;
}
