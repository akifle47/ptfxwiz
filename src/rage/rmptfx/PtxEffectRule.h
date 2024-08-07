#pragma once
#include "../../Utils.h"
#include "../ReferenceCounter.h"
#include "PtxKeyFrame.h"
#include "PtxTimeline.h"

namespace rage
{
    class ptxEvolutionGroup
    {
    public:
        atArray<struct ptxEvolution> mEvoList;
        atArray<struct ptxEvoBlendMode> mEvoBlendModeList;
        int8_t field_10[0x14];
        struct ptxEvoHashIndexList* mEvoListIndices;
        int8_t field_28[0x8];
    };
    ASSERT_SIZE(ptxEvolutionGroup, 0x30);


    struct ptxEvolution
    {
        char* mEvoName;
        atArray<struct ptxEvoPropList> mPropList;
        int8_t field_C[0x8];
    };
    ASSERT_SIZE(ptxEvolution, 0x14);

    struct ptxEvoBlendMode
    {
        int32_t field_0;
        int32_t field_4;
    };
    ASSERT_SIZE(ptxEvoBlendMode, 0x8);

    struct ptxEvoPropList
    {
        int32_t field_0;
        int32_t field_4;
        int32_t field_8;
        int32_t field_C;
        int32_t field_10;
        int32_t field_14;
        int32_t field_18;
        int32_t field_1C;
        int32_t field_20;
        int32_t field_24;
        atArray<struct ptxEvoProp> mPropList;
    };
    ASSERT_SIZE(ptxEvoPropList, 0x30);

    struct ptxEvoProp
    {
        rmPtfxKeyframe mKeyFrameData;
        int32_t mRegID;
    };
    ASSERT_SIZE(ptxEvoProp, 0x2C);

    struct ptxEvoHashIndexList
    {
        int8_t field_0;
        //first is a hash of ptxEvolutionGroup::mEvoList::mEvoName
        //second is an index into ptxEvolutionGroup::mEvoList
        atArray<std::pair<uint32_t, uint32_t>> mList;
    };
    ASSERT_SIZE(ptxEvoHashIndexList, 0xC);
        

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