#ifndef __CHOICE_HPP__
#define __CHOICE_HPP__

#include <string>
#include <vector>

#include "Book.hpp"

// classes and functions for managing story choices
namespace BloodSword::Choice
{
    enum class Type
    {
        NORMAL
    };

    class Base
    {
    public:
        Choice::Type Choice = Choice::Type::NORMAL;

        Book::Destination Destination = {Book::Number::None, 0};

        std::string Text;
    };
}

#endif