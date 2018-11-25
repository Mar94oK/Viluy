#ifndef ROOMPARAMETERS_H
#define ROOMPARAMETERS_H

#include <QWidget>
#include <room.h>

namespace Ui {
class RoomParameters;
}

class RoomParameters : public QWidget
{
    Q_OBJECT

public:
    explicit RoomParameters(QWidget *parent = 0);
    ~RoomParameters();

private:

    Ui::RoomParameters *ui;
    Room roomProperty;

public:

    void AssignNewRoom(const Room& givenRoom);
    void UpdateRoom(const Room& givenRoom);

    uint32_t RoomId() { return roomProperty.id(); }

    Room getRoomProperty() const;
    void setRoomProperty(const Room &value);

    void DeleteRoomCredentialsByUsername(const QString& clientName);

private:

    void UpdateRoom();

public slots:

    void SlotUpdateRoomNumber(uint32_t roomNumber);
    void SlotUpdateSockets(const std::vector<uint32_t>& socketDescriptor);
    void SlotUpdateMaxNumberOfPlayers(uint32_t maxNumber);
    void SlotUpdateActualNumberOfPLayers(uint32_t actualNumberOfPLayers);
    void SlotUpdatePlayersNames(const QStringList& playersNames);




};

#endif // ROOMPARAMETERS_H
