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
    QObject::connect(this, &Server::SignalUpdateRoomsQuantity, qobject_cast<ServerMainWindow*>(parent), &ServerMainWindow::SlotReportNewRoomsQuantity);
    QObject::connect(this, &Server::SignalConnectionSendOutgoingData, this, &Server::SlotConnectionSendOutgoingData);
    QObject::connect(qobject_cast<ServerMainWindow*>(parent), &ServerMainWindow::DebugSignalOpponentEnteringRoomReport, this, &Server::DebugSlotSendReportsOpponentIsEnteringRoom);
    QObject::connect(this, &Server::SignalUpdateStatistics, qobject_cast<ServerMainWindow*>(parent), &ServerMainWindow::SlotUpdateStatistic);


    emit sig_serverInfoReport("Starting to initialize the MunchkinServer");

    SlotServerInitializaion();
    connect(tcpServer, &QTcpServer::newConnection, this, &Server::SlotSetUpNewConnection);

    _settings.setServerName("TheBestMunchkinServerEver");

    UpdateStatistics();
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

    serverMessageSystem::DefaultMessage defaultMessage;


    if(!defaultMessage.ParseFromArray(data.data(), data.size()))
    {
       qDebug() << "NAY-0001: Error during protobuf message parsing! ";
       qDebug() << "NAY-001: Array size: array.size()";
       emit SignalServerLogReport("NAY-0001: Error during protobuf message parsing! ");
    }
    else
    {
       qDebug() << "NAY-0001: Header Parsed successfully! ";
       qDebug() << "NAY-001: Array size: " << data.size();
       qDebug() << "Subsystem: " << QString::number(defaultMessage.header().subsystem());
       //defaultMessage.PrintDebugString();
       //uint32_t subSys = header.GetDescriptor()->FindFieldByNumber(2)->number();
       //qDebug() << "Subsystem defined by Descriptor: " << subSys;
       switch (defaultMessage.header().subsystem())
       {
            case serverMessageSystem::SubSystemID::CONNECTION_SUBSYSTEM:
            {
                switch (defaultMessage.header().commandid()) {

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
                    qDebug() << "Processing CLIENT_CONNECTION_TO_ROOM_REQUEST.";
                    ProcessClientConnectionToRoomRequest(data, socketDescriptor);
                break;

                default:
                    emit SignalServerLogReport("NAY-0001: Unsupported Command in CONNECTION_SUBSYSTEM with CmdID: " + QString::number(defaultMessage.header().commandid()));
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

       case serverMessageSystem::SubSystemID::CHART_SUBSYSTEM:
           switch (defaultMessage.header().commandid())
           {
           case serverMessageSystem::ChartSubSysCommandsID::CHART_MESSAGE:
               ProcessChartMessage(data, socketDescriptor);
               break;
           case serverMessageSystem::ChartSubSysCommandsID::CHART_NOTIFICATION:
               qDebug() << ("NAY-0001: Unsupported Command in CHART_SUBSYSTEM with CmdID: " + QString::number(defaultMessage.header().commandid()));
               break;
           default:
               qDebug() << ("NAY-0001: Unsupported Command in CHART_SUBSYSTEM with CmdID: " + QString::number(defaultMessage.header().commandid()));
               break;
           }
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
        header->set_commandid(serverMessageSystem::ConnectionSubSysCommandsID::CLIENT_ROOM_CREATION_REPLY);
        message.set_connectioncmdid(serverMessageSystem::ConnectionSubSysCommandsID::CLIENT_ROOM_CREATION_REPLY);
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
        header->set_commandid(serverMessageSystem::ConnectionSubSysCommandsID::CLIENT_ROOM_CREATION_REPLY);
        message.set_connectioncmdid(serverMessageSystem::ConnectionSubSysCommandsID::CLIENT_ROOM_CREATION_REPLY);
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
            errors->set_incorrectsettings(false);
            errors->set_nofreeslotsavailable(false);
            errors->set_rulesarenotsupported(false);
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

QByteArray Server::FromServerReportsOpponentIsEnteringRoom(const QString &opponentName, uint32_t roomId)
{
    serverMessageSystem::ServerReportsOpponentIsEnteringRoom message;
    serverMessageSystem::CommonHeader *header = message.mutable_header();
    header->set_subsystem(serverMessageSystem::SubSystemID::CONNECTION_SUBSYSTEM);
    header->set_commandid(serverMessageSystem::ConnectionSubSysCommandsID::SERVER_REPORTS_OPPONENT_IS_ENTERING_ROOM);
    message.set_connectioncmdid(serverMessageSystem::ConnectionSubSysCommandsID::SERVER_REPORTS_OPPONENT_IS_ENTERING_ROOM);
    message.set_opponentname(opponentName.toUtf8().constData());
    message.set_roomid(roomId);

    QByteArray block;
    block.resize(message.ByteSize());
    message.PrintDebugString();
    message.SerializeToArray(block.data(), block.size());

    qDebug() << "NAY-001: Serialized FromServerReportsOpponentIsEnteringRoom is ready.";


    return block;
}

QByteArray Server::FormChartMessage(const QString &textMessage, const QString &sender, uint32_t roomID)
{
    serverMessageSystem::ChartMessage message;
    serverMessageSystem::CommonHeader *header(message.mutable_header());
    header->set_subsystem(serverMessageSystem::SubSystemID::CHART_SUBSYSTEM);
    header->set_commandid(static_cast<uint32_t>(serverMessageSystem::ChartSubSysCommandsID::CHART_MESSAGE));
    message.set_chartcmdid(serverMessageSystem::ChartSubSysCommandsID::CHART_MESSAGE);
    message.set_sendername(sender.toUtf8().constData());
    message.set_chartmessage(textMessage.toUtf8().constData());

    //NAY-001: WHERE TO SAVE THIS DATA?
    //De-facto, now it is needed only by the server.
    message.set_roomid(roomID);

    QByteArray block;
    block.resize(message.ByteSize());
    message.SerializeToArray(block.data(), block.size());
    qDebug() << "NAY-001: Serialized FormChartMessage is ready.";
    return block;
}

QByteArray Server::FormServerRoomChangesInSelectableList(uint32_t roomId, bool deleteUpdateFlag)
{
    serverMessageSystem::ServerRoomChangesInSelectableList message;
    serverMessageSystem::CommonHeader *header(message.mutable_header());
    header->set_subsystem(serverMessageSystem::SubSystemID::CONNECTION_SUBSYSTEM);
    header->set_commandid(static_cast<uint32_t>(serverMessageSystem::ConnectionSubSysCommandsID::SERVER_ROOM_CHANGES_IN_SELECTABLE_LIST));
    message.set_connectioncmdid(serverMessageSystem::ConnectionSubSysCommandsID::SERVER_ROOM_CHANGES_IN_SELECTABLE_LIST);

    message.set_deletedorupdateflag(deleteUpdateFlag);

    if (deleteUpdateFlag)
    {
        serverMessageSystem::CreatedRoom *room = message.mutable_room();
        Room* givenRoom = DefineRoom(roomId);
        room->set_roomname(givenRoom->name().toUtf8().constData());
        room->set_maximumnumberofplayers(givenRoom->gameSettings().maximumNumberOfPlayers());
        room->set_roomid(givenRoom->id());
        room->set_players(givenRoom->numberOfPlayers());
    }
    else
    {
        serverMessageSystem::CreatedRoom *room = message.mutable_room();
        Room* givenRoom = DefineRoom(roomId);
        room->set_roomid(givenRoom->id());
    }

    QByteArray block;
    block.resize(message.ByteSize());
    message.SerializeToArray(block.data(), block.size());
    qDebug() << "NAY-001: Serialized FormServerRoomChangesInSelectableList is ready.";
    return block;

}

QByteArray Server::FormClientConnectionToRoomReply(bool noRoomsAvailable, uint32_t freeSlotLeft, const std::vector<uint32_t> &roomIDs, uint32_t queryOrder)
{
    serverMessageSystem::ClientConnectionToRoomReply message;
    serverMessageSystem::CommonHeader *header(message.mutable_header());
    header->set_subsystem(serverMessageSystem::SubSystemID::CONNECTION_SUBSYSTEM);
    header->set_commandid(static_cast<uint32_t>(serverMessageSystem::ConnectionSubSysCommandsID::CLIENT_CONNECTION_TO_ROOM_REPLY));
    message.set_connectioncmdid(serverMessageSystem::ConnectionSubSysCommandsID::CLIENT_CONNECTION_TO_ROOM_REPLY);

    message.set_noroomsavailable(noRoomsAvailable);
    message.set_freeslotsleft(freeSlotLeft);


    message.set_querysize(_query.size());
    message.set_queryorder(queryOrder);

    //Теперь надо проконтролировать, что везде комнаты создаются и обслуживаются правильно - т.е. в комнате надо изменять число игроков, вызывать для
    //них соответсвующие методы и т.д.
    for (uint32_t var = 0; var < _rooms.size(); ++var)
    {
        message.add_room();
        serverMessageSystem::CreatedRoom *room = message.mutable_room(var);
        room->set_players(_rooms[var]->numberOfPlayers());
        room->set_roomid(_rooms[var]->id());
        room->set_maximumnumberofplayers(_rooms[var]->gameSettings().maximumNumberOfPlayers());
        room->set_roomname(_rooms[var]->name().toUtf8().constData());
    }

    QByteArray block;
    block.resize(message.ByteSize());
    message.SerializeToArray(block.data(), block.size());
    qDebug() << "NAY-001: Serialized FormClientConnectionToRoomReply is ready.";
    return block;
}

Connection *Server::DefineConnection(int socketDescriptor)
{
    qDebug() << "NAY-001: Established Conenctions Size: " <<  _establishedConnections.size();
    for (unsigned int var = 0; var < _establishedConnections.size(); ++var)
    {
        qDebug() << "NAY-001: _establishedConnections[var]->socket()->socketDescriptor(): " << _establishedConnections[var]->socket()->socketDescriptor();
        if (_establishedConnections[var]->socket()->socketDescriptor() == socketDescriptor )
        {
            //qDebug() << "NAY-001: Defining Connection";
            return _establishedConnections[var];
        }
    }
    qDebug() << "NAY-001: Error while connection searching!";
    return nullptr;
}

bool Server::RemoveConnectionFromRoom(int socketDescriptor)
{
    foreach (Room* room, _rooms)
    {
        if (room->RemoveConnection(socketDescriptor))
        {
            emit SignalServerLogReport("Connection with socket descriptor: " + QString::number(socketDescriptor)
                                       + " has been successfully deleted from room with id: " + QString::number(room->id()));
            if (room->RoomIsNotEmpty())
            {
                 emit SignalServerLogReport("Reassigning master for room with id: " + QString::number(room->id())
                                            + " to Socket with id: " + QString::number(room->ReassignedRoomMaster()));
                 emit SignalServerLogReport("There are : " + QString::number(room->PlayersLeft())
                                            + " players left in room with id:  " + QString::number(room->id()));
                 return true;
            }
            else
            {
                emit SignalServerLogReport("Room with ID : " + QString::number(room->id())
                                           + "is Empty. To be deleted.");

                //    NAY-001: MARK_EXPECTED_ERROR
                //Here should be sure that id is still valid.
                 SendRoomDeletedMessageToQuery(room->id());
                 uint32_t id = room->id();
                if (RoomDeleting(room->id()))
                {
                    ++_closedRooms;
                    --_roomsArePreparingToGame;
                    UpdateStatistics();
                    emit SignalServerLogReport("Room with ID : " + QString::number(id)
                                               + " has been deleted successfully!");
                    return true;
                }
                else
                {
                    emit SignalServerLogReport("Error occured while deleting room with id:  : " + QString::number(id));
                    return false;
                }
            }
        }
        emit SignalServerLogReport("RemoveConnectionFromRoom() room having socket with SokcetDescriptor: " + QString::number(socketDescriptor)
                                   + " not found!");
        return false;
    }
    return false;
}

bool Server::RemoveConnectionFromQuery(int socketDescriptor)
{
    for (uint32_t var = 0; var < _query.size(); ++var)
    {
        if (_query[var]->socket()->socketDescriptor() == socketDescriptor)
        {

            //It has been an error!!! =))))
            //Not so good idea to use raw pointers, nontheless, don't spend much time
            // I should work more on GameMechanics sooner. At least, this week (19.11.2018 today)
            // So, nevermind.There is the only place where I do remove connections. I know the
            // order of removing.
            //            if (_query[var] != nullptr)
            //                delete _query[var];
            //        NAY-001: MARK_EXPECTED_ERROR
            //When I delete here the Connection* since it is the same like in all the other places, an erro may appear;
            _query.erase(_query.begin() + var);
            emit SignalServerLogReport("NAY-001: Disconnected socket with ID (in query) " + QString::number(var)
                                       + " has been successfully deleted! ");
            return true;
        }
    }
    qDebug() << "NAY-001:: Error! RemoveConnectionFromQuery() Connection with such SocketDesriptor not found." << socketDescriptor;
    return false;
}

bool Server::RoomDeleting(uint32_t roomId)
{
    qDebug() << "NAY-001: Entering room deleting...";
    for (unsigned int var = 0; var < _rooms.size(); ++var)
    {
        if (_rooms[var]->id() == roomId)
        {
            //deleting instance
            delete _rooms[var];
            //erase nullpointer from the array.
            _rooms.erase(_rooms.begin() + var);
            emit SignalUpdateRoomsQuantity(_rooms.size());
            return true;
        }
    }
    qDebug() << "NAY-001:: Error! RoomDeleting() Room with such ID not found.";
    return false;
}

Room *Server::DefineRoom(uint32_t roomId)
{
    for (unsigned int var = 0; var < _rooms.size(); ++var)
    {
        if (_rooms[var]->id() == roomId)
            return _rooms[var];
    }
    qDebug() << "NAY-001: Room with ID: " << roomId << " not found!";
    return nullptr;
}

bool Server::QueryOverSize(uint32_t givenSize)
{
    qDebug() << "NAY-001: Maximum Query size: "<< _settings.maxNumberOfRooms() * (6-1);
    if (givenSize > _settings.maxNumberOfRooms() * (6-1))
        return true;
     return  false;

}

void Server::UpdateStatistics()
{

    QStringList statistic;
    statistic.push_back(_roomsCreatedBaseText + QString::number(_roomsCreatedDuringSession));
    statistic.push_back(_connectionsCreatedBaseText + QString::number(_connectionsDuringSession));
    statistic.push_back(_gamesStartedCreatedBaseText + QString::number(_gamesStartedDuringSession));
    statistic.push_back(_roomsArePreparingToGameCreatedBaseText + QString::number(_roomsArePreparingToGame));
    statistic.push_back(_roomsAreActiveBaseText + QString::number(_roomArePLaying));
    statistic.push_back(_closedRoomsBaseText + QString::number(_closedRooms));
    statistic.push_back(_activeConnectionsBaseText + QString::number(_activeConnections));
    statistic.push_back(_maximumSimultaneousConnectionsdBaseText + QString::number(_maximumSimultaneousConnections));
    statistic.push_back(_querySizeBaseText + QString::number(_querySize));


    SignalUpdateStatistics(statistic);
}

void Server::SendRoomDeletedMessageToQuery(uint32_t roomID)
{
    foreach (Connection* connection, _query)
    {
        connection->setOutgoingDataBuffer(FormServerRoomChangesInSelectableList(roomID, false));
        emit SignalServerLogReport("NAY-001: Report Deleting Room to Query. Socket:" + QString::number(connection->socket()->socketDescriptor()));
        emit SignalConnectionSendOutgoingData(connection->socket()->socketDescriptor());
    }
}

void Server::SendRoomAddedMessageToQuery(uint32_t roomID)
{
    foreach (Connection* connection, _query)
    {
        connection->setOutgoingDataBuffer(FormServerRoomChangesInSelectableList(roomID, true));
        emit SignalServerLogReport("NAY-001: Report Adding Room to Query. Socket:" + QString::number(connection->socket()->socketDescriptor()));
        emit SignalConnectionSendOutgoingData(connection->socket()->socketDescriptor());
    }
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
    {
        _establishedConnections.push_back(new Connection(clientConnection, QString::number(clientConnection->socketDescriptor())));
        ++_connectionsDuringSession;
        ++_activeConnections;
        if (_maximumSimultaneousConnections < _activeConnections)
            ++_maximumSimultaneousConnections;
        UpdateStatistics();
        emit SignalServerLogReport("Connection #" + QString::number(_establishedConnections.size())  + " established!");

    }
    else
    {
        emit SignalServerLogReport("Error while establishing connection #" + QString::number(_establishedConnections.size() + 1));
        return;
    }

   long long ID = clientConnection->socketDescriptor();

    //connect the signal with the Specified slot.
    connect(clientConnection, &QIODevice::readyRead, [this, ID]{SlotReadIncomingData(ID);});
    //connect(clientConnection, &QTcpSocket::aboutToClose, [this, ID] {SlotClientConnectionIsClosing(ID);});
    connect(clientConnection, &QAbstractSocket::disconnected, [this, ID] {SlotClientConnectionIsClosing(ID);});

    typedef void (QAbstractSocket::*QAbstractSocketErrorSignal)(QAbstractSocket::SocketError);
    connect(clientConnection, static_cast<QAbstractSocketErrorSignal>(&QAbstractSocket::error),
            this, &Server::slot_reportError);
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

void Server::SlotClientConnectionIsClosing(long long ID)
{
    qDebug() << "NAY-001: Server found socket which is about to close! with ID: " << ID;
    Connection* connection = DefineConnection(CLOSED_SOCKET_DESCRIPTOR);
    if (connection == nullptr)
        return;
    //check socket state:
    switch (connection->socket()->state())
    {
    case QAbstractSocket::SocketState::ClosingState:
        qDebug() << "Socket is in the Closing State";
        break;
    case QAbstractSocket::SocketState::ConnectedState:
        qDebug() << "Socket is in the Connected State!";
        break;
    case QAbstractSocket::SocketState::UnconnectedState:
        qDebug() << "Socket is in the UnconnectedState State!";
        break;
    case QAbstractSocket::SocketState::HostLookupState:
        qDebug() << "Socket is in the HostLookupState State!";
        break;
    case QAbstractSocket::SocketState::BoundState:
        qDebug() << "Socket is in the BoundState State!";
        break;
    case QAbstractSocket::SocketState::ListeningState:
        qDebug() << "Socket is in the ListeningState State!";
        break;
    case QAbstractSocket::SocketState::ConnectingState:
        qDebug() << "Socket is in the ConnectingState State!";
        break;
    }

    if (connection->socket()->state() != QAbstractSocket::SocketState::ConnectedState)
    {
        qDebug() << "NAY-001: Processing exclusion of unconnected socket.";


        //Delete from Query:
        qDebug() << "NAY-001: Satring remove from query..";
        if (RemoveConnectionFromQuery(CLOSED_SOCKET_DESCRIPTOR))
        {
            --_querySize;
            UpdateStatistics();
            emit SignalServerLogReport("NAY-001: Disconnected socket with ID (in query) " + QString::number(connection->socket()->socketDescriptor())
                                       + " has been successfully deleted! ");
        }
        else
        {
            emit SignalServerLogReport("NAY-001: Error while Removing Connection: " + QString::number(CLOSED_SOCKET_DESCRIPTOR)
                                       + "  from Query.");
        }
        //Delete from Rooms:
        if (RemoveConnectionFromRoom(CLOSED_SOCKET_DESCRIPTOR))
        {
            emit SignalServerLogReport("NAY-001: Disconnected socket with ID (in room) " + QString::number(connection->socket()->socketDescriptor())
                                       + " has been successfully deleted! ");
        }
        else
        {
            emit SignalServerLogReport("NAY-001: Error while Removing Connection with scoketId: " + QString::number(CLOSED_SOCKET_DESCRIPTOR)
                                       + " from Room.");
        }

        //Delete from Established Connections (Only here is actual deleting process!):
        for (unsigned int var = 0; var < _establishedConnections.size(); ++var)
        {
            if (_establishedConnections[var]->socket()->socketDescriptor() == CLOSED_SOCKET_DESCRIPTOR)
            {
                delete _establishedConnections[var];
                //should also delete here from queue
                _establishedConnections.erase(_establishedConnections.begin() + var);
                --_activeConnections;
                UpdateStatistics();
                emit SignalServerLogReport("NAY-001: Disconnected socket with ID (in pull) " + QString::number(var)
                                           + " has been successfully deleted! ");
            }
        }
    }

}

void Server::DebugSlotSendReportsOpponentIsEnteringRoom(uint32_t roomId)
{
    qDebug() << "NAY-001: Sending DEBUG Client Entering Report for room wit ID: " << roomId;
    Room* room = DefineRoom(roomId);

    if (room == nullptr)
    {
        emit SignalServerLogReport("NAY-001: Cannot send ReportsOpponentIsEnteringRoom to the room with id: " + QString::number(roomId)
                                   + " Room not found!");
        return;
    }

    QString opponentName = "TheVeryFirstOpponent";

    (room->connections())[MASTER_CONNECTION_ID]->setOutgoingDataBuffer(FromServerReportsOpponentIsEnteringRoom(opponentName, roomId));
    unsigned int socketDescriptor = (room->connections())[MASTER_CONNECTION_ID]->socket()->socketDescriptor();
    emit SignalConnectionSendOutgoingData(socketDescriptor);
    emit SignalServerLogReport("NAY-001: DebugSlotSendReportsOpponentIsEnteringRoom to socket #" + QString::number(socketDescriptor));

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

        //ROOM_ID_DEFINITION

        Room* newRoom = new Room(_rooms.size() + 1,
                                 QString::fromStdString(message.roomname()),
                                 1,
                                 givenSettings,
                                 Player(QString::fromStdString(message.clientname())),
                                 DefineConnection(socketDescriptor)
                                 );

        _rooms.push_back(newRoom);
        emit SignalServerLogReport("NAY-001: Creted room with ID: " + QString::number(_rooms.back()->id()));
        ++_roomsArePreparingToGame;
        ++_roomsCreatedDuringSession;
        UpdateStatistics();
        emit SignalUpdateRoomsQuantity(_rooms.size());

        Connection* currentConnection = DefineConnection(socketDescriptor);
        currentConnection->setOutgoingDataBuffer(FormClientRoomCreationReply(true, _rooms.size(), _settings.maxNumberOfRooms() - _rooms.size(), RoomCreationErrors::NO_ERRORS));
        emit SignalServerLogReport("NAY-001: ClientRoomCreationReply to socket #" + QString::number(socketDescriptor));
        emit SignalConnectionSendOutgoingData(socketDescriptor);

        SendRoomAddedMessageToQuery(_rooms.size());
    }
    else //NO FREE SPACE AVAILABLE
    {
        Connection* currentConnection = DefineConnection(socketDescriptor);
        currentConnection->setOutgoingDataBuffer(FormClientRoomCreationReply(false, 0, 0, RoomCreationErrors::NO_FREE_SLOTS_AVAILABLE));
        emit SignalServerLogReport("NAY-001: ClientRoomCreationReply WITH ERRORS to socket #" + QString::number(socketDescriptor));
        emit SignalConnectionSendOutgoingData(socketDescriptor);

    }


}

void Server::ProcessChartMessage(const QByteArray &data, int socketDescriptor)
{
    serverMessageSystem::ChartMessage message;

    if (!message.ParseFromArray(data.data(), data.size()))
    {
        emit SignalServerLogReport("NAY-001: Error while ProcessServerInputQueryRequest() ");
        return;
    }

    uint32_t roomID = message.roomid();

    emit SignalServerLogReport("NAY-001: Processing Chart Message From Socket Descriptor: " + QString::number(socketDescriptor));
    emit SignalServerLogReport("NAY-001: ChartMessage: ChartMessage: " + QString::fromStdString(message.chartmessage()));
    emit SignalServerLogReport("NAY-001: ChartMessage: Sender Name: " + QString::fromStdString(message.sendername()));
    emit SignalServerLogReport("NAY-001: ChartMessage: Room ID: " + QString::number(roomID));

    //First of all, check the room for existance.
    if (_rooms.size() + 1 < roomID)
    {
        emit SignalServerLogReport("NAY-001: Error while ProcessChartMessage() Room not found!!!");
        return;
    }
    //If there's.. Send the message to all the clients we have in the room.
    else
    {
        foreach (Connection* connection, _rooms[roomID-1]->connections())
        {
            connection->setOutgoingDataBuffer(FormChartMessage(QString::fromStdString(message.chartmessage()),
                                                               QString::fromStdString(message.sendername()),
                                                               roomID)
                                              );
            emit SignalServerLogReport("NAY-001: ServerInputQueryReply to socket #" + QString::number(connection->socket()->socketDescriptor()));
            emit SignalConnectionSendOutgoingData(socketDescriptor);
        }
    }



}

void Server::ProcessClientConnectionToRoomRequest(const QByteArray &data, int socketDescriptor)
{
    serverMessageSystem::ClientConnectionToRoomRequest message;

    if (!message.ParseFromArray(data.data(), data.size()))
    {
        emit SignalServerLogReport("NAY-001: Error while ProcessServerInputQueryRequest() ");
        return;
    }

    emit SignalServerLogReport("NAY-001: Processing ClientConnectionToRoomRequest From Socket Descriptor: " + QString::number(socketDescriptor));
    emit SignalServerLogReport("NAY-001: ClientConnectionToRoomRequest: CleintName: " + QString::fromStdString(message.clientname()));
    emit SignalServerLogReport("NAY-001: ClientConnectionToRoomRequest: AgreeToWait: " + QString::number(message.agreetowait()));
    emit SignalServerLogReport("NAY-001: ClientConnectionToRoomRequest: ConnectToAnyRoom: " + QString::number(message.connecttoanyroom()));


    //ПО УМОЛЧАНИЮ ЗАЛОЧИТЬ "Согласен ждать!"

    //Если пользователь не согласен ждать
    if (!message.agreetowait())
    {
        //Если есть комнаты, и среди них есть те, которые готовятся к игре (готовы принять игроков)
        if (_rooms.size() && _roomsArePreparingToGame)
        {
            //Если пользователь согласен присоединиться к любой комнате, а такая комната всего одна ли их несколько - соединить его с первой свободной.
            if (message.connecttoanyroom() && _roomsArePreparingToGame)
            {
                //Отправить пользователя непосредственно в эту комнату
                //Но проверить очередь.
                //Если очередь не пустая, отправить ему (есть свободные комнаты) и номер в очереди.
                //На стороне клиента должно появиться сообщение ожидания в очереди на соединение (с обновлением)
                //Если очередь пустая, соединить его с текущей комнатой

                if (_query.empty())
                {
                    //подсоединить его к первой попавшейся
                    std::vector<uint32_t>roomIDs;
                    if (FirstReadyToAcceptPlayersRoom() != nullptr) // не может быть равно, т.к. _roomsArePreparingToGame не ноль
                        roomIDs.push_back(FirstReadyToAcceptPlayersRoom()->id());
                    else
                    {
                        qDebug() << "NAY-001: Unexpected ERROR while FirstReadyToAcceptPlayersRoom()! ";
                        return;
                    }
                    Connection* connection = DefineConnection(socketDescriptor);
                    connection->setOutgoingDataBuffer(FormClientConnectionToRoomReply(false,
                                                                                      FreeSlotsLeft(),
                                                                                      roomIDs,
                                                                                      NO_QUERY_POSITION));

                }
                else //если очередь не пуста, проверить, что есть места (или - в будущем - сбросить, т.к. ждать он не хочет)
                {
                    //если места есть, показать пользователю место в очереди
                    if (!QueryOverSize(_query.size() + 1))
                    {
                        std::vector<uint32_t>roomIDs;
                        if (FirstReadyToAcceptPlayersRoom() != nullptr) // не может быть равно, т.к. _roomsArePreparingToGame не ноль
                            roomIDs.push_back(FirstReadyToAcceptPlayersRoom()->id());
                        else
                        {
                            qDebug() << "NAY-001: Unexpected ERROR while FirstReadyToAcceptPlayersRoom()! ";
                            return;
                        }

                        Connection* connection = DefineConnection(socketDescriptor);
                        _query.push_back(connection);
                        ++_querySize;
                        UpdateStatistics();
                        qDebug() << "NAY-001 : Connection was added to Query at position: " << _query.size();
                        emit SignalServerLogReport("NAY-001: Connection was added to Query at position: " + QString::number(_query.size()));
                        connection->setOutgoingDataBuffer(FormClientConnectionToRoomReply(false,
                                                                                          FreeSlotsLeft(),
                                                                                          roomIDs,
                                                                                          _query.size()));
                    }
                    //если мест нет, сказать об этом
                    else
                    {
                        qDebug() << "NAY-001 : No free space in query! Size: " << _query.size();
                        emit SignalServerLogReport("NAY-001: No free space in query! Size: " + QString::number(_query.size()));
                        std::vector<uint32_t>roomIDs;
                        Connection* connection = DefineConnection(socketDescriptor);
                        connection->setOutgoingDataBuffer(FormClientConnectionToRoomReply(false,
                                                                                          FreeSlotsLeft(),
                                                                                          roomIDs,
                                                                                          QUERY_OVERSIZE));
                    }
                }
            } ////Если пользователь согласен присоединиться к любой комнате, а такая комната всего одна
            //Если пользователь не согласен присоединяться к любой комнате и есть комнаты, готовящиеся к игре
            if (!message.connecttoanyroom() && _roomsArePreparingToGame)
            {
                //Если в очереди всё ещё есть места, отправить ему (есть свободные комнаты) и номер в очереди.
                if (!QueryOverSize(_query.size() + 1))
                {
                    //Показать ему все варианты и добавить его в очередь
                    std::vector<uint32_t>roomIDs;
                    for (uint32_t var = 0; var < _rooms.size(); ++var) {
                        if (!(_rooms[var]->GetIsPlaying()))
                            roomIDs.push_back(_rooms[var]->id());
                    }

                    Connection* connection = DefineConnection(socketDescriptor);
                    _query.push_back(connection);
                    ++_querySize;
                    UpdateStatistics();
                    qDebug() << "NAY-001 : Connection was added to Query at position: " << _query.size();
                    connection->setOutgoingDataBuffer(FormClientConnectionToRoomReply(false,
                                                                                      FreeSlotsLeft(),
                                                                                      roomIDs,
                                                                                      _query.size()));
                }
                else //Если в очереди нет мест, отправить ему сообщение об этом.
                {
                    qDebug() << "NAY-001 : No free space in query! Size: " << _query.size();
                    emit SignalServerLogReport("NAY-001: No free space in query! Size: " + QString::number(_query.size()));
                    std::vector<uint32_t>roomIDs;
                    Connection* connection = DefineConnection(socketDescriptor);
                    connection->setOutgoingDataBuffer(FormClientConnectionToRoomReply(false,
                                                                                      FreeSlotsLeft(),
                                                                                      roomIDs,
                                                                                      QUERY_OVERSIZE));
                } ////Если в очереди нет мест, отправить ему сообщение об этом.
            }////Если пользователь не согласен присоединяться к любой комнате и есть комнаты, готовящиеся к игре
        }////Если есть комнаты, и среди них есть те, которые готовятся к игре (готовы принять игроков)
        //Если нет свободных комнат, готовящихся к игре - в независимости от того, готов ли он присоединиться к любой  - сбросить его,
        //т.к. он не хочет ждать
        else if ((_rooms.size() && !_roomsArePreparingToGame) || (!_rooms.size()))
        {
            //Вернуть ему сообщение о невозмоности присоединиться сразу - нет свободных комнат.
            //Пользовательское меню на стороне клиента должно показать сообщение об ошибке соединения - "Нет свободных комнат".

            std::vector<uint32_t>roomIDs;
            Connection* connection = DefineConnection(socketDescriptor);
            connection->setOutgoingDataBuffer(FormClientConnectionToRoomReply(true,
                                                                              FreeSlotsLeft(),
                                                                              roomIDs,
                                                                              NO_QUERY_POSITION));
        }////Если нет свободных комнат, готовящихся к игре
    }////Если пользователь не согласен ждать
    //Если пользователь согласен ждать
    else
    {
        //Если пользователь готов сразу присоединиться к игре и свободные комнаты есть:
        if (message.connecttoanyroom() && _roomsArePreparingToGame)
        {
            //подсоединить его к первой попавшейся
            if (_query.empty())
            {
                //подсоединить его к первой попавшейся
                std::vector<uint32_t>roomIDs;
                if (FirstReadyToAcceptPlayersRoom() != nullptr) // не может быть равно, т.к. _roomsArePreparingToGame не ноль
                    roomIDs.push_back(FirstReadyToAcceptPlayersRoom()->id());
                else
                {
                    qDebug() << "NAY-001: Unexpected ERROR while FirstReadyToAcceptPlayersRoom()! ";
                    return;
                }
                Connection* connection = DefineConnection(socketDescriptor);
                connection->setOutgoingDataBuffer(FormClientConnectionToRoomReply(false,
                                                                                  FreeSlotsLeft(),
                                                                                  roomIDs,
                                                                                  NO_QUERY_POSITION));
            }
            else //если очередь не пуста, проверить, что есть места
            {
                //если места есть, показать пользователю место в очереди
                if (!QueryOverSize(_query.size() + 1))
                {
                    //Показать ему все варианты и добавить его в очередь
                    std::vector<uint32_t>roomIDs;
                    for (uint32_t var = 0; var < _rooms.size(); ++var) {
                        if (!(_rooms[var]->GetIsPlaying()))
                            roomIDs.push_back(_rooms[var]->id());
                    }

                    Connection* connection = DefineConnection(socketDescriptor);
                    _query.push_back(connection);
                    ++_querySize;
                    UpdateStatistics();
                    qDebug() << "NAY-001 : Connection was added to Query at position: " << _query.size();
                    emit SignalServerLogReport("NAY-001: Connection was added to Query at position: " + QString::number(_query.size()));
                    connection->setOutgoingDataBuffer(FormClientConnectionToRoomReply(false,
                                                                                      FreeSlotsLeft(),
                                                                                      roomIDs,
                                                                                      _query.size()));
                }
                //если мест нет, сказать об этом
                else
                {
                    qDebug() << "NAY-001 : No free space in query! Size: " << _query.size();
                    emit SignalServerLogReport("NAY-001: No free space in query! Size: " + QString::number(_query.size()));
                    std::vector<uint32_t>roomIDs;
                    Connection* connection = DefineConnection(socketDescriptor);
                    connection->setOutgoingDataBuffer(FormClientConnectionToRoomReply(false,
                                                                                      FreeSlotsLeft(),
                                                                                      roomIDs,
                                                                                      QUERY_OVERSIZE));
                }
            }
        }////Если пользователь готов сразу присоединиться к игре и свободные комнаты есть:
        //Если пользователь не готов сразу присоединиться к игре и свободные комнаты есть:
        if (!message.connecttoanyroom() && _roomsArePreparingToGame)
        {
            //если места есть, показать пользователю все комнаты. Он находится в очереди, И МОЖЕТ СВОБОДНО ВЫБИРАТЬ ЛЮБУЮ КОМНАТУ.
            //Он сможет выбрать любую комнату даже впердёт тех, кто стоит в очереди перед ним, но слишком долго думает.

            if (!QueryOverSize(_query.size() + 1))
            {
                //Показать ему все варианты и добавить его в очередь
                std::vector<uint32_t>roomIDs;
                for (uint32_t var = 0; var < _rooms.size(); ++var) {
                    if (!(_rooms[var]->GetIsPlaying()))
                        roomIDs.push_back(_rooms[var]->id());
                }

                Connection* connection = DefineConnection(socketDescriptor);
                _query.push_back(connection);
                ++_querySize;
                UpdateStatistics();
                qDebug() << "NAY-001 : Connection was added to Query at position: " << _query.size();
                emit SignalServerLogReport("NAY-001: Connection was added to Query at position: " + QString::number(_query.size()));
                connection->setOutgoingDataBuffer(FormClientConnectionToRoomReply(false,
                                                                                  FreeSlotsLeft(),
                                                                                  roomIDs,
                                                                                  _query.size()));
            }
            //если мест нет, сказать об этом
            else
            {
                qDebug() << "NAY-001 : No free space in query! Size: " << _query.size();
                emit SignalServerLogReport("NAY-001: No free space in query! Size: " + QString::number(_query.size()));
                std::vector<uint32_t>roomIDs;
                Connection* connection = DefineConnection(socketDescriptor);
                connection->setOutgoingDataBuffer(FormClientConnectionToRoomReply(false,
                                                                                  FreeSlotsLeft(),
                                                                                  roomIDs,
                                                                                  QUERY_OVERSIZE));
            }
        }////Если пользователь не готов сразу присоединиться к игре и свободные комнаты есть:
        if (!_roomsArePreparingToGame) //Если свободных комнат нет
        {
          //И есть свободное место в очереди - просто добавить его в очередь, показать пустой список, без комнат.
            if (!QueryOverSize(_query.size() + 1))
            {
                //Показать ему пустое окно выбора и добавить его в очередь (меню должно будет показать ему его позицию - чтобы торопился)
                std::vector<uint32_t>roomIDs;

                Connection* connection = DefineConnection(socketDescriptor);
                _query.push_back(connection);
                ++_querySize;
                UpdateStatistics();
                qDebug() << "NAY-001 : Connection was added to Query at position: " << _query.size();
                emit SignalServerLogReport("NAY-001: Connection was added to Query at position: " + QString::number(_query.size()));
                connection->setOutgoingDataBuffer(FormClientConnectionToRoomReply(true,
                                                                                  FreeSlotsLeft(),
                                                                                  roomIDs,
                                                                                  _query.size()));
            }
            //если мест нет, сказать об этом
            else
            {
                qDebug() << "NAY-001 : No free space in query! Size: " << _query.size();
                emit SignalServerLogReport("NAY-001: No free space in query! Size: " + QString::number(_query.size()));
                std::vector<uint32_t>roomIDs;
                Connection* connection = DefineConnection(socketDescriptor);
                connection->setOutgoingDataBuffer(FormClientConnectionToRoomReply(true,
                                                                                  FreeSlotsLeft(),
                                                                                  roomIDs,
                                                                                  QUERY_OVERSIZE));
            }
        }
    }////Если пользователь согласен ждать

    //Отправить ответ в любом случае!
    Connection* connection = DefineConnection(socketDescriptor);
    emit SignalServerLogReport("NAY-001: ClientConnectionToRoomReply to socket #" + QString::number(connection->socket()->socketDescriptor()));
    emit SignalConnectionSendOutgoingData(socketDescriptor);

}
