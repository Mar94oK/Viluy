#include "roombrowser.h"
#include "ui_roombrowser.h"

RoomBrowser::RoomBrowser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoomBrowser)
{
    ui->setupUi(this);
}

RoomBrowser::~RoomBrowser()
{
    delete ui;
}

void RoomBrowser::SlotAddNewRoom(const Room &givenRoom)
{
    RoomParameters* room = new RoomParameters();
    room->AssignNewRoom(givenRoom);
    _roomParameters.push_back(room);
    ui->layt_Rooms->addWidget(room, FindPositionForNewRoom().row,
                              FindPositionForNewRoom().column);

}

void RoomBrowser::SlotUpdateExisting(const Room &givenRoom)
{
    RoomParameters* currentRoom = DefineRoomById(givenRoom.id());
    if (currentRoom != nullptr)
        currentRoom->UpdateRoom(givenRoom);

}

void RoomBrowser::SlotDeleteRoom(uint32_t roomID)
{
    RoomParameters* currentRoom = DefineRoomById(roomID);
    if (currentRoom != nullptr)
    {
        currentRoom->hide();
        currentRoom->deleteLater();
        for (uint32_t var = 0; var < _roomParameters.size(); ++var)
        {
            if (_roomParameters[var] == nullptr)
                _roomParameters.erase(_roomParameters.begin() + var);
        }
    }
}

void RoomBrowser::SlotDeleteClientFromRoom(const QString &name)
{
    RoomParameters* current = DefineRoomByUser(name);
    if (current != nullptr)
    {
        current->DeleteRoomCredentialsByUsername(name);
    }
}

RoomParameters *RoomBrowser::DefineRoomById(uint32_t id)
{
    qDebug() << "NAY-001: Enetering room deleting in browser! ";
    for (int var = 0; var < _roomParameters.size(); ++var)
    {
        if (_roomParameters[var]->RoomId() == id)
            return _roomParameters[var];
    }
    qDebug() << "Error while DefineRoomById() in RoomBrowser(): ";
    return nullptr;
}

RoomParameters *RoomBrowser::DefineRoomByUser(const QString &clientName)
{
    for (int var = 0; var < _roomParameters.size(); ++var)
    {
        for (uint32_t y = 0; y < _roomParameters[var]->getRoomProperty().players().size(); ++y)
        {
            if (_roomParameters[var]->getRoomProperty().players()[y].name() == clientName)
                return _roomParameters[var];
        }
    }
    qDebug() << "NAY-001 : Error while RoomParameters *RoomBrowser::DefineRoomByUser(). Not found!";
    return nullptr;
}

RoomsPosition RoomBrowser::FindPositionForNewRoom()
{
    uint32_t currentRow = _roomParameters.size() / 4; //full Rows
    uint32_t nextColumn = _roomParameters.size() % 4;

    return RoomsPosition(currentRow, nextColumn);
}
