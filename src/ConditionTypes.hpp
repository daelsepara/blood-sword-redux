#ifndef __CONDITION_TYPES_HPP__
#define __CONDITION_TYPES_HPP__

#include "Templates.hpp"

namespace BloodSword::Conditions
{
    enum class Type
    {
        NONE = -1,
        NORMAL,
        IN_PARTY,
        CHOOSE_PLAYER,
        CHOSEN_PLAYER,
        BATTLE_VICTORY,
        BATTLE_FLEE,
        BATTLE_ENTHRALMENT,
        BATTLE_VICTORY_OR_ENTHRALMENT,
        HAS_ITEM,
        USE_ITEM,
        DROP_ITEM,
        TAKE_ITEM,
        TAKE_ITEMS,
        GAIN_ENDURANCE,
        LOSE_ENDURANCE,
        CHOOSE_NUMBER,
        CHOSEN_NUMBER,
        PARTY_WOUNDED,
        PREVIOUS_LOCATION,
        TEST_ATTRIBUTE,
        ITEM_QUANTITY,
        LOSE_ALL,
        HAVE_COLLEAGUES,
        SOLO,
        GAIN_STATUS,
        LOSE_STATUS,
        FIRST,
        TEST_GAIN_STATUS,
        COUNT_STATUS,
        SELECT_MULTIPLE,
        CALL_TO_MIND,
        FORGET_SPELL,
        KALUGEN_SELECT,
        KALUGEN_SCORE,
        KALUGEN_LIST,
        KALUGEN_PLAY,
        DISCARD_ITEM,
        RECEIVE_ITEM
    };

    BloodSword::Mapping<Conditions::Type> TypeMapping = {
        {Conditions::Type::NONE, "NONE"},
        {Conditions::Type::NORMAL, "NORMAL"},
        {Conditions::Type::IN_PARTY, "IN PARTY"},
        {Conditions::Type::CHOOSE_PLAYER, "CHOOSE PLAYER"},
        {Conditions::Type::CHOSEN_PLAYER, "CHOSEN PLAYER"},
        {Conditions::Type::BATTLE_VICTORY, "BATTLE VICTORY"},
        {Conditions::Type::BATTLE_FLEE, "BATTLE FLEE"},
        {Conditions::Type::BATTLE_ENTHRALMENT, "BATTLE ENTHRALMENT"},
        {Conditions::Type::BATTLE_VICTORY_OR_ENTHRALMENT, "BATTLE VICTORY OR ENTHRALMENT"},
        {Conditions::Type::HAS_ITEM, "HAS ITEM"},
        {Conditions::Type::USE_ITEM, "USE ITEM"},
        {Conditions::Type::DROP_ITEM, "DROP ITEM"},
        {Conditions::Type::TAKE_ITEM, "TAKE ITEM"},
        {Conditions::Type::TAKE_ITEMS, "TAKE ITEMS"},
        {Conditions::Type::GAIN_ENDURANCE, "GAIN ENDURANCE"},
        {Conditions::Type::LOSE_ENDURANCE, "LOSE ENDURANCE"},
        {Conditions::Type::CHOOSE_NUMBER, "CHOOSE NUMBER"},
        {Conditions::Type::CHOSEN_NUMBER, "CHOSEN NUMBER"},
        {Conditions::Type::PARTY_WOUNDED, "PARTY WOUNDED"},
        {Conditions::Type::PREVIOUS_LOCATION, "PREVIOUS LOCATION"},
        {Conditions::Type::TEST_ATTRIBUTE, "TEST ATTRIBUTE"},
        {Conditions::Type::ITEM_QUANTITY, "ITEM QUANTITY"},
        {Conditions::Type::LOSE_ALL, "LOSE ALL"},
        {Conditions::Type::HAVE_COLLEAGUES, "HAVE COLLEAGUES"},
        {Conditions::Type::SOLO, "SOLO"},
        {Conditions::Type::GAIN_STATUS, "GAIN STATUS"},
        {Conditions::Type::FIRST, "FIRST"},
        {Conditions::Type::TEST_GAIN_STATUS, "TEST GAIN STATUS"},
        {Conditions::Type::COUNT_STATUS, "COUNT STATUS"},
        {Conditions::Type::LOSE_STATUS, "LOSE STATUS"},
        {Conditions::Type::SELECT_MULTIPLE, "SELECT MULTIPLE"},
        {Conditions::Type::CALL_TO_MIND, "CALL TO MIND"},
        {Conditions::Type::FORGET_SPELL, "FORGET SPELL"},
        {Conditions::Type::KALUGEN_SELECT, "KALUGEN SELECT"},
        {Conditions::Type::KALUGEN_SCORE, "KALUGEN SCORE"},
        {Conditions::Type::KALUGEN_LIST, "KALUGEN LIST"},
        {Conditions::Type::KALUGEN_PLAY, "KALUGEN PLAY"},
        {Conditions::Type::DISCARD_ITEM, "DISCARD ITEM"},
        {Conditions::Type::RECEIVE_ITEM, "RECEIVE ITEM"}};

    Conditions::Type Map(const char *Conditions)
    {
        return BloodSword::Find(Conditions::TypeMapping, Conditions);
    }

    Conditions::Type Map(std::string Conditions)
    {
        return Conditions::Map(Conditions.c_str());
    }
}

#endif
