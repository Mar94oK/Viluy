#ifndef CARD_H
#define CARD_H

#include <QString>
#include <QMap>
#include <map>
#include <cardsborderdefines.h>
#include <vector>
#include <QPoint>


/*
 * There are .csv tables of all the cards.
 * The_Game receives from Before_The_Game stock type (calculated basing on the settings selected in a
 * specific Before_The_Game menu).
 * According to this stock type it fills up the basis maps of the cards. This will be needed to show cards
 * in the game.
 * At the same time the Server does the same job and also makes the vectors from initial maps
 * taking away cards from basis maps.
 * So the program-client will have basis maps of cards and small maps of the cards that player has.
 * It will be two maps: things_in_the_game, races_professions and one vector "cards_on_the_hands";
 * About the other players client-programm knows things_in_the_game, races_professions.
 * There are some mechanics, that show the other player's hands to another or all the players. But for the
 * main time they are unknown to the program-client. So the vector!!! "cards_on_the_hands" exisists, but is filled
 * with a specific card_type "rubashka". Client knows the quantity, but not the fullfilment.
 * "cards_on_the_hands" should be the vector, since the order might be important: there are mechanics, that allows
 * to take some cards from player. Might be a good idea to allow player to change cards in places from time to time.
 *
*/



//may be it is not neccessary to add <cardsborderdefines.h>, since keys can be received directly form formed maps.




enum class cardAddon {Basic, WildAxe, ClericalErrors };
enum class doorType {Monster, MonsterAmplifier, Curse, Race, Profession, SpecialMechanic };
enum class treasureType {Armor, ArmorAmplifier, BattleAmplifier, LevelUp, SpecialMechanic, ThingsAmplifiers, Weapon };


#define DEFAULT_GAME_STOCK

//Depending on the game stock!!!!!

#ifdef DEFAULT_GAME_STOCK
//There will be five Races and five Classes (Here and Further Renamed to Professions) in the Basic Game
//Player can only have 4 abilities from Races and 4 abilities from Classes;
//Therefore there will be an array of each group;
//Passive abilities should have a special sign near Avatar, active abilities should have not only the sign,
//but also the button in the specific menu (the total of six buttons for Mage-Priest & Halfling-Ork for ex).
//There will be maximum of
// Race-Active-Abilities == 2
// Race-Passive-Abilities == 4
// Profession-Active-Abilities == 4
// Profession-Passive-Abilities == 2
//Therefore maximum of
// Passive_labels == 6
// Active_labels == 6 (will be made as buttons in a spec. menu)



enum class Race { Human, Elf, Dwarf, Gnome, Halfling, Ork };
enum class Profession {No_Profession, Warrior, Thief, Cleric, Bard, Wizard}; //using "profession instead of class



enum class Race_Active_Abilities {No_Abilities, Power_of_the_Lonesome, The_Second_Chance, Rejecting};
enum class Race_Passive_Abilities {No_Abilities, Big_Guy, Big_Hands, Escaping, Semi_Altruism,
                                   Power_of_GN_Nose_Auto_Escape, Businessman, Overpower};

enum class Profession_Active_Abilities {No_Abilities,  Ressurection, Banishment, Theft, Shrinking, Berserking, Enchanting,
                             Flight, Pacification };
enum class Profession_Passive_Abilities {No_Abilities, Power_of_the_Parity, Bards_Luck };

enum class Abilities_Keys_Races {elven_ability_1, elven_ability_2, dwarfs_ability_1, dwarfs_ability_2,
                           gnomes_ability_1, gnomes_ability_2, halflings_ability_1, halflings_ability_2,
                           orks_ability_1, orks_ability_2 };
enum class Abilities_Keys_Professions {warriors_ability_1, warriors_ability_2, rogue_ability_1, rogue_ability_2,
                                      priests_ability_1, priests_ability_2, bards_ability_1, bards_ability_2,
                                      mage_ability_1, mage_ability_2};

enum class Players_Sex {Man, Woman};

