#pragma once
#include "PtxEvent.h"
#include "PtxEffectRule.h"
#include "../Resource.h"
#include "../DatRef.h"
#include "../../Utils.h"

namespace rage
{
    class ptxTriggerEvent
    {
    public:
        ptxTriggerEvent(const datResource& rsc) : mEffectRule(rsc) {}

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            mEffectRule.AddToLayout(layout, depth);
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            mEffectRule.SerializePtrs(layout, rsc, depth);
        }

        ptxEffectOverridables mOverrideMins;
        ptxEffectOverridables mOverrideMaxes;
        datRef<ptxEffectRule> mEffectRule;
        int32_t mType;
        float mTime;
        bool mToggle1;
        int8_t field_6D[0x2];
        int8_t field_6F;
    };
    ASSERT_SIZE(ptxTriggerEvent, 0x70);
}