#pragma once
#include "../../Utils.h"
#include "../Vector.h"
#include "PtxDomain.h"
#include "PtxTriggerEvent.h"
//#include "PtxEffectRule.h"

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
        virtual ~ptxEvent() = default;

        int32_t field_4;
        float mTriggerTime;
        struct ptxEvolutionGroup* mEvoGroup;
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
        Vector3 mRotationMin;
        int8_t field_2C[0x4];
        ptxEventEffect_obj1 mOverrideMins;
        int8_t field_54[0x10];
        ptxEventEffect_obj1 mOverrideMaxes;
        int8_t field_88[0x8];
        char* mEffectName;
        datOwner<void*> field_94;
        int32_t field_98;
        ptxDomain* mEmitterDomain;
        int8_t field_A0[4];
        int8_t field_A4;
        int8_t field_A5[10];
        int8_t field_AF;
    };
    ASSERT_SIZE(ptxEventEffect, 0xB0);


    class ptxEventEmitter : public ptxEvent
    {
    public:
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