enum class Card_Type {Monster, Monsters_Amplifier, Curse, Race, Profession, Special_Mechanics};

enum class Body_Part {Head, Armor, Feet};

enum class Size {Big, Small};

enum class Time_To_Use {ownFight, anytime, eachFight, immediately, afterFight,
                       dieWasPlayed, ownMove, exceptFight};

enum class Additional_Request {noCompanion, failedToFlee,
                              succeededToFlee, win, onceReceived,
                              haveCompanion, thereIsDwarf, noRequest};



#endif


struct CardsKeysBorders {
    int lowerBorder;
    int upperBorder;
};





//class-prototype (draft only)
//Despite of cards having somekind of the same nature,
//the Game mechanics can be made much easier to implement if I am not using the cards as childs of prototype
// "card", but make them completely separate natures as they ARE in real -
// I am expecting that inheritance is not essential here, with such cards;

//use this as the elements of arrays, controlled by The Game.
typedef std::pair<bool, unsigned int> SimpleCard;

struct PositionedCard {
    SimpleCard card;
    QPoint positionTopLeft;
    QPoint positionBottomRight;
};


class GameCard
{

public:
    GameCard();
    GameCard (int card_ID, QString picture_address, QString card_name, cardAddon card_Addon);

    int card_ID() const;
    void setCard_ID(int card_ID);

    QString card_Picture_address() const;
    void setCard_Picture_address(const QString &card_Picture_address);

    QString card_Name() const;
    void setCard_Name(const QString &card_Name);

    cardAddon cardAddOn() const;
    void setCardAddOn(const cardAddon &cardAddOn);

protected:

    int _card_ID;
    QString _card_Picture_address;
    QString _card_Name;
    cardAddon _cardAddOn;




};

class gameCardDoorMonster {

    int _cardID;
    QString _pictureAddress;
    QString _cardName;
    QString _monsterName;
    cardAddon _addOn;
    doorType _type;

    bool _isUndead;
    bool _fromHell;

    int _monsterLevel;

    int _strongAgainstHuman;
    int _strongAgainstElf;
    int _strongAgainstHalfling;
    int _strongAgainstDwarf;
    int _strongAgainstOrk;
    int _strongAgainstGnome;
    int _strongAgainstNoClass;
    int _strongAgainstHalfBreed;
    int _strongAgainstSuperMunchkin;
    int _strongAgainstWoman;
    int _strongAgainstWarrior;
    int _strongAgainstWizard;
    int _strongAgainstThief;
    int _strongAgainstBard;
    int _strongAgainstCleric;
    int _strongAgainstSaturday;

    bool _dontFightWithWoman;
    bool _dontFightWithThief;
    bool _dontFightWithElf;
    bool _dontFightwithOrk;

    int _dontFightTillLevel;

    bool _specialMechanicAgainstAll;
    bool _specialMechanicAgainstWoman;
    bool _specialMechanicAgainstCleric;
    bool _specialMechanicAginstThief;
    bool _specialMechanicAgainstElf;
    bool _specialMechanicAgainstWizard;
    bool _specialMechanicAgainstHalfling;


public:

    //I will try default constructor with no parameters;
    //The parameters will be filled up in the Parser;
    gameCardDoorMonster() {}



