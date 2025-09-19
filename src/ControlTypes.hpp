#ifndef __CONTROL_TYPES_HPP__
#define __CONTROL_TYPES_HPP__

#include "Templates.hpp"

namespace BloodSword::Controls
{
    // control types
    enum class Type
    {
        NONE = -1,
        ANY = 0,
        WARRIOR,
        TRICKSTER,
        SAGE,
        ENCHANTER,
        IMRAGARN,
        SCROLL_UP,
        SCROLL_DOWN,
        MAP_NONE,
        MAP_UP,
        MAP_DOWN,
        MAP_LEFT,
        MAP_RIGHT,
        MAP_EXIT,
        BACK,
        RESET,
        CONFIRM,
        CANCEL,
        ROLL,
        PLAYER,
        ENEMY,
        LOCATION,
        ABILITY,
        ITEMS,
        CALL_TO_MIND,
        FORGET,
        FLEE,
        MOVE,
        HEAL,
        PLUS,
        MINUS,
        FIGHT,
        DEFEND,
        SHOOT,
        CAST,
        DAMAGE,
        ARCHERY,
        DODGING,
        QUARTERSTAFF,
        EXORCISM,
        ESP,
        PARANORMAL_SIGHT,
        LEVITATION,
        SPELLS,
        TEST,
        CHOICE,
        CONTINUE,
        MAP,
        ENCYCLOPEDIA,
        NEW,
        LOAD,
        SAVE,
        EXIT,
        GAME,
        PARTY,
        ABOUT,
        USE,
        DROP,
        TRADE,
        TAKE,
        TEMPORARY_OBSTACLE,
        VOLCANO_SPRAY,
        NIGHTHOWL,
        WHITE_FIRE,
        SWORDTHRUST,
        EYE_OF_THE_TIGER,
        IMMEDIATE_DELIVERANCE,
        MISTS_OF_DEATH,
        THE_VAMPIRE_SPELL,
        PILLAR_OF_SALT,
        SHEET_LIGHTNING,
        GHASTLY_TOUCH,
        NEMESIS_BOLT,
        SERVILE_ENTHRALMENT,
        SUMMON_FALTYN,
        PREDICTION,
        DETECT_ENCHANTMENT,
        START,
        SHURIKEN,
        NEXT,
        CENTER,
        SELECT,
        EQUIP,
        UNEQUIP,
        MONEY,
        INVENTORY,
        IDENTIFY,
        DRINK,
        READ,
        EAT,
        BATTLE_ORDER,
        QUIVER,
        STEEL_SCEPTRE,
        TROPHY,
        SKILLS,
        STATUS,
        INFO,
        UP,
        DOWN,
        LEFT,
        RIGHT,
        MENU,
        ACTION,
        HINTS
    };

    // list of control types
    typedef std::vector<Controls::Type> List;

    // list of spell control types
    Controls::List Spells = {
        Controls::Type::VOLCANO_SPRAY,
        Controls::Type::NIGHTHOWL,
        Controls::Type::WHITE_FIRE,
        Controls::Type::SWORDTHRUST,
        Controls::Type::EYE_OF_THE_TIGER,
        Controls::Type::IMMEDIATE_DELIVERANCE,
        Controls::Type::MISTS_OF_DEATH,
        Controls::Type::THE_VAMPIRE_SPELL,
        Controls::Type::PILLAR_OF_SALT,
        Controls::Type::SHEET_LIGHTNING,
        Controls::Type::GHASTLY_TOUCH,
        Controls::Type::NEMESIS_BOLT,
        Controls::Type::SERVILE_ENTHRALMENT,
        Controls::Type::SUMMON_FALTYN,
        Controls::Type::PREDICTION,
        Controls::Type::DETECT_ENCHANTMENT};

    // mapping of control types to strings
    BloodSword::ConstStrings<Controls::Type> TypeMapping = {
        {Controls::Type::NONE, "NONE"},
        {Controls::Type::MOVE, "MOVE"},
        {Controls::Type::FIGHT, "FIGHT"},
        {Controls::Type::QUARTERSTAFF, "QUARTERSTAFF"},
        {Controls::Type::SHOOT, "SHOOT"},
        {Controls::Type::SHURIKEN, "SHURIKEN"},
        {Controls::Type::FLEE, "FLEE"},
        {Controls::Type::SPELLS, "SPELLS"},
        {Controls::Type::ITEMS, "ITEMS"},
        {Controls::Type::STEEL_SCEPTRE, "STEEL SCEPTRE"}};

    // list of player control types
    Controls::List Players = {
        Controls::Type::WARRIOR,
        Controls::Type::TRICKSTER,
        Controls::Type::SAGE,
        Controls::Type::ENCHANTER,
        Controls::Type::IMRAGARN,
    };

    // map control type from string
    Controls::Type MapType(const char *control)
    {
        return BloodSword::Find(Controls::TypeMapping, control);
    }

    // map control type from string
    Controls::Type MapType(std::string control)
    {
        return Controls::MapType(control.c_str());
    }
}

#endif
