#include "mainwindow.h"
#include "ui_mainwindow.h"

ServerMainWindow::ServerMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    tcpServer(Q_NULLPTR),
    networkSession(0)
{
    ui->setupUi(this);

//Now to make the initial Interface.


    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->lblServerInfo->setTextInteractionFlags(Qt::TextBrowserInteraction);

    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        // Get saved network configuration
        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) !=
            QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }

        networkSession = new QNetworkSession(config, this);
        connect(networkSession, &QNetworkSession::opened, this, &ServerMainWindow::sessionOpened);

        ui->lblServerInfo->setText(tr("Opening network session."));
        networkSession->open();
    } else {
        sessionOpened();
    }

    //! [2]
        fortunes << tr("You've been leading a dog's life. Stay off the furniture.")
                 << tr("You've got to think about tomorrow.")
                 << tr("You will be surprised by a loud noise.")
                 << tr("You will feel hungry again in another hour.")
                 << tr("You might have mail.")
                 << tr("You cannot kill time without injuring eternity.")
                 << tr("Computers are not intelligent. They only think they are.");
    //! [2]
        QPushButton *quitButton = new QPushButton(tr("Quit"));
        quitButton->setAutoDefault(false);
        connect(ui->btnExit, &QAbstractButton::clicked, this, &QWidget::close);
    //! [3]
        connect(tcpServer, &QTcpServer::newConnection, this, &ServerMainWindow::setUpNewConnection);
        connect(this, &ServerMainWindow::signal_sendFortune, this, &ServerMainWindow::sendFortune);

        setWindowTitle(QGuiApplication::applicationDisplayName());

}


void ServerMainWindow::sessionOpened()
{
    // Save the used configuration
    if (networkSession) {
        QNetworkConfiguration config = networkSession->configuration();
        QString id;
        if (config.type() == QNetworkConfiguration::UserChoice)
            id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
        else
            id = config.identifier();

        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
        settings.endGroup();
    }

//! [0] //! [1]
    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen()) {
        QMessageBox::critical(this, tr("Fortune Server"),
                              tr("Unable to start the server: %1.")
                              .arg(tcpServer->errorString()));
        close();
        return;
    }
//! [0]
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    ui->lblServerInfo->setText(tr("The server is running on\n\nIP: %1\nport: %2\n\n"
                            "Run the Fortune Client example now.")
                         .arg(ipAddress).arg(tcpServer->serverPort()));
//! [1]
}

//! [4]
void ServerMainWindow::sendFortune(int socketDescriptor)
{
//! [5]
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);

    out << fortunes.at(qrand() % fortunes.size());
//! [4] //! [7]

    QTcpSocket *clientConnection = nullptr;
    for (unsigned int var = 0; var < _establishedConnections.size(); ++var) {

        if (_establishedConnections[var].first->socketDescriptor() == socketDescriptor )
            clientConnection = _establishedConnections[var].first;

    }


    clientConnection->write(block);
    //clientConnection->disconnectFromHost();
    //! [5]
}

void ServerMainWindow::setUpNewConnection()
{

    qDebug() << "Trying to establish connection...";
    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    _establishedConnections.push_back({clientConnection, ""});
    //each socket has unique descriptor.
    //Therefore I can identify it by the usage of it's descriptor.
    int ID = clientConnection->socketDescriptor();

    //connect the signal with the Specified slot.
    connect(clientConnection, &QIODevice::readyRead, [this, ID]{readTheClientName(ID);});
//! [2] //! [4]
    typedef void (QAbstractSocket::*QAbstractSocketErrorSignal)(QAbstractSocket::SocketError);
    connect(clientConnection, static_cast<QAbstractSocketErrorSignal>(&QAbstractSocket::error),
//! [3]
            this, &ServerMainWindow::displayError);
//! [4]
//!
    //set-up Data Sterams;
    QDataStream* _newStream = new QDataStream();

    _newStream->setDevice(clientConnection);
    _newStream->setVersion(QDataStream::Qt_4_0);

    _dataStreams.push_back({_newStream, clientConnection->socketDescriptor()});



}

void ServerMainWindow::readTheClientName(int socketDescriptor)
{
     qDebug() << "Trying to read the info...";
     QDataStream* in = nullptr;
     for (unsigned int var = 0; var < _dataStreams.size(); ++var) {
         if (_dataStreams[var].second == socketDescriptor )
             in = _dataStreams[var].first;
     }


     in->startTransaction();
     QString clientName;

     *in >> clientName;

     if (!in->commitTransaction())
         return;

     QLabel* newConnectionName = new QLabel(this);

     newConnectionName->setText(clientName);
     ui->mainLayout->addWidget(newConnectionName);

     for (unsigned int var = 0; var < _establishedConnections.size(); ++var) {

         if (_establishedConnections[var].first->socketDescriptor() == socketDescriptor )
             _establishedConnections[var].second = clientName;

     }

     //find the connection
     emit signal_sendFortune(socketDescriptor);






}
//! [8]


void ServerMainWindow::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, tr("Fortune Server"),
                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("Fortune Server"),
                                 tr("The connection was refused by the peer. "
                                    "Make sure the fortune server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        break;
    default:

        QMessageBox::information(this, tr("Fortune Server"),
                                 tr("The following error occurred: %1.")
                                 .arg((static_cast<QTcpSocket*>(QObject::sender()))->errorString()));
    }


}




ServerMainWindow::~ServerMainWindow()
{
    delete ui;
}
