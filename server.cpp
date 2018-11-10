#include "server.h"
#include <QDebug>
#include <utilites.h>


#define USE_VISUAL_DELAYS
#undef USE_VISUAL_DELAYS
#define ERROR_CODE_SERVER_DIDNT_START (-1)

Server::Server(QObject *parent) : QObject(parent),
    tcpServer(Q_NULLPTR),
    networkSession(0)
{

    setFortunes();
    QObject::connect(this, &Server::SignalServerLogReport, qobject_cast<ServerMainWindow*>(parent), &ServerMainWindow::slot_showServerLogMessage);
    QObject::connect(this, &Server::sig_serverErrorReport, qobject_cast<ServerMainWindow*>(parent), &ServerMainWindow::slot_showServerErrorMessage);
    QObject::connect(this, &Server::sig_serverInfoReport, qobject_cast<ServerMainWindow*>(parent), &ServerMainWindow::slot_showServerInfoMessage);
    QObject::connect(this, &Server::SignalConnectionSendOutgoingData, this, &Server::SlotConnectionSendOutgoingData);

    emit sig_serverInfoReport("Starting to initialize the MunchkinServer");

    SlotServerInitializaion();
    connect(tcpServer, &QTcpServer::newConnection, this, &Server::SlotSetUpNewConnection);

    _settings.setServerName("TheBestMunchkinServerEver");
}

void Server::SlotServerInitializaion()
{

    emit SignalServerLogReport("Entering server initialization...");
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

        emit SignalServerLogReport("Creating new network session...");
        networkSession = new QNetworkSession(config, this);
        connect(networkSession, &QNetworkSession::opened, this, &Server::SlotSessionOpened);

        emit SignalServerLogReport("Opening network session.");
        networkSession->open();
    }
    else
    {
        emit sig_serverInfoReport("Session is already opened! ");
        SlotSessionOpened();
    }
}

void Server::MessagesParser(const QByteArray &data, int socketDescriptor)
{
    qDebug() << "NAY-0001: Parsing Message for socketDescriptor: " << socketDescriptor;

    serverMessageSystem::CommonHeader header;
    if(!header.ParseFromArray(data.data(), data.size()))
    {
       qDebug() << "NAY-0001: Error during protobuf message parsing! ";
       qDebug() << "NAY-001: Array size: array.size()";
       emit SignalServerLogReport("NAY-0001: Error during protobuf message parsing! ");
    }
    else
    {
       qDebug() << "NAY-0001: Header Parsed successfully! ";
       qDebug() << "NAY-001: Array size: " << data.size();

       switch (header.subsystem())
       {
            case serverMessageSystem::SubSystemID::CONNECTION_SUBSYSTEM:
            {
                switch (header.commandid()) {

                case serverMessageSystem::ConnectionSubSysCommandsID::SERVER_INPUT_QUERY_REQUEST:
                {
                    ProcessServerInputQueryRequest(data, socketDescriptor);
                }
                break;
                case serverMessageSystem::ConnectionSubSysCommandsID::CLIENT_ROOM_CREATION_REQUEST:
                {
                    qDebug() << "Processing CLIENT_ROOM_CREATION_REQUEST.";
                    ProcessClientRoomCreationRequest(data, socketDescriptor);
                }
                break;
                case serverMessageSystem::ConnectionSubSysCommandsID::CLIENT_CONNECTION_TO_ROOM_REQUEST:
                break;

                default:
                    emit SignalServerLogReport("NAY-0001: Unsupported Command in CONNECTION_SUBSYSTEM with CmdID: " + QString::number(header.commandid()));
                break;
           }
        }
           break;
       case serverMessageSystem::SubSystemID::GAME_ACTIONS_SUBSYSTEM:
           emit SignalServerLogReport("NAY-0001: Message SubSystem"
                                      " GAME_ACTIONS_SUBSYSTEM "
                                      " Not supported yet.");
           break;

       case serverMessageSystem::SubSystemID::GAME_NOTIFICATION_SUBSYSTEM:
           emit SignalServerLogReport("NAY-0001: Message SubSystem"
                                      " GAME_NOTIFICATION_SUBSYSTEM "
                                      " Not supported yet.");
           break;


       default:
           emit SignalServerLogReport("NAY-0001: Message SubSystem"
                                      " GAME_NOTIFICATION_SUBSYSTEM "
                                      " Not supported yet.");
           break;
       }
    }
}


