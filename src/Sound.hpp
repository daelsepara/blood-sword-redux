#ifndef __SOUND_HPP__
#define __SOUND_HPP__

#include <fstream>
#include <string>
#include <SDL_mixer.h>

#include "nlohmann/json.hpp"
#include "Templates.hpp"

namespace BloodSword::Sound
{
    enum class Type
    {
        NONE = 0,
        BUTTON_CLICK,
        DICE_ROLL,
        ERROR
    };

    BloodSword::Mapping<Sound::Type> Mapping = {
        {Sound::Type::NONE, "NONE"},
        {Sound::Type::BUTTON_CLICK, "BUTTON CLICK"},
        {Sound::Type::DICE_ROLL, "DICE ROLL"},
        {Sound::Type::ERROR, "ERROR"}};

    BloodSword::UnorderedMap<Sound::Type, Mix_Chunk *> Assets = {};

    Sound::Type Map(const char *sound)
    {
        return BloodSword::Find(Sound::Mapping, sound);
    }

    Sound::Type Map(std::string sound)
    {
        return Sound::Map(sound.c_str());
    }

    // load all sound assets
    void Load(std::string assets)
    {
        std::ifstream ifs(assets.c_str());

        if (ifs.good())
        {
            auto data = nlohmann::json::parse(ifs);

            if (!data["sounds"].is_null() && data["sounds"].is_array() && data["sounds"].size() > 0)
            {
                for (auto i = 0; i < data["sounds"].size(); i++)
                {
                    auto asset = !data["sounds"][i]["id"].is_null() ? Sound::Map(std::string(data["sounds"][i]["id"])) : Sound::Type::NONE;

                    auto path = !data["sounds"][i]["path"].is_null() ? std::string(data["sounds"][i]["path"]) : "";

                    if (!path.empty() && asset != Sound::Type::NONE)
                    {
                        auto sound = Mix_LoadWAV(path.c_str());

                        if (sound)
                        {
                            Sound::Assets[asset] = sound;
                        }
                    }
                }
            }
        }
    }

    // play sound asset
    void Play(Sound::Type sound)
    {
        if (BloodSword::Has(Sound::Assets, sound))
        {
            Mix_PlayChannel(-1, Sound::Assets[sound], 0);
        }
    }

    // free all sound assets
    void Free()
    {
        for (auto asset : Sound::Assets)
        {
            if (asset.second)
            {
                Mix_FreeChunk(asset.second);
            }
        }

        Sound::Assets.clear();

        // shutdown mixer
        Mix_CloseAudio();
    }
}

#endif