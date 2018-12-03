#ifndef SERVER_H
#define SERVER_H

#include <QObject>

#include <QtNetwork>
#include <QString>
#include <QNetworkSession>
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkSession>
#include <serversettings.h>
//since it is necessary to report everything during server init, the Window should already be created.
#include <mainwindow.h>
#include <room.h>
#include "serverMessageSystem.pb.h"
#include "messagesdefinitions.h"
#include <queue>


#include "connection.h"
typedef QPair<QString, QString> serverSettings;



//query maximum size should be calculated as
//_settings.maximumNumberOfRooms * maximum number of opponents (more than this should never appear)


struct CredentialsOfUnconnectedSocketData
{
    Room* unconnectedSocketRoom;
    QString name;
    bool isMasterConnection;

    explicit CredentialsOfUnconnectedSocketData(Room* room, const QString& str, bool mstr) :
        unconnectedSocketRoom(room), name(str), isMasterConnection(mstr)
    { }
};




class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server( QObject *parent = 0);

private:

    QTcpServer *tcpServer;
    QStringList fortunes;
    QNetworkSession *networkSession;

    std::vector<Room* > _rooms;
    std::vector<Connection*> _establishedConnections;
    std::vector<uint32_t> _establishedConnectionsDescriptors;
    ServerSettings _settings;

    std::vector <Connection*> _query;

signals:

    void sig_serverErrorReport(QString);
    void SignalServerLogReport(QString);
    void sig_serverInfoReport(QString);
    void SignalConnectionSendOutgoingData(int socketDescriptor);
    void SignalUpdateRoomsQuantity(unsigned int quantity);
    void SignalUpdateStatistics(const QStringList& statistic);

signals:

    void SignalAddNewRoomToBrowser(const Room& room);
    void SignalUpdateExisitngRoomInBrowser(const Room& room);
    void SignalDeleteRoomInBrowser(uint32_t id);
    void SignalServerReportsClientIsLeaving(const QString& name);


private slots:

    int SlotSessionOpened();
    void SlotSetUpNewConnection();
    void SlotReadIncomingData(int socketDescriptor);
    void slot_reportError(QAbstractSocket::SocketError);
    void SlotConnectionSendOutgoingData(int socketDescriptor);
    void SlotClientConnectionIsClosing(long long ID);

public: //Debug Only!!!

    void DebugSlotSendReportsOpponentIsEnteringRoom(uint32_t roomId);

private:

    void setFortunes();
    void SlotServerInitializaion();
    void MessagesParser(const QByteArray& data, int socketDescriptor);

    void ProcessServerInputQueryRequest(const QByteArray& data, int socketDescriptor);
    void ProcessClientRoomCreationRequest(const QByteArray &data, int socketDescriptor);
    void ProcessChartMessage(const QByteArray &data, int socketDescriptor);
    void ProcessClientConnectionToRoomRequest(const QByteArray &data, int socketDescriptor);
    void ProcessClientWantedToEnterTheRoom(const QByteArray &data, int socketDescriptor);

    QByteArray FormServerInputQueryReply();
    QByteArray FormClientRoomCreationReply(bool created, unsigned int slotId, unsigned int freeSlotsLeft, RoomCreationErrors ErrorNumber);
    QByteArray FormServerReportsOpponentIsEnteringRoom(const QString& opponentName, uint32_t roomId);
    QByteArray FormChartMessage(const QString& message, const QString& sender, uint32_t roomID);
    QByteArray FormServerRoomChangesInSelectableList(uint32_t roomId, bool deleteUpdateFlag);
    QByteArray FormClientConnectionToRoomReply(bool noRoomsAvailable, uint32_t freeSlotLeft, const std::vector<uint32_t> &roomIDs, uint32_t queryOrder);
    QByteArray FormServerClientWantedToEnterTheRoomReply(uint32_t roomId, bool entranceAllowed);
    QByteArray FormServerReportsClientIsLeaving(uint32_t socketDescriptor, const QString& name);
    QByteArray FormServerReportsRoomHasChangedOwner(const QString& previousOwner, const QString& currentOwner);

    QByteArray FormServerReportsTheGameIsAboutToStart(Room *room);

    Connection* DefineConnection(int socketDescriptor);

    int32_t DefineDisconnectedSocketDescriptor();
    CredentialsOfUnconnectedSocketData DefineCredentialsOfUnconnectedSocket();

    bool RemoveConnectionFromRoom(int socketDescriptor);
    bool RemoveConnectionFromQuery(int socketDescriptor);
    bool RemoveConenctionFromServerCeonncetionsPull(int socketDescriptor);


    //RoomCreation and Maintaning Related

    void RoomCreation();
    bool RoomDeleting(uint32_t roomId);
    Room* DefineRoom(uint32_t roomId);
    Room* FirstReadyToAcceptPlayersRoom()
    {
        for (uint32_t var = 0; var < _rooms.size(); ++var)
        {
            if (!_rooms[var]->GetIsPlaying())
                return _rooms[var];
        }
        return nullptr;
    }


    uint32_t FreeSlotsLeft() { return _settings.maxNumberOfRooms() - _rooms.size(); }
    uint32_t QueryMaximumSize() { return _settings.maxNumberOfRooms()*(6-1); }
    bool QueryOverSize(uint32_t givenSize);

