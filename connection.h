#ifndef CONNECTION_H
#define CONNECTION_H
#include <QTcpSocket>
#include <QString>

#define ROOM_ID_NOT_ASSIGNED 0
#define CLOSED_SOCKET_DESCRIPTOR (-1)

class Connection
{

public:

    explicit Connection (QTcpSocket* connection, QString connectionName) :
        _socket(connection), _connectionName(connectionName), _assosiatedRoomId(ROOM_ID_NOT_ASSIGNED)
    { }

private:

    QTcpSocket* _socket;
    QByteArray  _OutgoingDataBuffer;
    QByteArray  _IncomingDataBuffer;
    QString     _connectionName; //init with socketDescriptor, continue with ClientName
    uint32_t    _assosiatedRoomId;

public:

    QByteArray OutgoingDataBuffer() const;
    void setOutgoingDataBuffer(const QByteArray &OutgoingDataBuffer);
    QByteArray IncomingDataBuffer() const;
    void setIncomingDataBuffer(const QByteArray &IncomingDataBuffer);
    QTcpSocket *socket() const;
    void setSocket(QTcpSocket *socket);

};

#endif // CONNECTION_H