    int cardID() const;
    void setCardID(int cardID);
    QString pictureAddress() const;
    void setPictureAddress(const QString &pictureAddress);
    QString cardName() const;
    void setCardName(const QString &cardName);
    QString monsterName() const;
    void setMonsterName(const QString &monsterName);
    cardAddon addOn() const;
    void setAddOn(const cardAddon &addOn);
    doorType type() const;
    void setType(const doorType &type);
    bool isUndead() const;
    void setIsUndead(bool isUndead);
    bool fromHell() const;
    void setFromHell(bool fromHell);
    int monsterLevel() const;
    void setMonsterLevel(int monsterLevel);
    int strongAgainstHuman() const;
    void setStrongAgainstHuman(int strongAgainstHuman);
    int strongAgainstElf() const;
    void setStrongAgainstElf(int strongAgainstElf);
    int strongAgainstHalfling() const;
    void setStrongAgainstHalfling(int strongAgainstHalfling);
    int strongAgainstDwarf() const;
    void setStrongAgainstDwarf(int strongAgainstDwarf);
    int strongAgainstOrk() const;
    void setStrongAgainstOrk(int strongAgainstOrk);
    int strongAgainstGnome() const;
    void setStrongAgainstGnome(int strongAgainstGnome);
    int strongAgainstNoClass() const;
    void setStrongAgainstNoClass(int strongAgainstNoClass);
    int strongAgainstHalfBreed() const;
    void setStrongAgainstHalfBreed(int strongAgainstHalfBreed);
    int strongAgainstSuperMunchkin() const;
    void setStrongAgainstSuperMunchkin(int strongAgainstSuperMunchkin);
    int strongAgainstWoman() const;
    void setStrongAgainstWoman(int strongAgainstWoman);
    int strongAgainstWarrior() const;
    void setStrongAgainstWarrior(int strongAgainstWarrior);
    int strongAgainstWizard() const;
    void setStrongAgainstWizard(int strongAgainstWizard);
    int strongAgainstThief() const;
    void setStrongAgainstThief(int strongAgainstThief);
    int strongAgainstBard() const;
    void setStrongAgainstBard(int strongAgainstBard);
    int strongAgainstCleric() const;
    void setStrongAgainstCleric(int strongAgainstCleric);
    int strongAgainstSaturday() const;
    void setStrongAgainstSaturday(int strongAgainstSaturday);
    bool dontFightWithWoman() const;
    void setDontFightWithWoman(bool dontFightWithWoman);
    bool dontFightWithThief() const;
    void setDontFightWithThief(bool dontFightWithThief);
    bool dontFightWithElf() const;
    void setDontFightWithElf(bool dontFightWithElf);
    bool dontFightwithOrk() const;
    void setDontFightwithOrk(bool dontFightwithOrk);
    int dontFightTillLevel() const;
    void setDontFightTillLevel(int dontFightTillLevel);
    bool specialMechanicAgainstAll() const;
    void setSpecialMechanicAgainstAll(bool specialMechanicAgainstAll);
    bool specialMechanicAgainstWoman() const;
    void setSpecialMechanicAgainstWoman(bool specialMechanicAgainstWoman);
    bool specialMechanicAgainstCleric() const;
    void setSpecialMechanicAgainstCleric(bool specialMechanicAgainstCleric);
    bool specialMechanicAginstThief() const;
    void setSpecialMechanicAginstThief(bool specialMechanicAginstThief);
    bool specialMechanicAgainstElf() const;
    void setSpecialMechanicAgainstElf(bool specialMechanicAgainstElf);
    bool specialMechanicAgainstWizard() const;
    void setSpecialMechanicAgainstWizard(bool specialMechanicAgainstWizard);
    bool specialMechanicAgainstHalfling() const;
    void setSpecialMechanicAgainstHalfling(bool specialMechanicAgainstHalfling);
};

class gameCardDoorAmplifier {

    int _cardID;
    QString _pictureAddress;
    QString _cardName;
    cardAddon _addOn;
    doorType _type;

    bool _isSleeping;
    bool _isEnraged;
    bool _isAncient;
    bool _isBaby;
    bool _isUndead;
    bool _isFromHell;
    bool _isVeryDepressed;

