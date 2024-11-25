#include "PtxTimeline.h"
#include "PtxEffectRule.h"

namespace rage
{
    ptxTimeLine::ptxTimeLine(const datResource& rsc) : mEvents(rsc), mEffectRule(rsc) {}

    void ptxTimeLine::AddToLayout(RSC5Layout& layout, uint32_t depth)
    {
        mEvents.AddToLayout(layout, depth);
        mEffectRule.AddToLayout(layout, depth);
    }

    void ptxTimeLine::SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
    {
        mEvents.SerializePtrs(layout, rsc, depth);
        mEffectRule.SerializePtrs(layout, rsc, depth);
    }
}