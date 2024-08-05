#pragma once
#include "../Utils.h"
#include "Vector.h"
#include "Array.h"
#include "Model.h"
#include "Skeleton.h"
//#include "Shader.h"

namespace rage
{
    class rmcLod
    {
    public:
        atArray<grmModel> mModels;
    };
    ASSERT_SIZE(rmcLod, 0x8);

    class rmcLodGroup
    {
    public:
        Vector4 mCenter;
        Vector4 mBoundingBox[2];
        atArray<rmcLod>* mLods[4];
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
        struct grmShaderGroup* mShaderGroup;
    };
    ASSERT_SIZE(rmcDrawableBase, 0xC);

    class rmcDrawable : public rmcDrawableBase
    {
    public:
        crSkeletonData* mSkeleton;
        rmcLodGroup mLodGroup;
    };
    ASSERT_SIZE(rmcDrawable, 0x80);
}