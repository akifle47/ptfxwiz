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

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            mGeometries.AddToLayout(layout, depth);

            layout.AddObject(mBounds.Get(), RSC5Layout::eBlockType::VIRTUAL, mGeometries.mCount + (mGeometries.mCount == 0 ? 0 : 1));
            layout.AddObject(mShaderMappings.Get(), RSC5Layout::eBlockType::VIRTUAL, mShaderMappingCount);
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            mGeometries.SerializePtrs(layout, rsc, depth);
            mBounds.SerializePtrs(layout, rsc, depth);
            mShaderMappings.SerializePtrs(layout, rsc, depth);
        }

        atArray<datOwner<grmGeometryQB>> mGeometries;
        datOwner<Vector4> mBounds;
        datOwner<uint16_t> mShaderMappings;
        uint8_t mBoneCount;
        uint8_t mFlags;
        uint8_t mType;
        uint8_t mBoneIndex;
        uint8_t mStride;
        bool mSkinned;
        uint16_t mShaderMappingCount;
    };
    ASSERT_SIZE(grmModel, 0x1C);

}