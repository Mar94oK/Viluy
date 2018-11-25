#ifndef ROOM_H
#define ROOM_H

#include <QObject>
#include <gamesettings.h>
#include <QTcpSocket>
#include <QString>
#include "connection.h"

#define MASTER_CONNECTION_ID 0

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


struct UnconnectedSocketInfo
{
    uint32_t position;
    QString name;

    explicit UnconnectedSocketInfo(uint32_t pos, const QString& str) :
        position(position), name(str)
    {}
};

class Room
{

public:

    Room(uint32_t id, QString name, uint32_t numberOfPLayers, GameSettings settings, Player firstPlayer, Connection* firstConnection) :
    _id(id), _name(name), _numberOfPlayers(numberOfPLayers), _gameSettings(settings)
    {
        _players.push_back(firstPlayer);
        _connections.push_back(firstConnection);
    }

    Room() { }

    Room(Room* given)
    {
        _id = given->id();
        _name = given->name();
        _numberOfPlayers = given->numberOfPlayers();
        _gameSettings = given->gameSettings();
        _players = given->players();
        _connections = given->connections();
        _isPlaying = given->GetIsPlaying();
    }

public:

    int numberOfPlayers() const;
    void setNumberOfPlayers(int numberOfPlayers);

    RulesType rulesType() const;
    void setRulesType(const RulesType &rulesType);

    std::vector<Player> players() const;
    void addPlayer(const Player &player);

    std::vector<Connection *> connections() const;
    bool RemoveConnection(int socketDescriptor);
    int ReassignedRoomMaster();
    bool RoomIsNotEmpty() { return _connections.size(); }

    uint32_t id() const;
    void setId(const uint32_t &id);


    //NAY-001: MARK_EXPECTED_ERROR
    //So many ideas how to define these values.
    //So I should be very accurate while forming an maintaning all of them!
    uint32_t PlayersLeftToStartTheGame()
    { return _gameSettings.maximumNumberOfPlayers() - _numberOfPlayers; }

    void SetIsPlaying() { _isPlaying = true; }
    bool GetIsPlaying() const { return _isPlaying; }

    GameSettings gameSettings() const;
    void setGameSettings(const GameSettings &gameSettings);

    QString name() const;
    void setName(const QString &name);

    void AddUserToTheRoom(Player player, Connection* connection);
    void DeleteUserByName(const QString& name);

public:

    void ApplyFromAnother (const Room& another);

public:

    //NAY-001:MARK_EXPECTED_IMPROVEMENT
    //To place names in the bundle with connections holder of the room;
    UnconnectedSocketInfo DefineInfoOfUnconnectedSocket();


private:

    uint32_t _id;
    QString _name;
    uint32_t _numberOfPlayers;
    GameSettings _gameSettings;
    std::vector<Player> _players;
    std::vector<Connection* > _connections;

    bool _isPlaying = false;

};

#endif // ROOM_H
