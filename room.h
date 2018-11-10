#ifndef ROOM_H
#define ROOM_H

#include <QObject>
#include <gamesettings.h>
#include <QTcpSocket>
#include <QString>
#include "connection.h"

class Player
{
  QString _name;

public:

  explicit Player (QString name) :
                    _name(name) {}

public:

  QString name() const;
  void setName(const QString &name);

};

class Room
{

public:

    explicit Room(uint32_t id, QString name, uint32_t numberOfPLayers, GameSettings settings, Player firstPlayer, Connection* firstConnection) :
    _id(id), _name(name), _numberOfPlayers(numberOfPLayers), _gameSettings(settings)
    {
        _players.push_back(firstPlayer);
        _connections.push_back(firstConnection);
    }

public:

    int numberOfPlayers() const;
    void setNumberOfPlayers(int numberOfPlayers);

    RulesType rulesType() const;
    void setRulesType(const RulesType &rulesType);

    std::vector<Player> players() const;
    void addPlayer(const Player &player);

private:

    uint32_t _id;
    QString _name;
    uint32_t _numberOfPlayers;
    GameSettings _gameSettings;
    std::vector<Player> _players;
    std::vector<Connection* > _connections;

};

#endif // ROOM_H
