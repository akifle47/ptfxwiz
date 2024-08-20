#pragma once
#include <cstdio>

namespace Log
{
    static bool msBreakOnError = true;

    template<typename ...Args>
    inline void Info(const char* fmt, Args ...args)
    {
        printf(fmt, args...);
        printf("\n");
    }

    template<typename ...Args>
    inline void Warn(const char* fmt, Args ...args)
    {
        printf("Warning: ");
        printf(fmt, args...);
        printf("\n");
    }

    template<typename ...Args>
    inline void Error(const char* fmt, Args ...args)
    {
        printf("ERROR: ");
        printf(fmt, args...);
        printf("\n");

#ifdef _DEBUG
        if(msBreakOnError)
            __debugbreak();
#endif
    }
}