private:

    void UpdateStatistics();

private:

    void SendRoomDeletedMessageToQuery(uint32_t roomID);
    void SendRoomAddedMessageToQuery(uint32_t roomID);

private:

    uint32_t _roomsCreatedDuringSession = 0;
    uint32_t _connectionsDuringSession = 0;
    uint32_t _gamesStartedDuringSession = 0;
    uint32_t _roomsArePreparingToGame = 0;
    uint32_t _roomArePLaying = 0;
    uint32_t _closedRooms = 0;
    uint32_t _activeConnections = 0;
    uint32_t _maximumSimultaneousConnections = 0;
    uint32_t _querySize = 0;

    QString _roomsCreatedBaseText = "Создано комнат: ";
    QString _connectionsCreatedBaseText = "Создано подключений: ";
    QString _gamesStartedCreatedBaseText = "Начато игр: ";
    QString _roomsArePreparingToGameCreatedBaseText = "Комнат, ожидающих игроков: ";
    QString _roomsAreActiveBaseText = "Активных комнат: ";
    QString _closedRoomsBaseText = "Закрытых комнат: ";
    QString _activeConnectionsBaseText = "Активных соединений: ";
    QString _maximumSimultaneousConnectionsdBaseText = "Максимальное количество соединений: ";
    QString _querySizeBaseText = "Игроков в очереди: ";

private:

    //Later to be reimplemented in room
    uint32_t totalTreasures = 163;
    uint32_t totalDoors = 225;

    std::vector<uint32_t> valuesTreasures;
    std::vector<uint32_t> valuesDoors;

    std::vector<uint32_t> _positionsTreasures;
    std::vector<uint32_t> _positionsDoors;

    uint32_t randUnsignedInt(uint32_t low, uint32_t high)
    {
        // Random number between low and high
        return static_cast<unsigned int>(qrand() % ((high + 1) - low) + low);
    }

    void SetRandomDoorsOrder()
    {
        _positionsDoors.clear();

        for (unsigned int var = 1; var < totalDoors+1; ++var) {
            valuesDoors.push_back(var);
        }
        for (uint32_t var = 0; var < totalDoors; ++var)
        {
            uint32_t valuesLeft = valuesDoors.size();
            uint32_t currentPosition = randUnsignedInt(0, valuesLeft-1);
            qDebug() << "NAY-001: Position: " << valuesDoors[currentPosition];
            _positionsDoors.push_back(valuesDoors[currentPosition]);
            valuesDoors.erase(valuesDoors.begin() + static_cast<int>(currentPosition)); //remove additional
            valuesDoors.shrink_to_fit();
        }

    }
    void SetRandomTreasuresOrder()
    {
        _positionsTreasures.clear();

        for (uint32_t var = 1; var < totalTreasures+1; ++var) {
            valuesTreasures.push_back(var);
        }
        for (uint32_t var = 0; var < totalTreasures; ++var) {

            uint32_t valuesLeft = valuesTreasures.size();
            uint32_t currentPosition = randUnsignedInt(0, valuesLeft-1);
            qDebug() << "NAY-001: Position: " << valuesTreasures[currentPosition];
            _positionsTreasures.push_back(valuesTreasures[currentPosition]);
            valuesTreasures.erase(valuesTreasures.begin() + static_cast<int>(currentPosition)); //remove additional
            valuesTreasures.shrink_to_fit();
        }
    }

};

#endif // SERVER_H
