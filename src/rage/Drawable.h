#pragma once
#include "../Utils.h"
#include "Vector.h"
#include "Array.h"
#include "Model.h"
#include "Skeleton.h"
#include "ShaderGroup.h"
#include "2DEffects.h"

namespace rage
{
    class rmcLod
    {
    public:
        rmcLod(const datResource& rsc) : mModels(rsc) {}

        inline void Place(void* that, const datResource& rsc)
        {
            new(that) rmcLod(rsc);
        }

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            mModels.AddToLayout(layout, depth);
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            mModels.SerializePtrs(layout, rsc, depth);
        }

        atArray<datOwner<grmModel>> mModels;
    };
    ASSERT_SIZE(rmcLod, 0x8);


    class rmcLodGroup
    {
    public:
        rmcLodGroup(const datResource& rsc) : mLods{rsc, rsc, rsc, rsc} {}

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            for(uint8_t i = 0; i < 4; i++)
            {
                mLods[i].AddToLayout(layout, depth);
            }
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            for(uint8_t i = 0; i < 4; i++)
            {
                mLods[i].SerializePtrs(layout, rsc, depth);
            }
        }

        Vector4 mSphere;
        Vector4 mBoundingBox[2];
        datOwner<rmcLod> mLods[4];
        float mLodDistances[4];
        int32_t mBucketMask[4];
        float mRadius;
        int32_t field_64;
        int32_t field_68;
        int32_t field_6C;
    };
    ASSERT_SIZE(rmcLodGroup, 0x70);


    class rmcDrawableBase : public pgBase
    {
    public:
        rmcDrawableBase(const datResource& rsc) : mShaderGroup(rsc) {}

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            pgBase::AddToLayout(layout, depth);
            mShaderGroup.AddToLayout(layout, depth);
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            pgBase::SerializePtrs(layout, rsc, depth);
            mShaderGroup.SerializePtrs(layout, rsc, depth);
        }

        datOwner<grmShaderGroup> mShaderGroup;
    };
    ASSERT_SIZE(rmcDrawableBase, 0xC);


    class rmcDrawable : public rmcDrawableBase
    {
    public:
        rmcDrawable(const datResource& rsc) : rmcDrawableBase(rsc), mSkeleton(rsc), mLodGroup(rsc) {}

        inline void Place(void* that, const datResource& rsc)
        {
            new(that) rmcDrawable(rsc);
        }

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            rmcDrawableBase::AddToLayout(layout, depth);
            mSkeleton.AddToLayout(layout, depth);
            mLodGroup.AddToLayout(layout, depth);
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            rmcDrawableBase::SerializePtrs(layout, rsc, depth);
            mSkeleton.SerializePtrs(layout, rsc, depth);
            mLodGroup.SerializePtrs(layout, rsc, depth);
        }

        datOwner<crSkeletonData> mSkeleton;
        rmcLodGroup mLodGroup;
    };
    ASSERT_SIZE(rmcDrawable, 0x80);
}

class gtaDrawable : public rage::rmcDrawable
{
public:
    gtaDrawable(const rage::datResource& rsc) : rage::rmcDrawable(rsc), mLights(rsc) {}

    inline void Place(void* that, const rage::datResource& rsc)
    {
        new(that) gtaDrawable(rsc);
    }

    void AddToLayout(RSC5Layout& layout, uint32_t depth)
    {
        rage::rmcDrawable::AddToLayout(layout, depth);
        mLights.AddToLayout(layout, depth);
    }

    void SerializePtrs(RSC5Layout& layout, rage::datResource& rsc, uint32_t depth)
    {
        rage::rmcDrawable::SerializePtrs(layout, rsc, depth);
        mLights.SerializePtrs(layout, rsc, depth);
    }

    rage::atArray<CLightAttr> mLights;
};
ASSERT_SIZE(gtaDrawable, 0x88);