#pragma once
#include <cstring>
#include <cstdint>

class AString
{
public:
    AString()
    {
        mString = nullptr;
    }

    AString(uint32_t size)
    {
        mString = new char[size];
        memset(mString, 0, size);
    }

    AString(const char* str)
    {
        mString = strdup(str);
    }

    ~AString()
    {
        if(mString)
        {
            delete[] mString;
            mString = nullptr;
        }
    }

    AString& operator=(const AString& that)
    {
        if(this == &that)
        {
            return *this;
        }

        if(mString)
        {
            delete[] mString;
            mString = nullptr;
        }

        if(that.mString)
        {
            mString = strdup(that.mString);
        }
    }

    AString& operator=(const char* str)
    {
        if(mString)
        {
            delete[] mString;
            mString = nullptr;
        }

        if(str)
        {
            mString = strdup(str);
        }

        return *this;
    }

    bool operator==(const AString& rhs)
    {
        return strcmp(mString, rhs.mString) == 0;
    }

    char& operator[](uint32_t index)
    {
        return mString[index];
    }

    char& At(uint32_t index)
    {
        return mString[index];
    }

    char* Get() const
    {
        return mString;
    }

private:
    char* mString;
};