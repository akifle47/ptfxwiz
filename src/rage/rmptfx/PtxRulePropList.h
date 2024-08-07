#pragma once
#include "../Base.h"
#include "../Resource.h"
#include "PtxKeyFrame.h"

namespace rage
{
    class ptxRulePropList_obj1
    {
    public:
        ptxRulePropList_obj1(const datResource& rsc) : field_40(rsc) {}

        inline void Place(void* that, const datResource& rsc)
        {
            new(that) ptxRulePropList_obj1(rsc);
        }

        char mName[64];
        //todo: unknown type
        datOwner<void*> field_40;
        int8_t field_44[7];
        int8_t field_4B;
    };
    ASSERT_SIZE(ptxRulePropList_obj1, 0x4C);


    class ptxRulePropList_obj2
    {
    public:
        ptxRulePropList_obj2() {}

        ptxRulePropList_obj2(const datResource& rsc) : mKeyFrame(rsc), field_2C(rsc) {}

        rmPtfxKeyframe mKeyFrame;
        int8_t field_28[4];
        datOwner<void*> field_2C;
        int8_t field_30[3];
        int8_t field_33;
    };
    ASSERT_SIZE(ptxRulePropList_obj2, 0x34);


    class ptxRulePropList : public datBase
    {
    public:
        ptxRulePropList(const datResource& rsc) : mBiasLinks(rsc), mColorKF(rsc), mColorMaxKF(rsc), mAccelerationMinKF(rsc), mAccelerationMaxKF(rsc),
                                                  mDampeningMinKF(rsc), mDampeningMaxKF(rsc), mMatrixWeightKF(rsc), mPlaybackRateKF(rsc), 
                                                  mAlphaKF(rsc), mPositionNoiseKF(rsc), mVelocityNoiseKF(rsc), mCollisionVelocityDampeningKF(rsc), 
                                                  mWindInfluenceKF(rsc), mVortexPropsKF(rsc) {}

        atArray<ptxRulePropList_obj1> mBiasLinks;
        int8_t field_C[4];
        ptxRulePropList_obj2 mColorKF;
        ptxRulePropList_obj2 mColorMaxKF;
        ptxRulePropList_obj2 mAccelerationMinKF;
        ptxRulePropList_obj2 mAccelerationMaxKF;
        ptxRulePropList_obj2 mDampeningMinKF;
        ptxRulePropList_obj2 mDampeningMaxKF;
        ptxRulePropList_obj2 mMatrixWeightKF;
        ptxRulePropList_obj2 mPlaybackRateKF;
        ptxRulePropList_obj2 mAlphaKF;
        ptxRulePropList_obj2 mPositionNoiseKF;
        ptxRulePropList_obj2 mVelocityNoiseKF;
        ptxRulePropList_obj2 mCollisionVelocityDampeningKF;
        ptxRulePropList_obj2 mCollisionImpVarKF;
        ptxRulePropList_obj2 mWindInfluenceKF;
        ptxRulePropList_obj2 mVortexPropsKF;
    };
    ASSERT_SIZE(ptxRulePropList, 0x31C);


    class ptxSpriteRulePropList : public ptxRulePropList
    {
    public:
        ptxSpriteRulePropList(const datResource& rsc) : ptxRulePropList(rsc), mSizeKF(rsc), mThetaKF(rsc), mInitThetaKFOT(rsc),
                                                        mInitRotateVelKFOT(rsc), mRotateVelKF(rsc), mDirectionalKF(rsc), mDirectionalVelKF(rsc), 
                                                        mTextureAnimRateKF(rsc), mTrailPropsKF(rsc) {}

        ptxRulePropList_obj2 mSizeKF;
        ptxRulePropList_obj2 mThetaKF;
        ptxRulePropList_obj2 mInitThetaKFOT;
        ptxRulePropList_obj2 mInitRotateVelKFOT;
        ptxRulePropList_obj2 mRotateVelKF;
        ptxRulePropList_obj2 mDirectionalKF;
        ptxRulePropList_obj2 mDirectionalVelKF;
        ptxRulePropList_obj2 mTextureAnimRateKF;
        ptxRulePropList_obj2 mTrailPropsKF;
    };
    ASSERT_SIZE(ptxSpriteRulePropList, 0x4F0);


    class ptxModelRulePropList : public ptxRulePropList
    {
    public:
        ptxModelRulePropList(const datResource& rsc) : ptxRulePropList(rsc), mSizeMin(rsc), mSizeMax(rsc), mInitialThetaMin(rsc), 
                                                       mInitialThetaMax(rsc), mThetaMin(rsc), mThetaMax(rsc), mInitialRotationMin(rsc), 
                                                       mInitialRotationMax(rsc), mInitRotationSpeed(rsc), mRotationSpeed(rsc) {}

        ptxRulePropList_obj2 mSizeMin;
        ptxRulePropList_obj2 mSizeMax;
        ptxRulePropList_obj2 mInitialThetaMin;
        ptxRulePropList_obj2 mInitialThetaMax;
        ptxRulePropList_obj2 mThetaMin;
        ptxRulePropList_obj2 mThetaMax;
        ptxRulePropList_obj2 mInitialRotationMin;
        ptxRulePropList_obj2 mInitialRotationMax;
        ptxRulePropList_obj2 mInitRotationSpeed;
        ptxRulePropList_obj2 mRotationSpeed;
    };
    ASSERT_SIZE(ptxModelRulePropList, 0x524);

    class PtxNameDrawablePair
    {
    public:
        PtxNameDrawablePair(const datResource& rsc) : mDrawable(rsc) 
        {
            rsc.PointerFixUp(mName);
        }

        inline void Place(void* that, const datResource& rsc)
        {
            new(that) PtxNameDrawablePair(rsc);
        }

        char* mName;
        datOwner<class rmcDrawable> mDrawable;
    };
}