QByteArray Server::FormServerInputQueryReply()
{
    serverMessageSystem::ServerQueryReply message;
    serverMessageSystem::CommonHeader *header = message.mutable_header();
    header->set_subsystem(serverMessageSystem::SubSystemID::CONNECTION_SUBSYSTEM);
    header->set_commandid(serverMessageSystem::ConnectionSubSysCommandsID::SERVER_INPUT_QUERY_REPLY);
    message.set_connectioncmdid(serverMessageSystem::ConnectionSubSysCommandsID::SERVER_INPUT_QUERY_REPLY);
    message.set_servername(_settings.serverName().toUtf8().constData());
    message.set_roomcreationallowed(_settings.roomCreationAllowed());
    message.set_connectiontoroomallowed(_settings.connectionToRoomAllowed());

    QByteArray block;
    block.resize(message.ByteSize());
    message.PrintDebugString();
    message.SerializeToArray(block.data(), block.size());

    qDebug() << "NAY-001: Serialized FormServerInputQueryReply is ready.";

    return block;
}

QByteArray Server::FormClientRoomCreationReply(bool created, unsigned int slotId, unsigned int freeSlotsLeft, RoomCreationErrors ErrorNumber)
{
    if (created)
    {
        serverMessageSystem::ClientRoomCreationReply message;
        serverMessageSystem::CommonHeader *header = message.mutable_header();
        header->set_subsystem(serverMessageSystem::SubSystemID::CONNECTION_SUBSYSTEM);
        header->set_commandid(serverMessageSystem::ConnectionSubSysCommandsID::CLIENT_CONNECTION_TO_ROOM_REPLY);
        message.set_connectioncmdid(serverMessageSystem::ConnectionSubSysCommandsID::CLIENT_CONNECTION_TO_ROOM_REPLY);
        message.set_connectionallowed(created);
        message.set_slotid(slotId);
        message.set_freeslotsleft(freeSlotsLeft);

        QByteArray block;
        block.resize(message.ByteSize());
        message.PrintDebugString();
        message.SerializeToArray(block.data(), block.size());

        qDebug() << "NAY-001: Serialized FromClientRoomCreationReply is ready.";

        return block;
    }
    else
    {
        serverMessageSystem::ClientRoomCreationReply message;
        serverMessageSystem::CommonHeader *header = message.mutable_header();
        header->set_subsystem(serverMessageSystem::SubSystemID::CONNECTION_SUBSYSTEM);
        header->set_commandid(serverMessageSystem::ConnectionSubSysCommandsID::CLIENT_CONNECTION_TO_ROOM_REPLY);
        message.set_connectioncmdid(serverMessageSystem::ConnectionSubSysCommandsID::CLIENT_CONNECTION_TO_ROOM_REPLY);
        message.set_connectionallowed(created);
        message.set_freeslotsleft(freeSlotsLeft);
        serverMessageSystem::RoomCreationErrors *errors = message.mutable_roomcreationerrors();

        switch (ErrorNumber)
        {
        case RoomCreationErrors::INCORRECT_SETTINGS:
            errors->set_incorrectsettings(true);
            break;
        case RoomCreationErrors::NO_FREE_SLOTS_AVAILABLE:
            errors->set_nofreeslotsavailable(true);
            break;

        case RoomCreationErrors::RULES_ARE_NOT_SUPPORTED:
            errors->set_rulesarenotsupported(true);
            break;
        default:
            break;
        }

        QByteArray block;
        block.resize(message.ByteSize());
        message.PrintDebugString();
        message.SerializeToArray(block.data(), block.size());

        qDebug() << "NAY-001: Serialized FromClientRoomCreationReply is ready. WITH ERRORS!";
        return block;
    }

}

Connection *Server::DefineConnection(int socketDescriptor)
{
    for (unsigned int var = 0; var < _establishedConnections.size(); ++var)
    {

        if (_establishedConnections[var]->socket()->socketDescriptor() == socketDescriptor )
        {
            //qDebug() << "NAY-001: Defining Connection";
            return _establishedConnections[var];
        }
    }
    qDebug() << "NAY-001: Error while connection searching!";
    return nullptr;
}

