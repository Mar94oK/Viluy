#include "room.h"

Room::Room(QObject *parent) : QObject(parent)
{

}

int Room::numberOfPlayers() const
{
    return _numberOfPlayers;
}

void Room::setNumberOfPlayers(int numberOfPlayers)
{
    _numberOfPlayers = numberOfPlayers;
}

RulesType Room::rulesType() const
{
    return _rulesType;
}

void Room::setRulesType(const RulesType &rulesType)
{
    _rulesType = rulesType;
}
