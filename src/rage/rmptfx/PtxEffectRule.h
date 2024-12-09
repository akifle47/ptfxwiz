#pragma once
#include "rapidjson/include/prettywriter.h"
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
        ptxEffectRule() : mName(nullptr), field_FC(1), mPtxEvoGroup(nullptr), field_104(0.0f), field_108(nullptr), mZoomCullDistance(1), mUseRandomColor(false),
                          mUseDefaultFunctors(false), field_10F{}, mHasDataSphere(false), mDataObjectType(0), mGameFlags(0), field_114(0), field_118{}, field_11F() {}

        ptxEffectRule(const datResource& rsc) : mKFColorTint(rsc), mKFColorTintMax(rsc), mKFZoom(rsc), mKFRotation(rsc), mKFDataSphere(rsc),
                                                mKFDataCapsuleA(rsc), mPtxEvoGroup(rsc)
        {
            if(mName)
            {
                rsc.PointerFixUp(mName);
            }
        }

        ~ptxEffectRule()
        {
            if(mName)
            {
                delete[] mName;
                mName = nullptr;
            }

            if(mPtxEvoGroup.Get())
            {
                delete mPtxEvoGroup.Get();
                mPtxEvoGroup = {nullptr};
            }
        }

        void AddToLayout(RSC5Layout& layout, uint32_t depth);
        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth);

        uint32_t GetObjectSize() const
        {
            return 0x1A0;
        }

        virtual void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) = 0;
        virtual void LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object) = 0;

        void Place(void* that, const datResource& rsc);

        rmPtfxKeyframe mKFColorTint;
        rmPtfxKeyframe mKFColorTintMax;
        rmPtfxKeyframe mKFZoom;
        rmPtfxKeyframe mKFRotation;
        rmPtfxKeyframe mKFDataSphere;
        rmPtfxKeyframe mKFDataCapsuleA;
        char* mName;
        int32_t field_FC;
        datOwner<ptxEvolutionGroup> mPtxEvoGroup;
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

    protected:
        void WriteToJsonBase(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
        void LoadFromJsonBase(rapidjson::GenericObject<true, rapidjson::Value>& object);
    };
    ASSERT_SIZE(ptxEffectRule, 0x120);


    class ptxEffectRuleStd : public ptxEffectRule
    {
    public:
        ptxEffectRuleStd() : ptxEffectRule(), mFadeDistance(-1.0f), mCullRadius(0.0f), mCullDistance(-1.0f), mLodNearDistance(-1.0f), mLodFarDistance(-1.0f),
                             mFileVersion(4.0f), mDurationMin(1.0f), mDurationMax(1.0f), mTimeScalarMin(1.0f), mTimeScalarMax(1.0f), field_16C(0), field_170(0),
                             mUseCullSphere(false), mCullNoUpdate(false), mCullNoEmit(false), mCullNoDraw(true), mSortEvents(false), mQuality(0), field_17A{},
                             mCullSphere(0.0f), field_184(0.0f), field_188(0.0f), field_18C{}, mRandomOffsetPos{.x = 0.0f, .y = 0.0f, .z = 0.0f}, field_19C{}, field_19F(0)
        {
            mTimeline.mEffectRule = {this};
        }

        ptxEffectRuleStd(const datResource& rsc) : ptxEffectRule(rsc), mTimeline(rsc)
        {
            field_170 = 0;
            mQuality = mQuality == 0 ? mQuality : 0;
        }

        void AddToLayout(RSC5Layout& layout, uint32_t depth);
        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth);

        void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override;
        void LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object) override;

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