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


QString Player::name() const
{
    return _name;
}

void Player::setName(const QString &name)
{
    _name = name;
}
