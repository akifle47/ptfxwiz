#pragma once
#include "../Base.h"
#include "../DatRef.h"
#include "PtxKeyFrame.h"
#include "../grmodel/Drawable.h"

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

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            field_40.AddToLayout(layout, depth);
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            field_40.SerializePtrs(layout, rsc, depth);
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

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            mKeyFrame.AddToLayout(layout, depth);
            field_2C.AddToLayout(layout, depth);
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            mKeyFrame.SerializePtrs(layout, rsc, depth);
            field_2C.SerializePtrs(layout, rsc, depth);
        }

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

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            mBiasLinks.AddToLayout(layout, depth);

            mColorKF.AddToLayout(layout, depth);
            mColorMaxKF.AddToLayout(layout, depth);
            mAccelerationMinKF.AddToLayout(layout, depth);
            mAccelerationMaxKF.AddToLayout(layout, depth);
            mDampeningMinKF.AddToLayout(layout, depth);
            mDampeningMaxKF.AddToLayout(layout, depth);
            mMatrixWeightKF.AddToLayout(layout, depth);
            mPlaybackRateKF.AddToLayout(layout, depth);
            mAlphaKF.AddToLayout(layout, depth);
            mPositionNoiseKF.AddToLayout(layout, depth);
            mVelocityNoiseKF.AddToLayout(layout, depth);
            mCollisionVelocityDampeningKF.AddToLayout(layout, depth);
            mWindInfluenceKF.AddToLayout(layout, depth);
            mVortexPropsKF.AddToLayout(layout, depth);
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            mBiasLinks.SerializePtrs(layout, rsc, depth);

            mColorKF.SerializePtrs(layout, rsc, depth);
            mColorMaxKF.SerializePtrs(layout, rsc, depth);
            mAccelerationMinKF.SerializePtrs(layout, rsc, depth);
            mAccelerationMaxKF.SerializePtrs(layout, rsc, depth);
            mDampeningMinKF.SerializePtrs(layout, rsc, depth);
            mDampeningMaxKF.SerializePtrs(layout, rsc, depth);
            mMatrixWeightKF.SerializePtrs(layout, rsc, depth);
            mPlaybackRateKF.SerializePtrs(layout, rsc, depth);
            mAlphaKF.SerializePtrs(layout, rsc, depth);
            mPositionNoiseKF.SerializePtrs(layout, rsc, depth);
            mVelocityNoiseKF.SerializePtrs(layout, rsc, depth);
            mCollisionVelocityDampeningKF.SerializePtrs(layout, rsc, depth);
            mWindInfluenceKF.SerializePtrs(layout, rsc, depth);
            mVortexPropsKF.SerializePtrs(layout, rsc, depth);
        }

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

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            ptxRulePropList::AddToLayout(layout, depth);

            mSizeKF.AddToLayout(layout, depth);
            mThetaKF.AddToLayout(layout, depth);
            mInitThetaKFOT.AddToLayout(layout, depth);
            mInitRotateVelKFOT.AddToLayout(layout, depth);
            mRotateVelKF.AddToLayout(layout, depth);
            mDirectionalKF.AddToLayout(layout, depth);
            mDirectionalVelKF.AddToLayout(layout, depth);
            mTextureAnimRateKF.AddToLayout(layout, depth);
            mTrailPropsKF.AddToLayout(layout, depth);
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            ptxRulePropList::SerializePtrs(layout, rsc, depth);

            mSizeKF.SerializePtrs(layout, rsc, depth);
            mThetaKF.SerializePtrs(layout, rsc, depth);
            mInitThetaKFOT.SerializePtrs(layout, rsc, depth);
            mInitRotateVelKFOT.SerializePtrs(layout, rsc, depth);
            mRotateVelKF.SerializePtrs(layout, rsc, depth);
            mDirectionalKF.SerializePtrs(layout, rsc, depth);
            mDirectionalVelKF.SerializePtrs(layout, rsc, depth);
            mTextureAnimRateKF.SerializePtrs(layout, rsc, depth);
            mTrailPropsKF.SerializePtrs(layout, rsc, depth);
        }

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

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            ptxRulePropList::AddToLayout(layout, depth);

            mSizeMin.AddToLayout(layout, depth);
            mSizeMax.AddToLayout(layout, depth);
            mInitialThetaMin.AddToLayout(layout, depth);
            mInitialThetaMax.AddToLayout(layout, depth);
            mThetaMin.AddToLayout(layout, depth);
            mThetaMax.AddToLayout(layout, depth);
            mInitialRotationMin.AddToLayout(layout, depth);
            mInitialRotationMax.AddToLayout(layout, depth);
            mInitRotationSpeed.AddToLayout(layout, depth);
            mRotationSpeed.AddToLayout(layout, depth);
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            ptxRulePropList::SerializePtrs(layout, rsc, depth);

            mSizeMin.SerializePtrs(layout, rsc, depth);
            mSizeMax.SerializePtrs(layout, rsc, depth);
            mInitialThetaMin.SerializePtrs(layout, rsc, depth);
            mInitialThetaMax.SerializePtrs(layout, rsc, depth);
            mThetaMin.SerializePtrs(layout, rsc, depth);
            mThetaMax.SerializePtrs(layout, rsc, depth);
            mInitialRotationMin.SerializePtrs(layout, rsc, depth);
            mInitialRotationMax.SerializePtrs(layout, rsc, depth);
            mInitRotationSpeed.SerializePtrs(layout, rsc, depth);
            mRotationSpeed.SerializePtrs(layout, rsc, depth);
        }

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

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            layout.AddObject(mName, RSC5Layout::eBlockType::VIRTUAL, strlen(mName) + 1);
            mDrawable.AddToLayout(layout, depth);
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            layout.SerializePtr(mName, strlen(mName) + 1);
            mDrawable.SerializePtrs(layout, rsc, depth);
        }

        inline void Place(void* that, const datResource& rsc)
        {
            new(that) PtxNameDrawablePair(rsc);
        }

        char* mName;
        datRef<rmcDrawable> mDrawable;
    };
}