#include "Lock_SRV.h"

void Lock_SRV::Notify(std::mutex& mutex, std::condition_variable& cb)
{
    std::lock_guard<std::mutex> lockGuard(mutex);
    cb.notify_all();
}

void Lock_SRV::Wait(std::mutex& mutex, std::condition_variable& cb)
{
    std::unique_lock<std::mutex> ulock(mutex);
    cb.wait(ulock);
}
