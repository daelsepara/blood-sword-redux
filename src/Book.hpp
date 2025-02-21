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
        NONE = 0,
        BOOK1,
        BOOK2,
        BOOK3,
        BOOK4,
        BOOK5
    };

    // book location specifier: book and section number pair
    typedef std::pair<Book::Number, int> Location;

    BloodSword::Mapping<Book::Number> Title = {
        {Book::Number::NONE, "Blood Sword"},
        {Book::Number::BOOK1, "The Battlepits of Krarth"},
        {Book::Number::BOOK2, "The Kingdom of Wyrd"},
        {Book::Number::BOOK3, "The Demon's Claw"},
        {Book::Number::BOOK4, "Doomwalk"},
        {Book::Number::BOOK5, "The Walls of Spyte"}};

    BloodSword::Mapping<Book::Number> Mapping = {
        {Book::Number::NONE, "NONE"},
        {Book::Number::BOOK1, "BOOK1"},
        {Book::Number::BOOK2, "BOOK2"},
        {Book::Number::BOOK3, "BOOK3"},
        {Book::Number::BOOK4, "BOOK4"},
        {Book::Number::BOOK5, "BOOK5"}};

    BloodSword::UnorderedMap<Book::Number, std::vector<int>> Ranks = {
        {Book::Number::NONE, {1, 1, 1, 1}},
        {Book::Number::BOOK1, {8, 4, 3, 2}},
        {Book::Number::BOOK2, {12, 6, 4, 3}},
        {Book::Number::BOOK3, {16, 8, 5, 4}},
        {Book::Number::BOOK4, {20, 10, 7, 5}},
        {Book::Number::BOOK5, {20, 12, 8, 6}}};

    bool IsUndefined(Location location)
    {
        return (location.first == Book::Number::NONE && location.second == -1);
    }

    bool IsDefined(Location location)
    {
        return !Book::IsUndefined(location);
    }

    Book::Number MapTitle(const char *book)
    {
        return BloodSword::Find(Book::Title, book);
    }

    Book::Number MapTitle(std::string book)
    {
        return Book::MapTitle(book.c_str());
    }

    Book::Number MapBook(const char *book)
    {
        return BloodSword::Find(Book::Mapping, book);
    }

    Book::Number MapBook(std::string book)
    {
        return Book::MapBook(book.c_str());
    }

    // compare if the book locations are the same
    bool Equal(Book::Location &current, Book::Location &next)
    {
        std::cerr << "Equal ["
                  << Book::Mapping[current.first] << ", " << std::to_string(current.second) << "]"
                  << " == "
                  << Book::Mapping[next.first] << ", " << std::to_string(next.second) << "]"
                  << std::endl;

        return ((current.first == next.first) && (current.second == next.second));
    }
}
#endif
