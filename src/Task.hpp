#ifndef __TASK_HPP__
#define __TASK_HPP__

#include "Templates.hpp"

// functions for managing in-game "player" tasks
namespace BloodSword::Task
{
    // task status
    enum class Status
    {
        NONE = -1,
        START,
        PROGRESS,
        INCOMPLETE,
        COMPLETE
    };

    // task status to string mapping
    BloodSword::StringMap<Task::Status> Mapping = {
        {Task::Status::NONE, "NONE"},
        {Task::Status::START, "START"},
        {Task::Status::PROGRESS, "PROGRESS"},
        {Task::Status::INCOMPLETE, "INCOMPLETE"},
        {Task::Status::COMPLETE, "COMPLETE"}};

    // map string to task status
    Task::Status Map(const char *task)
    {
        return BloodSword::Find(Task::Mapping, task);
    }

    // map string to task status
    Task::Status Map(std::string task)
    {
        return Task::Map(task.c_str());
    }
}

#endif
