#include "gamesettings.h"
#include <QTime>

unsigned int GameSettings::maximumNumberOfPlayers() const
{
    return _maximumNumberOfPlayers;
}

void GameSettings::setMaximumNumberOfPlayers(unsigned int maximumNumberOfPlayers)
{
    _maximumNumberOfPlayers = maximumNumberOfPlayers;
}

unsigned int GameSettings::totalTimeToMove() const
{
    return _totalTimeToMove;
}

void GameSettings::setTotalTimeToMove(unsigned int totalTimeToMove)
{
    _totalTimeToMove = totalTimeToMove;
}

unsigned int GameSettings::timeToThink() const
{
    return _timeToThink;
}

unsigned int GameSettings::timeForOpponentsDecision() const
{
    return _timeForOpponentsDecision;
}

void GameSettings::setTimeForOpponentsDecision(unsigned int timeForOpponentsDecision)
{
    _timeForOpponentsDecision = timeForOpponentsDecision;
}

unsigned int GameSettings::diplomacyTime() const
{
    return _diplomacyTime;
}

void GameSettings::setDiplomacyTime(unsigned int diplomacyTime)
{
    _diplomacyTime = diplomacyTime;
}

bool GameSettings::hasAddonClericalErrors() const
{
    return _hasAddonClericalErrors;
}

void GameSettings::setHasAddonClericalErrors(bool hasAddonClericalErrors)
{
    _hasAddonClericalErrors = hasAddonClericalErrors;
}

bool GameSettings::hasAddonWildAxe() const
{
    return _hasAddonWildAxe;
}

void GameSettings::setHasAddonWildAxe(bool hasAddonWildAxe)
{
    _hasAddonWildAxe = hasAddonWildAxe;
}

QString GameSettings::clientName() const
{
    return _clientName;
}

void GameSettings::setClientName(const QString &clientName)
{
    _clientName = clientName;
}

RulesType GameSettings::rulesType() const
{
    return _rulesType;
}

void GameSettings::setRulesType(const RulesType &rulesType)
{
    _rulesType = rulesType;
}

void GameSettings::setTimeToThink(unsigned int timeToThink)
{
    _timeToThink = timeToThink;
}

bool GameSettings::settingsCorrectionAllowed() const
{
    return _settingsCorrectionAllowed;
}

void GameSettings::setSettingsCorrectionAllowed(bool settingCorrectionAllowed)
{
    _settingsCorrectionAllowed = settingCorrectionAllowed;
}

GameSettings::GameSettings()
{
    _maximumNumberOfPlayers = 6;
    _totalTimeToMove = 120;
    _timeToThink = 15;
    _timeForOpponentsDecision = 10;
    _diplomacyTime = 30;
    _hasAddonClericalErrors = true;
    _hasAddonWildAxe = true;
    _settingsCorrectionAllowed = true;
    qsrand(static_cast<quint64>(QTime::currentTime().msecsSinceStartOfDay()));
    QString::number(qrand() % ((99999 + 1) - 1) + 1);
    _clientName = "EmpERRoR" + QString::number(qrand() % ((99999 + 1) - 1) + 1);

    _rulesType = RulesType::Automatic;
}

GameSettings::GameSettings(const GameSettings &settings)
{
    _maximumNumberOfPlayers = settings.maximumNumberOfPlayers();
    _totalTimeToMove = settings.totalTimeToMove();
    _timeToThink = settings.timeToThink();
    _timeForOpponentsDecision = settings.timeForOpponentsDecision();
    _diplomacyTime = settings.diplomacyTime();
    _hasAddonClericalErrors = settings.hasAddonClericalErrors();
    _hasAddonWildAxe = settings.hasAddonWildAxe();
    _clientName = settings.clientName();
    _rulesType = settings.rulesType();
}

void GameSettings::applyNewSettings(const GameSettings &settings)
{
    _maximumNumberOfPlayers = settings.maximumNumberOfPlayers();
    _totalTimeToMove = settings.totalTimeToMove();
    _timeToThink = settings.timeToThink();
    _timeForOpponentsDecision = settings.timeForOpponentsDecision();
    _diplomacyTime = settings.diplomacyTime();
    _hasAddonClericalErrors = settings.hasAddonClericalErrors();
    _hasAddonWildAxe = settings.hasAddonWildAxe();
    _clientName = settings.clientName();
    _rulesType = settings.rulesType();
}
