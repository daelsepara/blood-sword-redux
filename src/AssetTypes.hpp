#ifndef __ASSET_TYPES_HPP__
#define __ASSET_TYPES_HPP__

#include <map>
#include <string>

namespace BloodSword::Asset
{
    enum class Type
    {
        NONE = -1,
        WARRIOR,
        TRICKSTER,
        SAGE,
        ENCHANTER,
        ASSASSIN,
        BARBARIAN,
        DIRGE_MAN,
        SKIAPYR,
        MAGUS_VYL,
        ICON_THE_UNGODLY,
        MAN_IN_BLUE,
        CORPSE,
        BOWMAN,
        DIRGE,
        ECHIDNA,
        SKRYMIR_THE_GIANT,
        GUARD,
        ADVENTURER,
        RANGER,
        SMEABORG,
        GIANT_SPIDER,
        ICON_THE_WARLOCK,
        NIGHT_ELF,
        QUEL,
        DEATH_FETISHIST,
        NEBULARON,
        YOUNG_SORCERESS,
        EIDOLON,
        GARGOYLE,
        UP,
        DOWN,
        LEFT,
        RIGHT,
        WALL,
        IMPASSABLE,
        EMPTY_SPACE,
        WHITE_SPACE,
        VERTICAL_SURFACE,
        WINDOW,
        DOOR,
        LEDGE,
        ROCKS,
        CHASM,
        BRIDGE,
        TOWER,
        MOVE,
        FIGHT,
        SHOOT,
        SHOOT_SHURIKEN,
        DEFEND,
        FLEE,
        CALL_TO_MIND,
        CAST_SPELL,
        DODGING,
        ARCHERY,
        QUARTERSTAFF,
        HEALING,
        ESP,
        PARANORMAL_SIGHT,
        SPELLS,
        EXIT,
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
        DETECT_ENCHANTMENT
    };

    std::map<Asset::Type, const char *> TypeMapping = {
        {Type::NONE, "NONE"},
        {Type::WARRIOR, "WARRIOR"},
        {Type::TRICKSTER, "TRICKSTER"},
        {Type::SAGE, "SAGE"},
        {Type::ENCHANTER, "ENCHANTER"},
        {Type::ASSASSIN, "ASSASSIN"},
        {Type::BARBARIAN, "BARBARIAN"},
        {Type::DIRGE_MAN, "DIRGE MAN"},
        {Type::SKIAPYR, "SKIAPYR"},
        {Type::MAGUS_VYL, "MAGUS VYL"},
        {Type::ICON_THE_UNGODLY, "ICON THE UNGODLY"},
        {Type::MAN_IN_BLUE, "MAN IN BLUE"},
        {Type::CORPSE, "CORPSE"},
        {Type::BOWMAN, "BOWMAN"},
        {Type::DIRGE, "DIRGE"},
        {Type::ECHIDNA, "ECHIDNA"},
        {Type::SKRYMIR_THE_GIANT, "SKRYMIR THE GIANT"},
        {Type::GUARD, "GUARD"},
        {Type::ADVENTURER, "ADVENTURER"},
        {Type::RANGER, "RANGER"},
        {Type::SMEABORG, "SMEABORG"},
        {Type::GIANT_SPIDER, "GIANT SPIDER"},
        {Type::ICON_THE_WARLOCK, "ICON THE WARLOCK"},
        {Type::NIGHT_ELF, "NIGHT ELF"},
        {Type::QUEL, "QUEL"},
        {Type::DEATH_FETISHIST, "DEATH FETISHIST"},
        {Type::NEBULARON, "NEBULARON"},
        {Type::EIDOLON, "EIDOLON"},
        {Type::GARGOYLE, "GARGOYLE"},
        {Type::UP, "UP"},
        {Type::DOWN, "DOWN"},
        {Type::LEFT, "LEFT"},
        {Type::RIGHT, "RIGHT"},
        {Type::WALL, "WALL"},
        {Type::IMPASSABLE, "IMPASSABLE"},
        {Type::EMPTY_SPACE, "EMPTY SPACE"},
        {Type::WHITE_SPACE, "WHITE SPACE"},
        {Type::VERTICAL_SURFACE, "VERTICAL SURFACE"},
        {Type::WINDOW, "WINDOW"},
        {Type::DOOR, "DOOR"},
        {Type::LEDGE, "LEDGE"},
        {Type::ROCKS, "ROCKS"},
        {Type::CHASM, "CHASM"},
        {Type::BRIDGE, "BRIDGE"},
        {Type::TOWER, "TOWER"},
        {Type::MOVE, "MOVE"},
        {Type::FIGHT, "FIGHT"},
        {Type::SHOOT, "SHOOT"},
        {Type::SHOOT_SHURIKEN, "SHOOT SHURIKEN"},
        {Type::DEFEND, "DEFEND"},
        {Type::FLEE, "FLEE"},
        {Type::CALL_TO_MIND, "CALL TO MIND"},
        {Type::CAST_SPELL, "CAST SPELL"},
        {Type::DODGING, "DODGING"},
        {Type::ARCHERY, "ARCHERY"},
        {Type::QUARTERSTAFF, "QUARTERSTAFF"},
        {Type::HEALING, "HEALING"},
        {Type::ESP, "ESP"},
        {Type::PARANORMAL_SIGHT, "PARANORMAL SIGHT"},
        {Type::SPELLS, "SPELLS"},
        {Type::EXIT, "EXIT"},
        {Type::VOLCANO_SPRAY, "VOLCANO SPRAY"},
        {Type::NIGHTHOWL, "NIGHTHOWL"},
        {Type::WHITE_FIRE, "WHITE FIRE"},
        {Type::SWORDTHRUST, "SWORDTHRUST"},
        {Type::EYE_OF_THE_TIGER, "EYE OF THE TIGER"},
        {Type::IMMEDIATE_DELIVERANCE, "IMMEDIATE DELIVERANCE"},
        {Type::MISTS_OF_DEATH, "MISTS OF DEATH"},
        {Type::THE_VAMPIRE_SPELL, "THE VAMPIRE SPELL"},
        {Type::PILLAR_OF_SALT, "PILLAR OF SALT"},
        {Type::SHEET_LIGHTNING, "SHEET LIGHTNING"},
        {Type::GHASTLY_TOUCH, "GHASTLY TOUCH"},
        {Type::NEMESIS_BOLT, "NEMESIS BOLT"},
        {Type::SERVILE_ENTHRALMENT, "SERVILE ENTHRALMENT"},
        {Type::SUMMON_FALTYN, "SUMMON FALTYN"},
        {Type::PREDICTION, "PREDICTION"},
        {Type::DETECT_ENCHANTMENT, "DETECT ENCHANTMENT"}};

    Asset::Type GetType(const char *asset)
    {
        auto result = Asset::Type::NONE;

        auto types = Asset::TypeMapping.begin();

        while (types != Asset::TypeMapping.end())
        {
            if (std::strcmp(types->second, asset) == 0)
            {
                result = types->first;

                break;
            }

            types++;
        }

        return result;
    }

    Asset::Type GetType(std::string asset)
    {
        return Asset::GetType(asset.c_str());
    }
}

#endif
