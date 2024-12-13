#pragma once
#include "rapidjson/include/document.h"
#include "rapidjson/include/prettywriter.h"
#include "../../Utils.h"
#include "../math/Vector.h"
#include "PtxDomain.h"
#include "PtxEvolution.h"
#include "../DatRef.h"
#include "../Color32.h"

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
        ptxEvent(eEventType type) : field_4(0), mTriggerTime(0.0f), mEvoGroup(nullptr), field_10(0.0f), mType(type), mTriggerCap(-1), field_1C(-1)
        {}

        ptxEvent(const datResource& rsc) : mEvoGroup(rsc) {}

        virtual ~ptxEvent() = default;

        void AddToLayout(RSC5Layout& layout, uint32_t depth);
        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth);

        void Place(void* that, const datResource& rsc);

        static const char* TypeToString(eEventType type);
        static eEventType StringToType(const char* str);
        uint32_t GetObjectSize() const;

        virtual void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) = 0;
        virtual void LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object) = 0;

        int32_t field_4;
        float mTriggerTime;
        datOwner<ptxEvolutionGroup> mEvoGroup;
        float field_10;
        eEventType mType;
        int32_t mTriggerCap;
        int32_t field_1C;

    protected:
        void WriteToJsonBase(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
        void LoadFromJsonBase(rapidjson::GenericObject<true, rapidjson::Value>& object);
    };
    ASSERT_SIZE(ptxEvent, 0x20);


    struct ptxEffectOverridables
    {
        ptxEffectOverridables() : mSizeScale{.x = 1.0f, .y = 1.0f, .z = 1.0f}, pad_4(0.0f), mDuration(0.0f), mPlaybackRate(1.0f),
                                  mZoom(100.0f), mWhichFields(0x20), field_24{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
        {
            mColorTint.Color = 0xFFFFFFFF;
        }

        ptxEffectOverridables(const rage::datResource&) {}

        void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
        void LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object);

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
        ptxEventEffect() : ptxEvent(eEventType::EFFECT), mRotationMin{.x = 0.0f, .y = 0.0f, .z = 0.0f}, field_2C{}, mEffectName(nullptr), mEffectRule(nullptr),
                           field_98(0), mEmitterDomain(nullptr), field_A0{}, field_A4(0), field_A5{}, field_AF(0)
        {}

        ptxEventEffect(const datResource& rsc);

        ~ptxEventEffect()
        {
            if(mEmitterDomain.Get())
            {
                delete mEmitterDomain.Get();
                mEmitterDomain = {nullptr};
            }
        }

        void AddToLayout(RSC5Layout& layout, uint32_t depth);
        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth);

        void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override;
        void LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object) override;

        Vector3 mRotationMin;
        int8_t field_2C[0x4];
        ptxEffectOverridables mOverrideMins;
        ptxEffectOverridables mOverrideMaxes;
        char* mEffectName;
        datRef<class ptxEffectRule> mEffectRule;
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
        ptxEventEmitter() : ptxEvent(eEventType::EMITTER), mDurationScalarMin(1.0f), mDurationScalarMax(1.0f), mTimeScalarMin(1.0f), mTimeScalarMax(1.0f),
                            mZoomMin(1.0f), mZoomMax(1.0f), mEmmiterRuleName(nullptr), mPtxRuleName(nullptr), mEmitRule(nullptr), mRule(nullptr)
        {
            mColorTintMin.Color = 0xFFFFFFFF;
            mColorTintMax.Color = 0xFFFFFFFF;
        }

        ptxEventEmitter(const datResource& rsc);

        ~ptxEventEmitter()
        {
            if(mEmmiterRuleName)
            {
                delete[] mEmmiterRuleName;
                mEmmiterRuleName = nullptr;
            }

            if(mPtxRuleName)
            {
                delete[] mPtxRuleName;
                mPtxRuleName = nullptr;
            }
        }

        void AddToLayout(RSC5Layout& layout, uint32_t depth);
        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth);

        void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override;
        void LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object) override;

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