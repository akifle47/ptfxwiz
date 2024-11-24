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

        ptxTimeLine(const datResource& rsc) : mEvents(rsc), field_18(rsc) {}

        void AddToLayout(RSC5Layout& layout, uint32_t depth);
        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth);

        float field_4;
        float mPreUpdate;
        int32_t mNumLoops;
        atArray<datOwner<ptxEvent>> mEvents;
        datOwner<void*> field_18;
        int8_t field_1C[8];
    };
    ASSERT_SIZE(ptxTimeLine, 0x24);
}