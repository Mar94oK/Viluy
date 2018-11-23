#include "roomparameters.h"
#include "ui_roomparameters.h"

RoomParameters::RoomParameters(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoomParameters)
{
    ui->setupUi(this);
}

RoomParameters::~RoomParameters()
{
    delete ui;
}

void RoomParameters::AssignNewRoom(const Room &givenRoom)
{
    roomProperty.ApplyFromAnother(givenRoom);

    SlotUpdateRoomNumber(roomProperty.id());
    std::vector<uint32_t> connectionsIds;

    for (uint32_t var = 0; var < roomProperty.connections().size(); ++var) {
        connectionsIds.push_back(roomProperty.connections()[var]->socket()->socketDescriptor());
    }

    SlotUpdateSockets(connectionsIds);

    SlotUpdateMaxNumberOfPlayers(roomProperty.gameSettings().maximumNumberOfPlayers());

    SlotUpdateActualNumberOfPLayers(roomProperty.numberOfPlayers());

    QStringList names;
    for (int var = 0; var < roomProperty.players().size(); ++var)
    {
        names.append(roomProperty.players()[var].name());
    }

    SlotUpdatePlayersNames(names);

}

void RoomParameters::UpdateRoom(const Room &givenRoom)
{
    roomProperty.ApplyFromAnother(givenRoom);

    SlotUpdateRoomNumber(roomProperty.id());
    std::vector<uint32_t> connectionsIds;

    for (uint32_t var = 0; var < roomProperty.connections().size(); ++var) {
        connectionsIds.push_back(roomProperty.connections()[var]->socket()->socketDescriptor());
    }

    SlotUpdateSockets(connectionsIds);

    SlotUpdateMaxNumberOfPlayers(roomProperty.gameSettings().maximumNumberOfPlayers());

    SlotUpdateActualNumberOfPLayers(roomProperty.numberOfPlayers());

    QStringList names;
    for (int var = 0; var < roomProperty.players().size(); ++var)
    {
        names.append(roomProperty.players()[var].name());
    }

    SlotUpdatePlayersNames(names);
}

void RoomParameters::SlotUpdateRoomNumber(uint32_t roomNumber)
{
    ui->lbl_Number->setText(QString::number(roomNumber));
}

void RoomParameters::SlotUpdateSockets(const std::vector<uint32_t> &socketDescriptor)
{
    QString text;
    for (uint32_t var = 0; var < socketDescriptor.size(); ++var)
    {
       text += QString::number(socketDescriptor[var]) + "\n";
    }
    ui->lbl_Sockets->setText(text);
}

void RoomParameters::SlotUpdateMaxNumberOfPlayers(uint32_t maxNumber)
{
    ui->lbl_Max->setText(QString::number(maxNumber));
}

void RoomParameters::SlotUpdateActualNumberOfPLayers(uint32_t actualNumberOfPLayers)
{
    ui->lbl_Gamers->setText(QString::number(actualNumberOfPLayers));
}

void RoomParameters::SlotUpdatePlayersNames(const QStringList &playersNames)
{
    QString text;
    for (uint32_t var = 0; var < playersNames.size(); ++var)
    {
        text += playersNames[var] + "\n";
    }
    ui->lbl_Names->setText(text);
}
