#ifndef ROOM_H
#define ROOM_H

#include <QObject>
enum class RulesType {Ordinary, Realistyc};

class Room : public QObject
{
    Q_OBJECT
public:
    explicit Room(QObject *parent = 0);

    int numberOfPlayers() const;
    void setNumberOfPlayers(int numberOfPlayers);

    RulesType rulesType() const;
    void setRulesType(const RulesType &rulesType);

private:

    int _numberOfPlayers;
    RulesType _rulesType;

signals:

public slots:
};

#endif // ROOM_H
