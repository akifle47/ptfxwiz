#pragma once
#include "../../Utils.h"
//#include "PtxEffectRule.h"

namespace rage
{
    class ptxEvent
    {
    public:
        virtual ~ptxEvent() = default;

        int32_t field_4;
        float field_8;
        struct ptxEffectRule_obj1* field_C;
        float field_10;
        //type? game creates a ptxEventEmitter if this is 0 and a ptxEventEffect if its 1
        int32_t field_14;
        int32_t field_18;
        int32_t field_1C;
    };
    ASSERT_SIZE(ptxEvent, 0x20);
};