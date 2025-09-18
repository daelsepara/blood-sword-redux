#include "InterfaceStory.hpp"
#include "System.hpp"

// blood sword gamebook player
namespace BloodSword::Game
{
    // new game
    void NewGame(Graphics::Base &graphics)
    {
        Interface::NewGame(graphics, true);
    }

    // load game
    void LoadGame(Graphics::Base &graphics, Scene::Base &background)
    {
        // generate temporary party
        auto ranks = Story::LoadRanks(Interface::Settings["ranks"]);

        auto party = Party::Base();

        for (auto &character_class : Character::All)
        {
            auto character = Generate::Character(character_class, ranks[3]);

            party.Add(character);
        }

        auto load = Interface::LoadSaveGame(graphics, background, party, Controls::Type::LOAD, Asset::Map("LOAD"));

        if (load && Book::IsDefined(party.SaveLocation))
        {
            Interface::ReloadStory(graphics, party);

            auto current = Story::CurrentBook.Find(party.SaveLocation);

            if (current >= 0 && current < Story::CurrentBook.Sections.size())
            {
                Interface::Notify(graphics, background, Interface::MSG_LOADED);

                background = Scene::Base();

                Interface::ProcessStory(graphics, background, party, current);
            }
        }
    }

    // about module
    void AboutModule(Graphics::Base &graphics, Scene::Base &background)
    {
        auto about = Interface::Settings["about"];

        auto width = BloodSword::DecaTile;

        auto height = BloodSword::Wrap;

        Interface::ScrollableTextBox(graphics, background, Fonts::Normal, about, width, height, Color::S(Color::Active), TTF_STYLE_NORMAL, Color::Background, Color::Active, BloodSword::Border, Color::Active, Asset::Map("SWORDTHRUST"), true);
    }

    // show rules of the game
    void Help(Graphics::Base &graphics)
    {
        Interface::Topics(graphics, Asset::Map("SWORDTHRUST"));
    }

    // menu text (textures)
    Asset::TextureList RegenerateMenu(Graphics::Base &graphics, std::string module, int width)
    {
        auto menu = Graphics::CreateText(
            graphics,
            {Graphics::RichText("NEW GAME", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
             Graphics::RichText("LOAD GAME", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
             Graphics::RichText((std::string("MODULE: ") + module).c_str(), Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
             Graphics::RichText((std::string("ABOUT: ") + module).c_str(), Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
             Graphics::RichText("HELP", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
             Graphics::RichText("EXIT", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 0)});

        return menu;
    }

    // main menu
    void Menu(Graphics::Base &graphics)
    {
        auto logo = Asset::Create(graphics.Renderer, std::string(Interface::Settings["logo"]).c_str());

        auto width = BloodSword::PentaTile;

        auto height = BloodSword::HalfTile;

        auto module = Interface::CurrentModule();

        auto menu = RegenerateMenu(graphics, module, width);

        height = std::max(height, BloodSword::Height(menu));

        auto options = int(menu.size());

        auto input = Controls::User();

        auto done = false;

        auto menu_x = (graphics.Width - width) / 2;

        auto menu_y = (graphics.Height - (options * (height + BloodSword::Pad) - BloodSword::Pad)) / 2;

        auto logo_x = (graphics.Width - BloodSword::Width(logo)) / 2;

        auto logo_y = (menu_y - BloodSword::Height(logo) - BloodSword::Pad);

        auto offset = BloodSword::Pixel + 1;

        auto adjust = offset * 2;

        while (!done)
        {
            auto scene = Scene::Base();

            for (auto item = 0; item < options; item++)
            {
                auto id = item;

                auto y = menu_y + item * (height + BloodSword::Pad);

                // place text at the center of the box
                auto text_y = y + (height - BloodSword::Height(menu[item])) / 2;

                auto text_x = (graphics.Width - BloodSword::Width(menu[item])) / 2;

                // add box
                scene.Add(Scene::Element(menu_x, y, width, height, Color::Background, Color::Inactive, BloodSword::Pixel));

                // add text (texture)
                scene.VerifyAndAdd(Scene::Element(menu[item], Point(text_x, text_y)));

                // attach to a CHOICE control
                scene.Add(Controls::Base(Controls::Type::CHOICE, id, id, id, id > 0 ? id - 1 : id, id < options - 1 ? id + 1 : id, menu_x - offset, y - offset, width + adjust, height + adjust, Color::Active));
            }

            // add logo
            scene.VerifyAndAdd(Scene::Element(logo, Point(logo_x, logo_y)));

            input = Input::WaitForInput(graphics, scene, input);

            if (Input::Validate(input))
            {
                if (input.Type == Controls::Type::CHOICE)
                {
                    switch (input.Current)
                    {
                    case 0:
                        Game::NewGame(graphics);

                        break;
                    case 1:
                        Game::LoadGame(graphics, scene);

                        break;
                    case 2:

                        break;
                    case 3:
                        Game::AboutModule(graphics, scene);

                        break;
                    case 4:
                        Game::Help(graphics);

                        break;
                    default:
                        done = true;
                        break;
                    }

                    input.Clear();
                }
            }
        }

        BloodSword::Free(menu);

        BloodSword::Free(&logo);
    }

    // main loop
    int Main(int argc, char **argv)
    {
        auto return_code = 0;

        auto system = BloodSword::System();

        std::string session_name = "BloodSword " + BloodSword::Version();

        system.Initialize(session_name.c_str());

        try
        {
            Game::Menu(system.graphics);
        }
        catch (std::exception &e)
        {
            std::cerr << std::endl
                      << "BLOODSWORD ("
                      << Version()
                      << ") EXCEPTION: "
                      << e.what()
                      << std::endl
                      << std::endl;

            return_code = 1;
        }

        system.Shutdown();

        return return_code;
    }
}

int main(int argc, char **argv)
{
    return BloodSword::Game::Main(argc, argv);
}
