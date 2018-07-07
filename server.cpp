#include "server.h"
#include <QDebug>

#define ERROR_CODE_SERVER_DIDNT_START (-1)

Server::Server(QObject *parent) : QObject(parent),
    tcpServer(Q_NULLPTR),
    networkSession(0)
{

    setFortunes();
    QObject::connect(this, &Server::sig_serverLogReport, qobject_cast<ServerMainWindow*>(parent), &ServerMainWindow::slot_showServerLogMessage);
    QObject::connect(this, &Server::sig_serverErrorReport, qobject_cast<ServerMainWindow*>(parent), &ServerMainWindow::slot_showServerErrorMessage);
    QObject::connect(this, &Server::sig_serverInfoReport, qobject_cast<ServerMainWindow*>(parent), &ServerMainWindow::slot_showServerInfoMessage);

    emit sig_serverInfoReport("Starting to initialize the MunchkinServer");

    slot_serverInitializaion();
    connect(tcpServer, &QTcpServer::newConnection, this, &Server::slot_setUpNewConnection);
    connect(this, &Server::sig_sendFortune, this, &Server::slot_sendFortune);
}

void Server::slot_serverInitializaion()
{
    emit sig_serverLogReport("Entering server initialization...");
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

        emit sig_serverLogReport("Creating new network session...");
        networkSession = new QNetworkSession(config, this);
        connect(networkSession, &QNetworkSession::opened, this, &Server::slot_sessionOpened);

        emit sig_serverLogReport("Opening network session.");
        networkSession->open();
    }
    else
    {
        emit sig_serverInfoReport("Session is already opened! ");
        slot_sessionOpened();
    }
}

int Server::slot_sessionOpened()
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
    emit sig_serverLogReport("Creating new TCP Server... ");
    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen()) {
       emit sig_serverErrorReport(tr("Unable to start the Munchkin Server: %1.")
                                  .arg(tcpServer->errorString()));
       return ERROR_CODE_SERVER_DIDNT_START;
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
    emit sig_serverLogReport("If we did not find one, use IPv4 localhost... ");
    if (ipAddress.isEmpty())
    {
        qDebug() << "if ipAddress.isEmpty()...";
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    }
    emit sig_serverLogReport("Server is running!");
    emit sig_serverInfoReport(tr("The Munchkin Server is running on\n\nIP: %1\nport: %2\n\n"
                             "Run the Fortune Client example now.")
                          .arg(ipAddress).arg(tcpServer->serverPort()));
   //! [1]
   return 0;
}

//! [4]
void Server::slot_sendFortune(int socketDescriptor)
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
}

void Server::slot_setUpNewConnection()
{

    qDebug() << "Trying to establish connection...";
    emit sig_serverLogReport("Trying to establish connection...");
    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    _establishedConnections.push_back({clientConnection, ""});
    //each socket has unique descriptor.
    //Therefore I can identify it by the usage of it's descriptor.
    int ID = clientConnection->socketDescriptor();

    //connect the signal with the Specified slot.
    connect(clientConnection, &QIODevice::readyRead, [this, ID]{slot_readTheClientName(ID);});
    typedef void (QAbstractSocket::*QAbstractSocketErrorSignal)(QAbstractSocket::SocketError);
    connect(clientConnection, static_cast<QAbstractSocketErrorSignal>(&QAbstractSocket::error),
            this, &Server::slot_reportError);

    //set-up Data Sterams;
    QDataStream* _newStream = new QDataStream();

    _newStream->setDevice(clientConnection);
    _newStream->setVersion(QDataStream::Qt_4_0);

    _dataStreams.push_back({_newStream, clientConnection->socketDescriptor()});



}

void Server::slot_readTheClientName(int socketDescriptor)
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

     emit sig_serverLogReport(clientName);
     qDebug() << "Client Name: " << clientName;

     for (unsigned int var = 0; var < _establishedConnections.size(); ++var) {

         if (_establishedConnections[var].first->socketDescriptor() == socketDescriptor )
             _establishedConnections[var].second = clientName;

     }

     //find the connection
     emit sig_sendFortune(socketDescriptor);
}


void Server::slot_reportError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        emit sig_serverLogReport(tr("Munchkin Server: The host was not found. Please check the "
                                    "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        emit sig_serverLogReport(tr("Munchkin Server: The connection was refused by the peer. "
                                    "Make sure the fortune server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        break;
    default:
       emit sig_serverLogReport(tr("Munchkin Server: The following error occurred: %1.")
                             .arg((static_cast<QTcpSocket*>(QObject::sender()))->errorString()));
    }


}

void Server::setFortunes()
{
    fortunes << tr("You've been leading a dog's life. Stay off the furniture.")
             << tr("You've got to think about tomorrow.")
             << tr("You will be surprised by a loud noise.")
             << tr("You will feel hungry again in another hour.")
             << tr("You might have mail.")
             << tr("You cannot kill time without injuring eternity.")
             << tr("Computers are not intelligent. They only think they are.");
}
