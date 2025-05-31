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
        ERROR,
        SWORD_HIT,
        STAFF_HIT,
        COMBAT_DAMAGE,
        BOW_RELEASE,
        WEAPON_THROW,
        CAST_SPELL
    };

    BloodSword::Mapping<Sound::Type> Mapping = {
        {Sound::Type::NONE, "NONE"},
        {Sound::Type::BUTTON_CLICK, "BUTTON CLICK"},
        {Sound::Type::DICE_ROLL, "DICE ROLL"},
        {Sound::Type::ERROR, "ERROR"},
        {Sound::Type::SWORD_HIT, "SWORD HIT"},
        {Sound::Type::STAFF_HIT, "STAFF HIT"},
        {Sound::Type::COMBAT_DAMAGE, "COMBAT DAMAGE"},
        {Sound::Type::BOW_RELEASE, "BOW RELEASE"},
        {Sound::Type::WEAPON_THROW, "WEAPON THROW"},
        {Sound::Type::CAST_SPELL, "CAST SPELL"}};

    BloodSword::UnorderedMap<Sound::Type, Mix_Chunk *> Assets = {};

    Sound::Type Map(const char *sound)
    {
        return BloodSword::Find(Sound::Mapping, sound);
    }

    Sound::Type Map(std::string sound)
    {
        return Sound::Map(sound.c_str());
    }

    // initialize mixer and load all sound assets
    void Load(std::string assets)
    {
        // Initialize SDL_mixer
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        {
            std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        }

        if (Mix_AllocateChannels(4) < 0)
        {
            std::cerr << "SDL_mixer unable to allocate mixing channels! SDL_mixer Error: " << Mix_GetError() << std::endl;
        }

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

    // play sound asset in specific channel (use -1 for first available)
    void Play(Sound::Type sound, int channel)
    {
        if (BloodSword::Has(Sound::Assets, sound))
        {
            // play sound in channel so it cancels sound
            Mix_PlayChannel(channel, Sound::Assets[sound], 0);
        }
    }

    // play sound asset
    void Play(Sound::Type sound)
    {
        Sound::Play(sound, 0);
    }

    // free all sound assets
    void Free()
    {
        for (auto &asset : Sound::Assets)
        {
            if (asset.second)
            {
                Mix_FreeChunk(asset.second);

                asset.second = nullptr;
            }
        }

        Sound::Assets.clear();

        // shutdown mixer
        Mix_CloseAudio();
    }
}

#endif