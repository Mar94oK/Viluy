#include "connection.h"

QByteArray Connection::IncomingDataBuffer() const
{
    return _IncomingDataBuffer;
}

void Connection::setIncomingDataBuffer(const QByteArray &IncomingDataBuffer)
{
    _IncomingDataBuffer = IncomingDataBuffer;
}

QTcpSocket *Connection::socket() const
{
    return _socket;
}

void Connection::setSocket(QTcpSocket *connection)
{
    _socket = connection;
}

QByteArray Connection::OutgoingDataBuffer() const
{
    return _OutgoingDataBuffer;
}

void Connection::setOutgoingDataBuffer(const QByteArray &OutgoingDataBuffer)
{
    _OutgoingDataBuffer = OutgoingDataBuffer;
}
