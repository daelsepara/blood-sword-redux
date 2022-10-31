#ifndef __STORY_HPP__
#define __STORY_HPP__

#include <string>

#include "Book.hpp"
#include "Battle.hpp"

// classes and functions for managing story sections
namespace BloodSword::Story
{
    enum class Option
    {
        NORMAL,
        BATTLE,
        ENDING
    };

    class Base
    {
    public:
        Book::Destination Section = std::make_pair<Book::Number, int>(Book::Number::None, -1);

        std::vector<Story::Option> Settings = {};

        std::string Text;
    };
}

#endif