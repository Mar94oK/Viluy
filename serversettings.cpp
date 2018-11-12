#include "serversettings.h"

unsigned int ServerSettings::freeSlots() const
{
    return _freeSlots;
}

void ServerSettings::setFreeSlots(unsigned int freeSlots)
{
    _freeSlots = freeSlots;
}

bool ServerSettings::roomCreationAllowed() const
{
    return _roomCreationAllowed;
}

void ServerSettings::setRoomCreationAllowed(bool roomCreationAllowed)
{
    _roomCreationAllowed = roomCreationAllowed;
}

bool ServerSettings::connectionToRoomAllowed() const
{
    return _connectionToRoomAllowed;
}

void ServerSettings::setConnectionToRoomAllowed(bool connectionToRoomAllowed)
{
    _connectionToRoomAllowed = connectionToRoomAllowed;
}

QString ServerSettings::serverName() const
{
    return _serverName;
}

void ServerSettings::setServerName(const QString &serverName)
{
    _serverName = serverName;
}

bool ServerSettings::immediateReplyToQuery() const
{
    return _immediateReplyToQuery;
}

void ServerSettings::setImmediateReplyToQuery(bool immediateReplyToQuery)
{
    _immediateReplyToQuery = immediateReplyToQuery;
}

unsigned int ServerSettings::maxNumberOfRooms() const
{
    return _maxNumberOfRooms;
}

void ServerSettings::setMaxNumberOfRooms(int maxNumberOfRooms)
{
    _maxNumberOfRooms = maxNumberOfRooms;
}

