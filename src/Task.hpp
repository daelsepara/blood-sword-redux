#ifndef __TASK_HPP__
#define __TASK_HPP__

#include "Templates.hpp"

namespace BloodSword::Task
{
    enum class Status
    {
        NONE = -1,
        START,
        PROGRESS,
        INCOMPLETE,
        COMPLETE
    };

    BloodSword::Mapping<Task::Status> Mapping = {
        {Task::Status::NONE, "NONE"},
        {Task::Status::START, "START"},
        {Task::Status::PROGRESS, "PROGRESS"},
        {Task::Status::INCOMPLETE, "INCOMPLETE"},
        {Task::Status::COMPLETE, "COMPLETE"}};

    Task::Status Map(const char *task)
    {
        return BloodSword::Find(Task::Mapping, task);
    }

    Task::Status Map(std::string task)
    {
        return Task::Map(task.c_str());
    }
}

#endif
