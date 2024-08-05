#pragma once
#include "Vector.h"

namespace rage
{
    class Matrix44
    {
    public:
        Vector4 a;
        Vector4 b;
        Vector4 c;
        Vector4 d;
    };

    class Matrix34
    {
    public:
        Vector4 a;
        Vector4 b;
        Vector4 c;
        Vector4 d;
    };
}