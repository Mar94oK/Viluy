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

GameSettings Room::gameSettings() const
{
    return _gameSettings;
}

void Room::setGameSettings(const GameSettings &gameSettings)
{
    _gameSettings = gameSettings;
}

QString Room::name() const
{
    return _name;
}

void Room::setName(const QString &name)
{
    _name = name;
}

void Room::AddUserToTheRoom(Player player, Connection *connection)
{
    ++_numberOfPlayers;
    _players.push_back(player);
    _connections.push_back(connection);
}

void Room::ApplyFromAnother(const Room &another)
{
    _numberOfPlayers = another.numberOfPlayers();
    _id = another.id();
    _name = another.name();
    _numberOfPlayers = another.numberOfPlayers();
    _gameSettings = another.gameSettings();
    _players = another.players();
    _connections = another.connections();
    _isPlaying = another.GetIsPlaying();
}

QString Room::DefineClientNameOfUnconnectedSocket()
{
    if (_players.size() != _connections.size())
    {
        qDebug() << "NAY-001: Error in Room Holder! _players.size() != _connections.size()";
        return "";
    }
    for (uint32_t var = 0; var < _players.size(); ++var)
    {
        if (_connections[var]->socket()->socketDescriptor() == CLOSED_SOCKET_DESCRIPTOR)
        {
            return _players[var].name();
        }
    }
    qDebug() << "NAY-001: Not found ";
    return "";
}


QString Player::name() const
{
    return _name;
}

void Player::setName(const QString &name)
{
    _name = name;
}
