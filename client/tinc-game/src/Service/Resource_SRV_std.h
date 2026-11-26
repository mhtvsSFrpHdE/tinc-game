#pragma once

namespace std
{
    /// <summary>
    /// Read "resource/Resource_SRV_std.h" to know more
    /// </summary>
    template<typename T>
    struct hash
    {
        size_t operator()(T key) const
        {
            return static_cast<size_t>(static_cast<long long>(key));
        }
    };
}