    int amplification;
    int additionalTreasures;



public:
    int cardID() const;
    void setCardID(int cardID);
    QString pictureAddress() const;
    void setPictureAddress(const QString &pictureAddress);
    QString cardName() const;
    void setCardName(const QString &cardName);
    QString monsterName() const;
    void setMonsterName(const QString &monsterName);
    cardAddon addOn() const;
    void setAddOn(const cardAddon &addOn);
    doorType type() const;
    void setType(const doorType &type);
    bool isSleeping() const;
    void setIsSleeping(bool isSleeping);
    bool isEnraged() const;
    void setIsEnraged(bool isEnraged);
    bool isAncient() const;
    void setIsAncient(bool isAncient);
    bool isBaby() const;
    void setIsBaby(bool isBaby);
    bool isUndead() const;
    void setIsUndead(bool isUndead);
    bool isFromHell() const;
    void setIsFromHell(bool isFromHell);
    int getAmplification() const;
    void setAmplification(int value);
    int getAdditionalTreasures() const;
    void setAdditionalTreasures(int value);

    bool getIsVeryDepressed() const;
    void setIsVeryDepressed(bool isVeryDepressed);
};

class gameCardDoorCurse {

    int _cardID;
    QString _pictureAddress;
    QString _cardName;
    cardAddon _addOn;
    doorType _type;

    QString _mechanic;
    int _curseMechanicID;

public:


    int cardID() const;
    void setCardID(int cardID);
    QString pictureAddress() const;
    void setPictureAddress(const QString &pictureAddress);
    QString cardName() const;
    void setCardName(const QString &cardName);
    cardAddon addOn() const;
    void setAddOn(const cardAddon &addOn);
    doorType type() const;
    void setType(const doorType &type);
    int getCurseMechanicID() const;
    void setCurseMechanicID(int value);
    QString getMechanic() const;
    void setMechanic(const QString &mechanic);
};

class gameCardDoorProfession {

    int _cardID;
    QString _pictureAddress;
    QString _cardName;
    cardAddon _addOn;
    doorType _type;
    Profession _profession;

public:




    int cardID() const;
    void setCardID(int cardID);
    QString pictureAddress() const;
    void setPictureAddress(const QString &pictureAddress);
    QString cardName() const;
    void setCardName(const QString &cardName);
    cardAddon addOn() const;
    doorType type() const;
    void setType(const doorType &type);
    Profession profession() const;
    void setProfession(const Profession &profession);
    void setAddOn(const cardAddon &addOn);
};

class gameCardDoorRace {

    int _cardID;
    QString _pictureAddress;
    QString _cardName;
    cardAddon _addOn;
    doorType _type;
    Race _race;

public:


    int cardID() const;
    void setCardID(int cardID);
    QString pictureAddress() const;
    void setPictureAddress(const QString &pictureAddress);
    QString cardName() const;
    void setCardName(const QString &cardName);
    cardAddon addOn() const;
    void setAddOn(const cardAddon &addOn);
    doorType type() const;
    void setType(const doorType &type);
    Race race() const;
    void setRace(const Race &race);
};

class gameCardDoorSpecialMechanic {

    int _cardID;
    QString _pictureAddress;
    QString _cardName;
    cardAddon _addOn;
    doorType _type;
    int _specialFunctionId;

public:




    int cardID() const;
    void setCardID(int cardID);
    QString pictureAddress() const;
    void setPictureAddress(const QString &pictureAddress);
    QString cardName() const;
    void setCardName(const QString &cardName);
    cardAddon addOn() const;
    void setAddOn(const cardAddon &addOn);
    doorType type() const;
    void setType(const doorType &type);
    int specialFunctionId() const;
    void setSpecialFunctionId(int specialFunctionId);
};




class gameCardTreasureArmor {

    int _cardID;
    QString _pictureAddress;
    QString _cardName;
    cardAddon _addOn;
    treasureType _type;

    Body_Part _part;
    Size _size;

    int _bonus;
    int _additionalBonusforElf;
    int _additionalBonusforOrk;

    bool _isOnlyForHuman;
    bool _isOnlyForWizard;
    bool _isOnlyForDwarf;
    bool _isOnlyForGnome;

    bool _isRestrictedToWizard;
    bool _isRestrictedToGnome;

    bool _hasSpecialMechanic;

    int _price;

    int _bonusToFleeing;

    bool _isCombined;


public:



