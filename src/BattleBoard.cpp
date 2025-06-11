#include "InterfaceBattle.hpp"
#include "System.hpp"

// framework for Battle Board
namespace BloodSword::BattleBoard
{
    void Menu(Graphics::Base &graphics, const char *battle_file)
    {
        auto party = Interface::CreateParty(graphics, Book::Number::BOOK1, false);

        if (party.Count() == 0)
        {
            return;
        }

        // load specific battle
        auto battle = Battle::Load(battle_file);

        auto scene = Scene::Base();

        // load story (items)
        Story::Load(Interface::Settings["adventure"]);

        // set battle order
        if (party.Count() > 1)
        {
            Interface::BattleOrder(graphics, scene, party);
        }

        if (battle.IsDefined())
        {
            if (Book::IsDefined(battle.Location))
            {
                party.Set(battle.Location);
            }

            auto result = Interface::RenderBattle(graphics, battle, party);

            Graphics::RichText message;

            auto border = Color::Active;

            switch (result)
            {
            case Battle::Result::VICTORY:

                message = Graphics::RichText("YOUR PARTY IS VICTORIOUS!", Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0);

                break;

            case Battle::Result::DEFEAT:

                message = party.Count() > 1 ? Interface::Text[Interface::MSG_DEAD] : Interface::Text[Interface::MSG_DIED];

                border = Color::Highlight;

                break;

            case Battle::Result::ENTHRALLED:

                message = Graphics::RichText("ENEMIES HAVE BEEN SUBDUED!", Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0);

                break;

            case Battle::Result::FLEE:

                message = Graphics::RichText("YOUR PARTY FLEES!", Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0);

                border = Color::Inactive;

                break;

            default:

                message = Graphics::RichText("THE OUTCOME WAS INCONCLUSIVE!", Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0);

                border = Color::Inactive;

                break;
            }

            Interface::MessageBox(graphics, scene, message, Color::Background, border, 4, Color::Highlight, true);
        }
        else
        {
            Interface::MessageBox(graphics, scene, Graphics::RichText("BATTLE NOT LOADED!", Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0), Color::Transparent, Color::Highlight, 4, Color::Highlight, true);
        }
    }

    // main loop
    int Main(const char *battle_file)
    {
        auto return_code = 0;

        auto system = BloodSword::System();

        try
        {
            system.Initialize("BloodSword: Battle Board");

            BattleBoard::Menu(system.graphics, battle_file);
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
    if (argc < 2)
    {
        std::cerr << "To run:" << std::endl
                  << std::endl
                  << argv[0] << " [battle.json]"
                  << std::endl;

        return 1;
    }

    return BloodSword::BattleBoard::Main(argv[1]);
}