int Server::SlotSessionOpened()
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

    emit SignalServerLogReport("Creating new TCP Server... ");
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

    emit SignalServerLogReport("If we did not find one, use IPv4 localhost... ");
    if (ipAddress.isEmpty())
    {
        qDebug() << "if ipAddress.isEmpty()...";
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    }
    emit SignalServerLogReport("Server is running!");
    emit sig_serverInfoReport(tr("The Munchkin Server is running on\n\nIP: %1\nport: %2\n\n")
                          .arg(ipAddress).arg(tcpServer->serverPort()));
   //! [1]
   return 0;
}

void Server::SlotSetUpNewConnection()
{
    emit SignalServerLogReport("Trying to establish connection #" + QString::number(_establishedConnections.size() + 1));
    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    if (clientConnection != nullptr)
        _establishedConnections.push_back(new Connection(clientConnection, QString::number(clientConnection->socketDescriptor())));
    long long ID = clientConnection->socketDescriptor();

    //connect the signal with the Specified slot.
    connect(clientConnection, &QIODevice::readyRead, [this, ID]{SlotReadIncomingData(ID);});
    typedef void (QAbstractSocket::*QAbstractSocketErrorSignal)(QAbstractSocket::SocketError);
    connect(clientConnection, static_cast<QAbstractSocketErrorSignal>(&QAbstractSocket::error),
            this, &Server::slot_reportError);
    if (clientConnection != nullptr)
        emit SignalServerLogReport("Connection #" + QString::number(_establishedConnections.size())  + " established!");
    else
        emit SignalServerLogReport("Connection #" + QString::number(_establishedConnections.size() + 1)  + " ERROR!");
}

void Server::SlotReadIncomingData(int socketDescriptor)
{
     emit SignalServerLogReport("Trying to read the info...");
     QByteArray array;
     qDebug() << "NAY-0001: Before message parsing! ";
     Connection* currentConnection = DefineConnection(socketDescriptor);
     array = currentConnection->socket()->readAll();

     MessagesParser(array, socketDescriptor);
}

void Server::slot_reportError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        emit SignalServerLogReport(tr("Munchkin Server: The host was not found. Please check the "
                                    "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        emit SignalServerLogReport(tr("Munchkin Server: The connection was refused by the peer. "
                                    "Make sure the fortune server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        break;
    default:
       emit SignalServerLogReport(tr("Munchkin Server: The following error occurred: %1.")
                             .arg((static_cast<QTcpSocket*>(QObject::sender()))->errorString()));
    }
}