    int cardID() const;
    void setCardID(int cardID);
    QString pictureAddress() const;
    void setPictureAddress(const QString &pictureAddress);
    QString cardName() const;
    void setCardName(const QString &cardName);
    cardAddon addOn() const;
    void setAddOn(const cardAddon &addOn);
    treasureType type() const;
    void setType(const treasureType &type);
    Body_Part part() const;
    void setPart(const Body_Part &part);
    Size size() const;
    void setSize(const Size &size);
    int bonus() const;
    void setBonus(int bonus);
    int additionalBonusforElf() const;
    void setAdditionalBonusforElf(int additionalBonusforElf);
    int additionalBonusforOrk() const;
    void setAdditionalBonusforOrk(int additionalBonusforOrk);
    bool isOnlyForHuman() const;
    void setIsOnlyForHuman(bool isOnlyForHuman);
    bool isOnlyForWizard() const;
    void setIsOnlyForWizard(bool isOnlyForWizard);
    bool isOnlyForDwarf() const;
    void setIsOnlyForDwarf(bool isOnlyForDwarf);
    bool isOnlyForGnome() const;
    void setIsOnlyForGnome(bool isOnlyForGnome);
    bool isRestrictedToWizard() const;
    void setIsRestrictedToWizard(bool isRestrictedToWizard);
    bool isRestrictedToGnome() const;
    void setIsRestrictedToGnome(bool isRestrictedToGnome);
    bool hasSpecialMechanic() const;
    void setHasSpecialMechanic(bool hasSpecialMechanic);
    int price() const;
    void setPrice(int price);
    int bonusToFleeing() const;
    void setBonusToFleeing(int bonusToFleeing);
    bool isCombined() const;
    void setIsCombined(bool isCombined);
};


class gameCardTreasureArmorAmplifier {

    int _cardID;
    QString _pictureAddress;
    QString _cardName;
    cardAddon _addOn;
    treasureType _type;

    int _bonus;

public:

    int cardID() const;
    void setCardID(int cardID);
    QString pictureAddress() const;
    void setPictureAddress(const QString &pictureAddress);
    QString cardName() const;
    void setCardName(const QString &cardName);
    cardAddon addOn() const;
    void setAddOn(const cardAddon &addOn);
    treasureType type() const;
    void setType(const treasureType &type);
    int bonus() const;
    void setBonus(int bonus);
};


class gameCardTreasureBattleAmplifier {

    int _cardID;
    QString _pictureAddress;
    QString _cardName;
    cardAddon _addOn;
    treasureType _type;

    int _bonus;
    bool _isPotion;
    bool _hasSpecialMechanic;


public:



    int cardID() const;
    void setCardID(int cardID);
    QString pictureAddress() const;
    void setPictureAddress(const QString &pictureAddress);
    QString cardName() const;
    void setCardName(const QString &cardName);
    cardAddon addOn() const;
    void setAddOn(const cardAddon &addOn);
    treasureType type() const;
    void setType(const treasureType &type);
    int bonus() const;
    void setBonus(int bonus);
    bool isPotion() const;
    void setIsPotion(bool isPotion);
    bool hasSpecialMechanic() const;
    void setHasSpecialMechanic(bool hasSpecialMechanic);
};

class gameCardTreasureLevelUp {

    int _cardID;
    QString _pictureAddress;
    QString _cardName;
    cardAddon _addOn;
    treasureType _type;

    bool _hasSpecialMechanic;

public:




    int cardID() const;
    void setCardID(int cardID);
    QString pictureAddress() const;
    void setPictureAddress(const QString &pictureAddress);
    QString cardName() const;
    void setCardName(const QString &cardName);
    cardAddon addOn() const;
    void setAddOn(const cardAddon &addOn);
    treasureType type() const;
    void setType(const treasureType &type);
    bool hasSpecialMechanic() const;
    void setHasSpecialMechanic(bool hasSpecialMechanic);
};



class gameCardTreasureSpecialMechanic {

    int _cardID;
    QString _pictureAddress;
    QString _cardName;
    cardAddon _addOn;
    treasureType _type;

