#ifndef __TEST_HPP__
#define __TEST_HPP__

#include <SDL.h>

#include "Asset.hpp"
#include "Engine.hpp"
#include "Fonts.hpp"
#include "Graphics.hpp"
#include "Input.hpp"

// framework for testing game subsystems
namespace BloodSword::Test
{
    void Controls(Graphics::Base &graphics)
    {
        if (graphics.Renderer)
        {
            auto x = 128;
            auto y = 64;
            auto w = 64;
            auto h = 64;
            auto s = 16;
            auto ws = w + s;

            // sprites
            auto warrior = Graphics::SceneElements(Asset::Get(Asset::Type::WARRIOR), x, y);
            auto trickster = Graphics::SceneElements(Asset::Get(Asset::Type::TRICKSTER), x + ws, y);
            auto sage = Graphics::SceneElements(Asset::Get(Asset::Type::SAGE), x + 2 * ws, y);
            auto enchanter = Graphics::SceneElements(Asset::Get(Asset::Type::ENCHANTER), x + 3 * ws, y);

            // text sprites
            auto text = Graphics::CreateText("Every thirteen lunar months the Magi of Krarth hold a desperate contest to see which of them will rule that bleak and icy land. Teams of daring adventurers are sent down into the labyrinths that lie beneath the tundra, each searching for the Emblem of Victory that will win power for their patron.\n\nOnly one team can prevail. The others must die.", Fonts::Normal, {96, 96, 96, 255}, TTF_STYLE_NORMAL, 8 * ws - s);
            auto text_sprite = Graphics::SceneElements(text, x + 4 * ws, y);

            auto warrior_text = Graphics::CreateText("You are a master of the fighting arts. You have better Fighting Prowess than any other character type, and when you strike a blow, you inflict more damage. You also have chainmail armour which provides an Armour rating of 3, which is better than the armour available to other characters.\n\nThese advantages give you a real edge in any fight, but you do not get things all your own way. You have none of the characters' special skills -- the Sage's ESP, for instance, or the Trickster's low devious cunning. Also, because you follow the honourable traditions of your class, you must be careful to stay true to the code of chivalry.", Fonts::Normal, {255, 255, 255, 255}, TTF_STYLE_NORMAL, 8 * ws - s);
            auto trickster_text = Graphics::CreateText("Some adventurers are honourable and prefer to face their foes in a straight fight. You live by your wits. If you can win by trickery or by shooting someone in the back, you will. You know how to wield a sword if you have to, but your main weapon is cunning.", Fonts::Normal, {255, 255, 255, 255}, TTF_STYLE_NORMAL, 8 * ws - s);
            auto sage_text = Graphics::CreateText("Your upbringing has been in the spartan Monastery of Illumination on the barren island of Kaxos. There, you have studied the Mystic Way, a series of demanding spiritual disciplines combined with rigorous physical training.", Fonts::Normal, {255, 255, 255, 255}, TTF_STYLE_NORMAL, 8 * ws - s);
            auto enchanter_text = Graphics::CreateText("Forget the mundane arts of swordplay. You know that true power lies in the manipulation of occult powers of sorcery.", Fonts::Normal, {255, 255, 255, 255}, TTF_STYLE_NORMAL, 8 * ws - s);

            auto warrior_sprite = Graphics::SceneElements(warrior_text, x + 4 * ws, y);
            auto trickster_sprite = Graphics::SceneElements(trickster_text, x + 4 * ws, y);
            auto sage_sprite = Graphics::SceneElements(sage_text, x + 4 * ws, y);
            auto enchanter_sprite = Graphics::SceneElements(enchanter_text, x + 4 * ws, y);

            // controls
            std::vector<Controls::Base> controls = {
                Controls::Base(Controls::Type::WARRIOR, 0, 0, 1, 0, 0, x, y, w, h, 0xFF606060),
                Controls::Base(Controls::Type::TRICKSTER, 1, 0, 2, 1, 1, x + w + s, y, w, h, 0xFF606060),
                Controls::Base(Controls::Type::SAGE, 2, 1, 3, 2, 2, x + 2 * ws, y, w, h, 0xFF606060),
                Controls::Base(Controls::Type::ENCHANTER, 3, 2, 3, 3, 3, x + 3 * ws, y, w, h, 0xFF606060)};

            auto sprites = std::vector<Graphics::SceneElements>();

            auto input = Controls::User();

            while (!input.Selected)
            {
                sprites.clear();
                sprites.push_back(warrior);
                sprites.push_back(trickster);
                sprites.push_back(sage);
                sprites.push_back(enchanter);

                if (input.Current >= 0 && input.Current < controls.size())
                {
                    switch (input.Current)
                    {
                    case 0:
                        sprites.push_back(warrior_sprite);
                        break;
                    case 1:
                        sprites.push_back(trickster_sprite);
                        break;
                    case 2:
                        sprites.push_back(sage_sprite);
                        break;
                    case 3:
                        sprites.push_back(enchanter_sprite);
                        break;
                    default:
                        sprites.push_back(text_sprite);
                    }
                }
                else
                {
                    sprites.push_back(text_sprite);
                }

                Graphics::Scene scene = Graphics::Scene(sprites, controls, 0);

                input = Input::WaitForInput(graphics, scene, input);
            }

            SDL_FreeSurface(enchanter_text);
            SDL_FreeSurface(sage_text);
            SDL_FreeSurface(trickster_text);
            SDL_FreeSurface(warrior_text);
            SDL_FreeSurface(text);

            enchanter_text = NULL;
            sage_text = NULL;
            trickster_text = NULL;
            warrior_text = NULL;
            text = NULL;
        }
    }
}

#endif
