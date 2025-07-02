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

    class ByReferenceTest {
    public:
        ByReferenceTest();
        ByReferenceTest(const ByReferenceTest&) = delete; // Delete copy constructor
        ByReferenceTest& operator=(const ByReferenceTest&) = delete; // Delete copy assignment operator
    };
    /// <summary>
    /// Test your lambda is capture by reference or by value
    /// Create ByReferenceTest as object, later pass it to ByRefTest function inside lambda
    /// If any copy happens, you'll receive a compiler error
    /// 
    /// See function definition for more
    /// </summary>
    void ByRefTest(ByReferenceTest& byRef);
};
