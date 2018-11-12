#ifndef SERVERSETTINGS_H
#define SERVERSETTINGS_H
#include <QString>

class ServerSettings
{
    QString      _serverName;
    unsigned int _maxNumberOfRooms;
    unsigned int _freeSlots;
    bool         _roomCreationAllowed;
    bool         _connectionToRoomAllowed;
    bool         _immediateReplyToQuery;

public:
    ServerSettings(QString serverName = "", unsigned int maxRooms = ServerSettings::defaultMaximumNumberOfRooms,
                   unsigned int freeSlots =  ServerSettings::defaultMaximumNumberOfRooms,
                   bool creationAllowed = true, bool connectionAllowed = true, bool immediateReplyToQuery = true) :
        _serverName(serverName),
        _maxNumberOfRooms(maxRooms), _freeSlots(freeSlots),
        _roomCreationAllowed(creationAllowed),
        _connectionToRoomAllowed(connectionAllowed),
        _immediateReplyToQuery(immediateReplyToQuery)
    { }

    unsigned int maxNumberOfRooms() const;
    void setMaxNumberOfRooms(int maxNumberOfRooms);

    static const unsigned int defaultMaximumNumberOfRooms = 10;
    unsigned int freeSlots() const;
    void setFreeSlots(unsigned int freeSlots);
    bool roomCreationAllowed() const;
    void setRoomCreationAllowed(bool roomCreationAllowed);
    bool connectionToRoomAllowed() const;
    void setConnectionToRoomAllowed(bool connectionToRoomAllowed);
    QString serverName() const;
    void setServerName(const QString &serverName);
    bool immediateReplyToQuery() const;
    void setImmediateReplyToQuery(bool immediateReplyToQuery);
};

#endif // SERVERSETTINGS_H