    int _price;
    bool _isPotion;
    Time_To_Use _timeToUse;
    Additional_Request _additionalRequest;

public:






    int cardID() const;
    void setCardID(int cardID);
    QString pictureAddress() const;
    void setPictureAddress(const QString &pictureAddress);
    QString cardName() const;
    void setCardName(const QString &cardName);
    cardAddon addOn() const;
    void setAddOn(const cardAddon &addOn);
    treasureType type() const;
    void setType(const treasureType &type);
    int price() const;
    void setPrice(int price);
    bool isPotion() const;
    void setIsPotion(bool isPotion);
    Time_To_Use timeToUse() const;
    void setTimeToUse(const Time_To_Use &timeToUse);
    Additional_Request additionalRequest() const;
    void setAdditionalRequest(const Additional_Request &additionalRequest);
};



class gameCardTreasureThingsAmplifiers {

    int _cardID;
    QString _pictureAddress;
    QString _cardName;
    cardAddon _addOn;
    treasureType _type;

    Size _size;
    int _bonus;

    bool _hasSpecialMechanic;
    bool _isKnees;
    bool _isOnlyForHalfling;
    bool _isOnlyForThief;
    bool _isOnlyForCleric;
    bool _isOnlyForWizard;

    bool _isRestrictedToWarrior;
    bool _isRestrictedToCleric;
    bool _isRestrictedToThief;

    int _price;
    int _bonusToFlee;
    int _bonusToHands;
    int _bonusToMan;
    int _bonusToWoman;

    bool _allowedToWearDwarven;
    bool _allowedToWearElven;


public:





    int cardID() const;
    void setCardID(int cardID);
    QString pictureAddress() const;
    void setPictureAddress(const QString &pictureAddress);
    QString cardName() const;
    void setCardName(const QString &cardName);
    cardAddon addOn() const;
    void setAddOn(const cardAddon &addOn);
    treasureType type() const;
    void setType(const treasureType &type);
    Size size() const;
    void setSize(const Size &size);
    int bonus() const;
    void setBonus(int bonus);
    bool hasSpecialMechanic() const;
    void setHasSpecialMechanic(bool hasSpecialMechanic);
    bool isKnees() const;
    void setIsKnees(bool isKnees);
    bool isOnlyForHalfling() const;
    void setIsOnlyForHalfling(bool isOnlyForHalfling);
    bool isOnlyForThief() const;
    void setIsOnlyForThief(bool isOnlyForThief);
    bool isOnlyForCleric() const;
    void setIsOnlyForCleric(bool isOnlyForCleric);
    bool isOnlyForWizard() const;
    void setIsOnlyForWizard(bool isOnlyForWizard);
    bool isRestrictedToWarrior() const;
    void setIsRestrictedToWarrior(bool isRestrictedToWarrior);
    bool isRestrictedToCleric() const;
    void setIsRestrictedToCleric(bool isRestrictedToCleric);
    bool isRestrictedToThief() const;
    void setIsRestrictedToThief(bool isRestrictedToThief);
    int price() const;
    void setPrice(int price);
    int bonusToFlee() const;
    void setBonusToFlee(int bonusToFlee);
    int bonusToHands() const;
    void setBonusToHands(int bonusToHands);
    int bonusToMan() const;
    void setBonusToMan(int bonusToMan);
    int bonusToWoman() const;
    void setBonusToWoman(int bonusToWoman);
    bool allowedToWearDwarven() const;
    void setAllowedToWearDwarven(bool allowedToWearDwarven);
    bool allowedToWearElven() const;
    void setAllowedToWearElven(bool allowedToWearElven);
};


class gameCardTreasureWeapon {

    int _cardID;
    QString _pictureAddress;
    QString _cardName;
    cardAddon _addOn;
    treasureType _type;

    int _hands;
    Size _size;
    int _bonus;

