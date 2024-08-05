#pragma once
#include "../../Utils.h"
#include "../Array.h"
#include "PtxEvent.h"

namespace rage
{
    class ptxTimeLine
    {
    public:
        virtual ~ptxTimeLine() = default;

        float field_4;
        float field_8;
        int32_t field_C;
        atArray<ptxEvent> mEvents;
        void* field_18;
        int8_t field_1C[7];
        int8_t field_23;
    };
    ASSERT_SIZE(ptxTimeLine, 0x24);
}