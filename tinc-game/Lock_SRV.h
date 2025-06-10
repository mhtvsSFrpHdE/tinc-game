#pragma once
#include <mutex>
#include <condition_variable>

/// <summary>
/// Cross thread unlock mutex, which is normally undefined behavior
/// by using condition_variable
/// </summary>
namespace Lock_SRV
{
    void Notify(std::mutex& mutex, std::condition_variable& cb);
    void Wait(std::mutex& mutex, std::condition_variable& cb);
};