    bool _isOnlyForElf;
    bool _isOnlyForMan;
    bool _isOnlyForDwarf;
    bool _isOnlyForHuman;
    bool _isOnlyForWizard;
    bool _isOnlyForWoman;
    bool _isOnlyForWarrior;
    bool _isOnlyForOrk;
    bool _isOnlyForThief;
    bool _isOnlyForHalfling;
    bool _isOnlyForGnome;
    bool _isOnlyForBard;
    bool _isOnlyForCleric;

    bool _hasSpecialMechanic;

    int _price;

    int _bonusToFlee;
    int _additionalBonusAgainstUndead;

    bool _automaticLooseToCalmadzila;

    int _additionalBonusAgainst_J;

    bool _addingClericalAbility;

    bool _addingThiefAbility;

public:








    int cardID() const;
    void setCardID(int cardID);
    QString pictureAddress() const;
    void setPictureAddress(const QString &pictureAddress);
    QString cardName() const;
    void setCardName(const QString &cardName);
    cardAddon addOn() const;
    void setAddOn(const cardAddon &addOn);
    treasureType type() const;
    void setType(const treasureType &type);
    int hands() const;
    void setHands(int hands);
    int bonus() const;
    void setBonus(int bonus);
    bool isOnlyForElf() const;
    void setIsOnlyForElf(bool isOnlyForElf);
    bool isOnlyForMan() const;
    void setIsOnlyForMan(bool isOnlyForMan);
    bool isOnlyForDwarf() const;
    void setIsOnlyForDwarf(bool isOnlyForDwarf);
    bool isOnlyForHuman() const;
    void setIsOnlyForHuman(bool isOnlyForHuman);
    bool isOnlyForWizard() const;
    void setIsOnlyForWizard(bool isOnlyForWizard);
    bool isOnlyForWoman() const;
    void setIsOnlyForWoman(bool isOnlyForWoman);
    bool isOnlyForWarrior() const;
    void setIsOnlyForWarrior(bool isOnlyForWarrior);
    bool isOnlyForOrk() const;
    void setIsOnlyForOrk(bool isOnlyForOrk);
    bool isOnlyForThief() const;
    void setIsOnlyForThief(bool isOnlyForThief);
    bool isOnlyForHalfling() const;
    void setIsOnlyForHalfling(bool isOnlyForHalfling);
    bool isOnlyForGnome() const;
    void setIsOnlyForGnome(bool isOnlyForGnome);
    bool isOnlyForBard() const;
    void setIsOnlyForBard(bool isOnlyForBard);
    bool hasSpecialMechanic() const;
    void setHasSpecialMechanic(bool hasSpecialMechanic);
    int price() const;
    void setPrice(int price);
    int bonusToFlee() const;
    void setBonusToFlee(int bonusToFlee);
    int additionalBonusAgainstUndead() const;
    void setAdditionalBonusAgainstUndead(int additionalBonusAgainstUndead);
    bool automaticLooseToCalmadzila() const;
    void setAutomaticLooseToCalmadzila(bool automaticLooseToCalmadzila);
    int additionalBonusAgainst_J() const;
    void setAdditionalBonusAgainst_J(int additionalBonusAgainst_J);
    bool addingClericalAbility() const;
    void setAddingClericalAbility(bool addingClericalAbility);
    bool addingThiefAbility() const;
    void setAddingThiefAbility(bool addingThiefAbility);
    bool isOnlyForCleric() const;
    void setIsOnlyForCleric(bool isOnlyForCleric);
    Size size() const;
    void setSize(const Size &size);
};


class Game_Card_Stock


{
public:
    Game_Card_Stock();
    Game_Card_Stock (int stock_type, int total_cards_to_be_played, int cards_left_to_be_played, int time_replayed);

  int type() const;
  void setType(int type);

private:

  int _type;  //make it as a specific type!
  int _total_cards; //how many card were in an initial stock
  int _cards_left; //how many cards left in a game;
  int _time_replayed; //how many times were the stock replayed (somekind of a debug info)

  std::vector<GameCard> _stock;


};






#endif // CARD_H


