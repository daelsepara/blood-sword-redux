#ifndef __BOOK_HPP__
#define __BOOK_HPP__

#include <map>
#include <vector>

namespace BloodSword::Book
{
    enum class Type
    {
        None = 0,
        Book1,
        Book2,
        Book3,
        Book4,
        Book5
    };

    typedef std::pair<Book::Type, int> Destination;

    std::map<Book::Type, const char *> Title = {
        {Book::Type::None, "Blood Sword"},
        {Book::Type::Book1, "The Battlepits of Krarth"},
        {Book::Type::Book2, "The Kingdom of Wyrd"},
        {Book::Type::Book3, "The Demon's Claw"},
        {Book::Type::Book4, "Doomwalk"},
        {Book::Type::Book5, "The Walls of Spyte"}};

    std::map<Book::Type, std::vector<int>> Ranks = {
        {Book::Type::None, {1, 1, 1, 1}},
        {Book::Type::Book1, {8, 4, 3, 2}},
        {Book::Type::Book2, {12, 6, 4, 3}},
        {Book::Type::Book3, {16, 8, 5, 4}},
        {Book::Type::Book4, {20, 10, 7, 5}},
        {Book::Type::Book5, {20, 12, 8, 6}}};
}
#endif
