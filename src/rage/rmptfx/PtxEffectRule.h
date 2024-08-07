#pragma once
#include "../../Utils.h"
#include "../ReferenceCounter.h"
#include "PtxEvolution.h"
#include "PtxKeyFrame.h"
#include "PtxTimeline.h"

namespace rage
{
    class ptxEffectRule : public atReferenceCounter
    {
    public:
        rmPtfxKeyframe mKFColorTint;
        rmPtfxKeyframe mKFColorTintMax;
        rmPtfxKeyframe mKFZoom;
        rmPtfxKeyframe mKFRotation;
        rmPtfxKeyframe mKFDataSphere;
        rmPtfxKeyframe mKFDataCapsuleA;
        char* mName;
        int32_t field_FC;
        ptxEvolutionGroup* mPtxEvoGroup;
        float field_104;
        void* field_108;
        int8_t mZoomCullDistance;
        bool mUseRandomColor;
        bool mUseDefaultFunctors;
        int8_t field_10F[2];
        bool mHasDataSphere;
        int8_t mDataObjectType;
        uint8_t mGameFlags;
        int32_t field_114;
        int8_t field_118[7];
        int8_t field_11F;
    };
    ASSERT_SIZE(ptxEffectRule, 0x120);


    class ptxEffectRuleStd : public ptxEffectRule
    {
    public:
        ptxTimeLine mTimeline;
        float mFadeDistance;
        float mCullRadius;
        float mCullDistance;
        float mLodNearDistance;
        float mLodFarDistance;
        float mFileVersion;
        float mDurationMin;
        float mDurationMax;
        float mTimeScalarMin;
        float mTimeScalarMax;
        int32_t field_16C;
        int32_t field_170;
        bool mUseCullSphere;
        bool mCullNoUpdate;
        bool mCullNoEmit;
        bool mCullNoDraw;
        bool mSortEvents;
        int8_t mQuality;
        int8_t field_17A[4];
        float mCullSphere;
        float field_184;
        float field_188;
        int8_t field_18C[4];
        Vector3 mRandomOffsetPos;
        int8_t field_19C[3];
        int8_t field_19F;
    };
    ASSERT_SIZE(ptxEffectRuleStd, 0x1A0);
}