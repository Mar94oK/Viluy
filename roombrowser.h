#ifndef ROOMBROWSER_H
#define ROOMBROWSER_H

#include <QWidget>
#include "roomparameters.h"


struct RoomsPosition
{
    uint32_t row;
    uint32_t column;

    RoomsPosition (uint32_t rw, uint32_t cl) :
        row(rw), column(cl)
    { }
};


namespace Ui {
class RoomBrowser;
}

class RoomBrowser : public QWidget
{
    Q_OBJECT

public:
    explicit RoomBrowser(QWidget *parent = 0);
    ~RoomBrowser();

private:

    Ui::RoomBrowser *ui;

private:

    std::vector <RoomParameters* > _roomParameters;

signals:

    void SignalUpdateRoomNumber(uint32_t roomNumber);
    void SignalUpdateSockets(const std::vector<uint32_t>& socketDescriptor);
    void SignalUpdateMaxNumberOfPlayers(uint32_t maxNumber);
    void SignalUpdateActualNumberOfPLayers(uint32_t actualNumberOfPLayers);
    void SignalUpdatePlayersNames(const QStringList& playersNames);

private slots:

    void SlotUpdateRoomNumber(uint32_t roomNumber)
    { emit SignalUpdateRoomNumber(roomNumber); }

    void SlotUpdateSockets(const std::vector<uint32_t>& socketDescriptor)
    { emit SignalUpdateSockets(socketDescriptor); }

    void SlotUpdateMaxNumberOfPlayers(uint32_t maxNumber)
    { emit SignalUpdateMaxNumberOfPlayers(maxNumber); }

    void SlotUpdateActualNumberOfPLayers(uint32_t actualNumberOfPLayers)
    { emit SignalUpdateActualNumberOfPLayers(actualNumberOfPLayers); }

    void SlotUpdatePlayersNames(const QStringList& playersNames)
    { emit SignalUpdatePlayersNames(playersNames); }

public slots:

    void SlotAddNewRoom(const Room& givenRoom);
    void SlotUpdateExisting(const Room& givenRoom);
    void SlotDeleteRoom(uint32_t roomID);

private:

    RoomParameters* DefineRoomById(uint32_t id);
    RoomsPosition FindPositionForNewRoom();


};

#endif // ROOMBROWSER_H