void Server::SlotConnectionSendOutgoingData(int socketDescriptor)
{
    Connection* connection = DefineConnection(socketDescriptor);

    if (connection->socket()->isOpen())
    {
        if (connection->socket()->ConnectedState == QTcpSocket::ConnectedState)
        {
             connection->socket()->write(connection->OutgoingDataBuffer());
             return;
        }
        else
        {
            qDebug() << "Error during data send. Socket is not closed, but in NOTConnected state! ";
//        NAY-001: MARK_EXPECTED_ERROR
            return;
        }
    }
    else
    {
        qDebug() << "Error during data send. Socket is closed! ";
//        NAY-001: MARK_EXPECTED_ERROR
        return;
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



void Server::ProcessServerInputQueryRequest(const QByteArray &data, int socketDescriptor)
{
    serverMessageSystem::ServerInputQuery message;

    if (!message.ParseFromArray(data.data(), data.size()))
    {
        emit SignalServerLogReport("NAY-001: Error while ProcessServerInputQueryRequest() ");
        return;
    }

    emit SignalServerLogReport("NAY-001: ServerInputQuery: ClientName: " + QString::fromStdString(message.clientname()));
    emit SignalServerLogReport("NAY-001: ServerInputQuery: OS Type: " + QString::fromStdString(message.ostype()));

    if (_settings.immediateReplyToQuery())
    {
        Connection* currentConnection = DefineConnection(socketDescriptor);
        currentConnection->setOutgoingDataBuffer(FormServerInputQueryReply());
        emit SignalServerLogReport("NAY-001: ServerInputQueryReply to socket #" + QString::number(socketDescriptor));
        emit SignalConnectionSendOutgoingData(socketDescriptor);
    }
}

void Server::ProcessClientRoomCreationRequest(const QByteArray &data, int socketDescriptor)
{
    serverMessageSystem::ClientRoomCreationRequest message;

    if (!message.ParseFromArray(data.data(), data.size()))
    {
        emit SignalServerLogReport("NAY-001: Error while ProcessClientRoomCreationRequest() ");
        return;
    }

    emit SignalServerLogReport("NAY-001: ServerInputQuery: ClientName: " + QString::fromStdString(message.clientname()));
    emit SignalServerLogReport("NAY-001: ServerInputQuery: Game Settings: GameType: HasAddon WildAxe: " + QString::number(message.gamesettings().gametype().hasaddonwildaxe()));
    emit SignalServerLogReport("NAY-001: ServerInputQuery: Game Settings: GameType: HasAddon ClericalErrors: " + QString::number(message.gamesettings().gametype().hasaddonclericalerrors()));
    emit SignalServerLogReport("NAY-001: ServerInputQuery: Game Settings: GameType: Rules Type: " + QString::number(static_cast<uint32_t>(message.gamesettings().gametype().rulestype())));
    emit SignalServerLogReport("NAY-001: ServerInputQuery: Game Settings: Time Settings: Diplomacy Time: " + QString::number(message.gamesettings().timesettings().diplomacytime()));
    emit SignalServerLogReport("NAY-001: ServerInputQuery: Game Settings: Time Settings: Time To Move: " + QString::number(message.gamesettings().timesettings().totaltimetomove()));
    emit SignalServerLogReport("NAY-001: ServerInputQuery: Game Settings: Time Settings: Time To Think: " + QString::number(message.gamesettings().timesettings().timetothink()));
    emit SignalServerLogReport("NAY-001: ServerInputQuery: Game Settings: Time Settings: Time For Opponents Decision: " + QString::number(message.gamesettings().timesettings().timeforopponentsdecision()));
    emit SignalServerLogReport("NAY-001: ServerInputQuery: Game Settings: Setting changes During preparation Allowance: " + QString::number(message.gamesettings().settingscorrectionallowed()));
    emit SignalServerLogReport("NAY-001: ServerInputQuery: Game Settings: Maximum number of players: " + QString::number(message.gamesettings().maximumnumberofplayers()));

    if (_rooms.size() < _settings.maxNumberOfRooms())
    {
        qDebug() << "NAY-001: Create new room!";
        GameSettings givenSettings(message.gamesettings().maximumnumberofplayers(),
                                   message.gamesettings().timesettings().totaltimetomove(),
                                   message.gamesettings().timesettings().timetothink(),
                                   message.gamesettings().timesettings().timeforopponentsdecision(),
                                   message.gamesettings().timesettings().diplomacytime(),
                                   message.gamesettings().gametype().hasaddonclericalerrors(),
                                   message.gamesettings().gametype().hasaddonwildaxe(),
                                   QString::fromStdString(message.clientname()),
                                   static_cast<RulesType>(message.gamesettings().gametype().rulestype()),
                                   message.gamesettings().settingscorrectionallowed());
//        explicit Room(uint32_t id, QString name, uint32_t numberOfPLayers, GameSettings settings, Player firstPlayer, Connection* firstConnection) :
//        _id(id), _name(name), _numberOfPlayers(numberOfPLayers), _gameSettings(settings)
        Room* newRoom = new Room(_rooms.size() + 1,
                                 "NewRoom",
                                 1,
                                 givenSettings,
                                 Player(QString::fromStdString(message.clientname())),
                                 DefineConnection(socketDescriptor)
                                 );

        _rooms.push_back(newRoom);

        Connection* currentConnection = DefineConnection(socketDescriptor);
        currentConnection->setOutgoingDataBuffer(FormClientRoomCreationReply(true, _rooms.size(), _settings.maxNumberOfRooms() - _rooms.size(), RoomCreationErrors::NO_ERRORS));
        emit SignalServerLogReport("NAY-001: ClientRoomCreationReply to socket #" + QString::number(socketDescriptor));
        emit SignalConnectionSendOutgoingData(socketDescriptor);
    }
    else //NO FREE SPACE AVAILABLE
    {
        Connection* currentConnection = DefineConnection(socketDescriptor);
        currentConnection->setOutgoingDataBuffer(FormClientRoomCreationReply(false, 0, 0, RoomCreationErrors::NO_FREE_SLOTS_AVAILABLE));
        emit SignalServerLogReport("NAY-001: ClientRoomCreationReply WITH ERRORS to socket #" + QString::number(socketDescriptor));
        emit SignalConnectionSendOutgoingData(socketDescriptor);
        ;
    }


}
