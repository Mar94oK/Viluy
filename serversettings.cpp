#include "serversettings.h"

int ServerSettings::maxNumberOfRooms() const
{
    return _maxNumberOfRooms;
}

void ServerSettings::setMaxNumberOfRooms(int maxNumberOfRooms)
{
    _maxNumberOfRooms = maxNumberOfRooms;
}

