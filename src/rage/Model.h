#pragma once
#include "Base.h"
#include "Geometry.h"
#include "../Utils.h"

namespace rage
{
    class grmModel : public datBase
    {
    public:
        grmModel(const datResource& rsc) : mGeometries(rsc), mBounds(rsc), mShaderMappings(rsc) {}

        inline void Place(void* that, const datResource& rsc)
        {
            new(that) grmModel(rsc);
        }

        atArray<datOwner<grmGeometryQB>> mGeometries;
        datOwner<Vector4> mBounds;
        datOwner<int8_t> mShaderMappings;
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