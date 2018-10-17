#ifndef SERVERSETTINGS_H
#define SERVERSETTINGS_H
#include "addressBook.pb.h"


class ServerSettings
{
    int _maxNumberOfRooms;

public:
    ServerSettings(int maxRooms = ServerSettings::defaultMaximumNumberOfRooms) : _maxNumberOfRooms(maxRooms) { }
    int maxNumberOfRooms() const;
    void setMaxNumberOfRooms(int maxNumberOfRooms);


    static const int defaultMaximumNumberOfRooms = 10;
};

#endif // SERVERSETTINGS_H
