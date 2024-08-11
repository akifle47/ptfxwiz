#pragma once
#include "../Utils.h"
#include "Vector.h"
#include "Array.h"
#include "Model.h"
#include "Skeleton.h"
#include "ShaderGroup.h"

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

        atArray<datOwner<grmModel>> mModels;
    };
    ASSERT_SIZE(rmcLod, 0x8);


    class rmcLodGroup
    {
    public:
        rmcLodGroup(const datResource& rsc) : mLods{rsc, rsc, rsc, rsc} {}

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

        datOwner<crSkeletonData> mSkeleton;
        rmcLodGroup mLodGroup;
    };
    ASSERT_SIZE(rmcDrawable, 0x80);
}