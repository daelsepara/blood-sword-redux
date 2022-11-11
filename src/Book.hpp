#ifndef __BOOK_HPP__
#define __BOOK_HPP__

#include <unordered_map>
#include <vector>

#include "Templates.hpp"

namespace BloodSword::Book
{
    // book number
    enum class Number
    {
        None = 0,
        Book1,
        Book2,
        Book3,
        Book4,
        Book5
    };

    // book location specifier: book and section number pair
    typedef std::pair<Book::Number, int> Destination;

    Mapping<Book::Number> Title = {
        {Book::Number::None, "Blood Sword"},
        {Book::Number::Book1, "The Battlepits of Krarth"},
        {Book::Number::Book2, "The Kingdom of Wyrd"},
        {Book::Number::Book3, "The Demon's Claw"},
        {Book::Number::Book4, "Doomwalk"},
        {Book::Number::Book5, "The Walls of Spyte"}};

    Mapping<Book::Number> BookNumber = {
        {Book::Number::None, "NONE"},
        {Book::Number::Book1, "BOOK1"},
        {Book::Number::Book2, "BOOK2"},
        {Book::Number::Book3, "BOOK3"},
        {Book::Number::Book4, "BOOK4"},
        {Book::Number::Book5, "BOOK5"}};

    std::unordered_map<Book::Number, std::vector<int>> Ranks = {
        {Book::Number::None, {1, 1, 1, 1}},
        {Book::Number::Book1, {8, 4, 3, 2}},
        {Book::Number::Book2, {12, 6, 4, 3}},
        {Book::Number::Book3, {16, 8, 5, 4}},
        {Book::Number::Book4, {20, 10, 7, 5}},
        {Book::Number::Book5, {20, 12, 8, 6}}};

    bool IsUndefined(Destination destination)
    {
        return (destination.first == Book::Number::None && destination.second == 0);
    }

    bool IsDefined(Destination destination)
    {
        return !Book::IsUndefined(destination);
    }
}
#endif
