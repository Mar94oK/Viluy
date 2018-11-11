#include "room.h"

int Room::numberOfPlayers() const
{
    return _numberOfPlayers;
}

void Room::setNumberOfPlayers(int numberOfPlayers)
{
    _numberOfPlayers = numberOfPlayers;
}

std::vector<Player> Room::players() const
{
    return _players;
}

void Room::addPlayer(const Player &player)
{
    _players.push_back(player);
}

std::vector<Connection *> Room::connections() const
{
    return _connections;
}

bool Room::RemoveConnection(int socketDescriptor)
{
    for (unsigned int var = 0; var < _connections.size(); ++var)
    {
        if (_connections[var]->socket()->socketDescriptor() == socketDescriptor)
        {
             _connections.erase(_connections.begin() + var);
             qDebug() << "NAY-001: socketDescriptor was removed from Room with ID: " << _id;
               return true;
        }
    }
    return false;
}

int Room::ReassignedRoomMaster()
{
    return _connections[MASTER_CONNECTION_ID]->socket()->socketDescriptor();
}

uint32_t Room::id() const
{
    return _id;
}

void Room::setId(const uint32_t &id)
{
    _id = id;
}


QString Player::name() const
{
    return _name;
}

void Player::setName(const QString &name)
{
    _name = name;
}
