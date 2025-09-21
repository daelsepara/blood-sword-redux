#ifndef __SOUND_HPP__
#define __SOUND_HPP__

#include <fstream>
#include <string>

#include <SDL_mixer.h>

#include "nlohmann/json.hpp"
#include "Templates.hpp"
#include "ZipFileLibrary.hpp"

// functions for managing sound effects
namespace BloodSword::Sound
{
    // number of mixing channels
    const int CHANNELS = 4;

    // sound effect types
    enum class Type
    {
        NONE = 0,
        BUTTON_CLICK,
        DICE_ROLL,
        FAIL,
        SWORD_HIT,
        STAFF_HIT,
        COMBAT_DAMAGE,
        BOW_RELEASE,
        WEAPON_THROW,
        CAST_SPELL
    };

    // mapping of sound types to strings
    BloodSword::ConstStrings<Sound::Type> Mapping = {
        {Sound::Type::NONE, "NONE"},
        {Sound::Type::BUTTON_CLICK, "BUTTON CLICK"},
        {Sound::Type::DICE_ROLL, "DICE ROLL"},
        {Sound::Type::FAIL, "FAIL"},
        {Sound::Type::SWORD_HIT, "SWORD HIT"},
        {Sound::Type::STAFF_HIT, "STAFF HIT"},
        {Sound::Type::COMBAT_DAMAGE, "COMBAT DAMAGE"},
        {Sound::Type::BOW_RELEASE, "BOW RELEASE"},
        {Sound::Type::WEAPON_THROW, "WEAPON THROW"},
        {Sound::Type::CAST_SPELL, "CAST SPELL"}};

    // loaded sound assets
    BloodSword::UnorderedMap<Sound::Type, Mix_Chunk *> Assets = {};

    // map string to sound type
    Sound::Type Map(const char *sound)
    {
        return BloodSword::Find(Sound::Mapping, sound);
    }

    // map string to sound type
    Sound::Type Map(std::string sound)
    {
        return Sound::Map(sound.c_str());
    }

    // create surface from buffer
    Mix_Chunk *Load(char *buffer, size_t size)
    {
        auto rw = SDL_RWFromMem((void *)buffer, size);

        if (!rw)
        {
            return nullptr;
        }

        // create surface and close SDL_RWops
        auto chunk = Mix_LoadWAV_RW(rw, 1);

        return chunk;
    }

    // load sound from a file in zip
    Mix_Chunk *Load(const char *zip_file, const char *path)
    {
        // read file from zip archive
        auto sound = ZipFile::Read(zip_file, path);

        // create a modifiable buffer
        auto buffer = sound.data();

        // load chunk
        auto chunk = Sound::Load(buffer, sound.size());

        sound.clear();

        return chunk;
    }

    // load sound from json data (and zip archive)
    void Load(nlohmann::json &data, const char *zip_file)
    {
        auto is_zip = (zip_file != nullptr);

        if (!data["sounds"].is_null() && data["sounds"].is_array() && data["sounds"].size() > 0)
        {
            for (auto i = 0; i < data["sounds"].size(); i++)
            {
                auto asset = !data["sounds"][i]["id"].is_null() ? Sound::Map(std::string(data["sounds"][i]["id"])) : Sound::Type::NONE;

                auto path = !data["sounds"][i]["path"].is_null() ? std::string(data["sounds"][i]["path"]) : "";

                if (!path.empty() && asset != Sound::Type::NONE)
                {
                    auto sound = is_zip ? Sound::Load(zip_file, path.c_str()) : Mix_LoadWAV(path.c_str());

                    if (sound)
                    {
                        Sound::Assets[asset] = sound;
                    }
                }
            }
        }
    }

    // initialize mixer and load all sound assets
    void Load(std::string assets)
    {
        // Initialize SDL_mixer
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, Sound::CHANNELS, 2048) < 0)
        {
            std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        }

        if (Mix_AllocateChannels(Sound::CHANNELS) < 0)
        {
            std::cerr << "SDL_mixer unable to allocate mixing channels! SDL_mixer Error: " << Mix_GetError() << std::endl;
        }

        std::ifstream ifs(assets.c_str());

        if (ifs.good())
        {
            auto data = nlohmann::json::parse(ifs);

            Sound::Load(data, nullptr);

            ifs.close();
        }
    }

    void Load(std::string assets, std::string zip_file)
    {
        if (zip_file.empty())
        {
            Sound::Load(assets);
        }
        else
        {
            auto ifs = ZipFile::Read(zip_file.c_str(), assets.c_str());

            if (!ifs.empty())
            {
                auto data = nlohmann::json::parse(ifs);

                Sound::Load(data, zip_file.c_str());

                ifs.clear();
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
        Sound::Play(sound, -1);
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