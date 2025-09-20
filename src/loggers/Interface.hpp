#ifndef __INTERFACE_LOGGER_HPP__
#define __INTERFACE_LOGGER_HPP__

#include "../Character.hpp"

namespace BloodSword::InterfaceLogger
{
    void LogSpellFailure(Character::Base &caster, Spells::Type spell)
    {
#if defined(DEBUG)
        if (!caster.HasCalledToMind(spell))
        {
            std::cerr << "[" << Spells::TypeMapping[spell] << "] NOT CALLED TO MIND" << std::endl;
        }
        else
        {
            std::cerr << "[" << Spells::TypeMapping[spell] << "] NOT IN GRIMOIRE" << std::endl;
        }
#endif
    }

    void LogOptions(Asset::List &assets, std::vector<int> &selection, std::string selected)
    {
#if defined(DEBUG)
        if (selection.size() > 0)
        {
            selected += " (";

            for (auto i = 0; i < selection.size(); i++)
            {
                if (i > 0)
                {
                    selected += ", ";
                }

                selected += std::string(Asset::TypeMapping[assets[selection[i]]]);
            }

            selected += ")";

            std::cerr << selected << std::endl;
        }
#endif
    }

    void LogChoice(const char *message, Asset::Type asset, int selected, int size)
    {
#if defined(DEBUG)
        std::cerr << "["
                  << message
                  << " "
                  << selected
                  << "] ["
                  << Asset::TypeMapping[asset]
                  << "] [SIZE] "
                  << size
                  << std::endl;
#endif
    }

    void LogPathToTarget(Point target, int path, int distance)
    {
#if defined(DEBUG)
        std::cerr << "[TARGET ("
                  << target.X
                  << ", "
                  << target.Y
                  << ")] [PATH] "
                  << path
                  << " [DIST] "
                  << distance
                  << std::endl;
#endif
    }

    void LogMoveTargets(const char *type, Target::Type character, int src_id, int dst_id, int path, int valid, int avail)
    {
#if defined(DEBUG)
        std::cerr << "["
                  << Target::Mapping[character]
                  << " "
                  << src_id
                  << "] [MOVE] "
                  << "[TARGET "
                  << type
                  << " "
                  << dst_id
                  << "] [PATH] "
                  << path
                  << " [DIST] "
                  << valid
                  << " [VULN] "
                  << avail
                  << std::endl;
#endif
    }
}

#endif
