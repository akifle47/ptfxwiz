#pragma once
#include "rapidjson/include/prettywriter.h"
#include "../../Utils.h"
#include "../math/Vector.h"
#include "PtxDomain.h"
#include "PtxEvolution.h"
#include "../DatRef.h"
#include "../../Color32.h"

namespace rage
{
    class ptxEvent
    {
    public:
        enum class eEventType : uint32_t
        {
            EMITTER = 0,
            EFFECT,
            COUNT,
        };

    public:
        ptxEvent(const datResource& rsc) : mEvoGroup(rsc) {}

        virtual ~ptxEvent() = default;

        void AddToLayout(RSC5Layout& layout, uint32_t depth);
        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth);

        void Place(void* that, const datResource& rsc);

        uint32_t GetObjectSize() const;

        virtual void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) = 0;

        int32_t field_4;
        float mTriggerTime;
        datOwner<ptxEvolutionGroup> mEvoGroup;
        float field_10;
        eEventType mType;
        int32_t mTriggerCap;
        int32_t field_1C;

    protected:
        const char* TypeToString(eEventType type);
        eEventType StringToType(const char* str);

        void WriteToJsonBase(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
    };
    ASSERT_SIZE(ptxEvent, 0x20);


    struct ptxEffectOverridables
    {
        void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);

        Vector3 mSizeScale;
        float pad_4;
        float mDuration;
        float mPlaybackRate;
        Color32 mColorTint;
        float mZoom;
        uint32_t mWhichFields;
        int8_t field_24[12];
    };
    ASSERT_SIZE(ptxEffectOverridables, 0x30);

    class ptxEventEffect : public ptxEvent
    {
    public:
        ptxEventEffect(const datResource& rsc);

        void AddToLayout(RSC5Layout& layout, uint32_t depth);
        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth);

        void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override;

        Vector3 mRotationMin;
        int8_t field_2C[0x4];
        ptxEffectOverridables mOverrideMins;
        ptxEffectOverridables mOverrideMaxes;
        char* mEffectName;
        //unknown type - always null
        datOwner<void*> field_94;
        int32_t field_98;
        datOwner<ptxDomain> mEmitterDomain;
        int8_t field_A0[4];
        int8_t field_A4;
        int8_t field_A5[10];
        int8_t field_AF;
    };
    ASSERT_SIZE(ptxEventEffect, 0xB0);


    class ptxEventEmitter : public ptxEvent
    {
    public:
        ptxEventEmitter(const datResource& rsc);

        void AddToLayout(RSC5Layout& layout, uint32_t depth);
        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth);

        void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override;

        float mDurationScalarMin;
        float mDurationScalarMax;
        float mTimeScalarMin;
        float mTimeScalarMax;
        float mZoomMin;
        float mZoomMax;
        Color32 mColorTintMin;
        Color32 mColorTintMax;
        char* mEmmiterRuleName;
        char* mPtxRuleName;
        datRef<class ptxEmitRuleStd> mEmitRule;
        datRef<class ptxRule> mRule;
    };
    ASSERT_SIZE(ptxEventEmitter, 0x50);
};