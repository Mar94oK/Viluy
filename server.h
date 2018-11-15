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


#include "connection.h"
typedef QPair<QString, QString> serverSettings;



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
    ServerSettings _settings;

signals:

    void sig_serverErrorReport(QString);
    void SignalServerLogReport(QString);
    void sig_serverInfoReport(QString);
    void SignalConnectionSendOutgoingData(int socketDescriptor);
    void SignalUpdateRoomsQuantity(unsigned int quantity);
    void SignalUpdateStatistics(const QStringList& statistic);

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

    QByteArray FormServerInputQueryReply();
    QByteArray FormClientRoomCreationReply(bool created, unsigned int slotId, unsigned int freeSlotsLeft, RoomCreationErrors ErrorNumber);
    QByteArray FromServerReportsOpponentIsEnteringRoom(const QString& opponentName, uint32_t roomId);
    QByteArray FormChartMessage(const QString& message, const QString& sender, uint32_t roomID);

    Connection* DefineConnection(int socketDescriptor);

    bool RemoveConnectionFromRoom(int socketDescriptor);
    bool RemoveConenctionFromServerCeonncetionsPull(int socketDescriptor);


    //RoomCreation and Maintaning Related

    void RoomCreation();
    bool RoomDeleting(uint32_t roomId);
    Room* DefineRoom(uint32_t roomId);

private:

    void UpdateStatistics();

private:

    uint32_t _roomsCreatedDuringSession = 0;
    uint32_t _connectionsDuringSession = 0;
    uint32_t _gamesStartedDuringSession = 0;
    uint32_t _roomsArePreparingToGame = 0;
    uint32_t _roomArePLaying = 0;
    uint32_t _closedRooms = 0;
    uint32_t _activeConnections = 0;
    uint32_t _maximumSimultaneousConnections = 0;

    QString _roomsCreatedBaseText = "Создано комнат: ";
    QString _connectionsCreatedBaseText = "Создано подключений: ";
    QString _gamesStartedCreatedBaseText = "Начато игр: ";
    QString _roomsArePreparingToGameCreatedBaseText = "Комнат, ожидающих игроков: ";
    QString _roomsAreActiveBaseText = "Активных комнат: ";
    QString _closedRoomsBaseText = "Закрытых комнат: ";
    QString _activeConnectionsBaseText = "Активных соединений: ";
    QString _maximumSimultaneousConnectionsdBaseText = "Максимальное количество соединений: ";

};

#endif // SERVER_H
