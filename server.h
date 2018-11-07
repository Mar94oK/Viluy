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

typedef QPair<QString, QString> serverSettings;


class Connection
{

public:

    explicit Connection (QTcpSocket* connection, QString connectionName) :
        _socket(connection), _connectionName(connectionName)
    { }

private:

    QTcpSocket* _socket;
    QByteArray  _OutgoingDataBuffer;
    QByteArray  _IncomingDataBuffer;
    QString     _connectionName; //init with socketDescriptor, continue with ClientName

public:

    QByteArray OutgoingDataBuffer() const;
    void setOutgoingDataBuffer(const QByteArray &OutgoingDataBuffer);
    QByteArray IncomingDataBuffer() const;
    void setIncomingDataBuffer(const QByteArray &IncomingDataBuffer);
    QTcpSocket *socket() const;
    void setSocket(QTcpSocket *socket);

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

    std::vector<Room> _rooms;
    std::vector<Connection*> _establishedConnections;
    ServerSettings _settings;

signals:

    void sig_serverErrorReport(QString);
    void SignalServerLogReport(QString);
    void sig_serverInfoReport(QString);
    void SignalConnectionSendOutgoingData(int socketDescriptor);

private slots:

    int SlotSessionOpened();
    void SlotSetUpNewConnection();
    void SlotReadIncomingData(int socketDescriptor);
    void slot_reportError(QAbstractSocket::SocketError);
    void SlotConnectionSendOutgoingData(int socketDescriptor);

private:

    void setFortunes();
    void slot_serverInitializaion();
    void MessagesParser(const QByteArray& data, int socketDescriptor);

    void ProcessServerInputQueryRequest(const QByteArray& data, int socketDescriptor);

    QByteArray FormServerInputQueryReply();

    Connection* DefineConnection(int socketDescriptor);

};

#endif // SERVER_H
