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

    QByteArray FormServerInputQueryReply();
    QByteArray FormClientRoomCreationReply(bool created, unsigned int slotId, unsigned int freeSlotsLeft, RoomCreationErrors ErrorNumber);
    QByteArray FromServerReportsOpponentIsEnteringRoom(const QString& opponentName, uint32_t roomId);


    Connection* DefineConnection(int socketDescriptor);

    bool RemoveConnectionFromRoom(int socketDescriptor);
    bool RemoveConenctionFromServerCeonncetionsPull(int socketDescriptor);


    //RoomCreation and Maintaning Related

    void RoomCreation();
    bool RoomDeleting(uint32_t roomId);
    Room* DefineRoom(uint32_t roomId);


};

#endif // SERVER_H
