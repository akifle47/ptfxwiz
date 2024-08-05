#pragma once
#include "Base.h"
#include "Geometry.h"
#include "../Utils.h"

namespace rage
{
    class grmModel : datBase
    {
    public:
        atArray<grmGeometryQB> mGeometries;
        Vector4* mBounds;
        int8_t* mShaderMappings;
        uint8_t mBoneCount;
        uint8_t mFlags;
        uint8_t mType;
        uint8_t mBoneIndex;
        uint8_t mStride;
        bool mSkinned;
        int16_t mShaderMappingCount;
    };
    ASSERT_SIZE(grmModel, 0x1C);

}