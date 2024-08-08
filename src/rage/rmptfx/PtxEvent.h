#pragma once
#include "../../Utils.h"
#include "../Vector.h"
#include "PtxDomain.h"
#include "PtxTriggerEvent.h"
#include "PtxEvolution.h"

namespace rage
{
    class ptxEvent
    {
    public:
        enum class eEventType : uint32_t
        {
            EMITTER = 0,
            EFFECT = 1,
        };

    public:
        ptxEvent(const datResource& rsc) : mEvoGroup(rsc) {}

        virtual ~ptxEvent() = default;

        void Place(void* that, const datResource& rsc);

        int32_t field_4;
        float mTriggerTime;
        datOwner<ptxEvolutionGroup> mEvoGroup;
        float field_10;
        eEventType mType;
        int32_t mTriggerCap;
        int32_t field_1C;
    };
    ASSERT_SIZE(ptxEvent, 0x20);


    //not really sure about the size of this
    struct ptxEventEffect_obj1
    {
        ptxTriggerEventScalars mSpawnEffect;
    };
    ASSERT_SIZE(ptxEventEffect_obj1, 0x24);

    class ptxEventEffect : public ptxEvent
    {
    public:
        ptxEventEffect(const datResource& rsc) : ptxEvent(rsc), field_94(rsc), mEmitterDomain(rsc)
        {
            if(mEffectName)
                rsc.PointerFixUp(mEffectName);
        }

        Vector3 mRotationMin;
        int8_t field_2C[0x4];
        ptxEventEffect_obj1 mOverrideMins;
        int8_t field_54[0x10];
        ptxEventEffect_obj1 mOverrideMaxes;
        int8_t field_88[0x8];
        char* mEffectName;
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
        ptxEventEmitter(const datResource& rsc) : ptxEvent(rsc), field_48(rsc), field_4C(rsc)
        {
            if(mEmmiterRuleName)
                rsc.PointerFixUp(mEmmiterRuleName);
            if(mPtxRuleName)
                rsc.PointerFixUp(mPtxRuleName);
        }

        float mDurationScalarMin;
        float mDurationScalarMax;
        float mTimeScalarMin;
        float mTimeScalarMax;
        float mZoomMin;
        float mZoomMax;
        uint32_t mColorTintMin;
        uint32_t mColorTintMax;
        char* mEmmiterRuleName;
        char* mPtxRuleName;
        datOwner<void*> field_48;
        datOwner<void*> field_4C;
    };
    ASSERT_SIZE(ptxEventEmitter, 0x50);
};