#include "room.h"

uint32_t Room::numberOfPlayers() const
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
             _connections.shrink_to_fit();
             qDebug() << "NAY-001: socketDescriptor was removed from Room with ID: " << _id;             
             try
             {
                 _players.erase(_players.begin() + var);
                 //update numbers...
                 --_numberOfPlayers;
                 //delete respective players,
             }
             catch (...)
             {
                 qDebug() << "Error while erasing vector of players.";
             }
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

void Room::DeleteUserByName(const QString &name)
{
    for (uint32_t var = 0; var < _players.size(); ++var)
    {
        if (_players[var].name() == name)
        {
            _connections.erase(_connections.begin() + var);
            _players.erase(_players.begin() + var);
            --_numberOfPlayers;
        }
    }
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

UnconnectedSocketInfo Room::DefineInfoOfUnconnectedSocket()
{
    if (_players.size() != _connections.size())
    {
        qDebug() << "NAY-001: Error in Room Holder! _players.size() != _connections.size()";
        return UnconnectedSocketInfo(0,"");
    }
    for (uint32_t var = 0; var < _players.size(); ++var)
    {
        if (_connections[var]->socket()->socketDescriptor() == CLOSED_SOCKET_DESCRIPTOR)
        {
            return UnconnectedSocketInfo(var,_players[var].name());
        }
    }
    qDebug() << "NAY-001: Not found ";
    return UnconnectedSocketInfo(0,"");
}


QString Player::name() const
{
    return _name;
}

void Player::setName(const QString &name)
{
    _name = name;
}
