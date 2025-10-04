#ifndef __SYSTEM_HPP__
#define __SYSTEM_HPP__

#include "nlohmann/json.hpp"
#include "Interface.hpp"

// main game system class
namespace BloodSword
{
    // main system class
    class System
    {
    public:
        // graphics system
        Graphics::Base graphics;

        // initialize all subsytems
        void Initialize(const char *session_name)
        {
            // initialize random number generator
            Engine::InitializeRNG();

            // load default module
            Interface::LoadModules();

            // initialize graphics system
            this->graphics = Graphics::Initialize(session_name);

            // load settings of current module
            if (Interface::Zipped)
            {
                Interface::LoadSettings(this->graphics, Interface::SettingsFile, Interface::ZipFile);
            }
            else
            {
                Interface::LoadSettings(this->graphics, Interface::SettingsFile);
            }

            // initialize save games list
            Interface::InitializeGamesList();

            // flush inputs
            Input::Flush();
        }

        // initialize all subsystems with default session name
        void Initialize()
        {
            this->Initialize("Blood Sword");
        }

        // shutdown all subsystems
        void Shutdown()
        {
            // close all assets
            Interface::UnloadAssets();

            // close subsystems
            Graphics::Quit(this->graphics);
        }

        System() {}

        // initialize all subsystems with specified session name
        System(const char *session_name)
        {
            this->Initialize(session_name);
        }
    };
}

#endif
