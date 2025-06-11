#ifndef __SYSTEM_HPP__
#define __SYSTEM_HPP__

#include "nlohmann/json.hpp"
#include "Interface.hpp"

namespace BloodSword
{
    class System
    {
    public:
        Graphics::Base graphics;

        // initialize all subsytems
        void Initialize(const char *session_name)
        {
            this->graphics = Graphics::Initialize(session_name);

            // Initialize random number generator
            Engine::InitializeRNG();

            // Load default module
            Interface::LoadModules();

            // Load settings of current module
            Interface::LoadSettings(this->graphics, Interface::SettingsFile);

            // initialize save games list
            Interface::InitializeGamesList();

            // flush inputs
            Input::Flush();
        }

        void Initialize()
        {
            this->Initialize("Blood Sword");
        }

        // shutdown all subsystems
        void Shutdown()
        {
            Interface::UnloadAssets();

            Graphics::Quit(this->graphics);
        }

        System() {}

        System(const char *session_name)
        {
            this->Initialize(session_name);
        }
    };
}

#endif
