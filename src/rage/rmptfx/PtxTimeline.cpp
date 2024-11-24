#include "PtxTimeline.h"

namespace rage
{
    void ptxTimeLine::AddToLayout(RSC5Layout& layout, uint32_t depth)
    {
        mEvents.AddToLayout(layout, depth);
        field_18.AddToLayout(layout, depth);
    }

    void ptxTimeLine::SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
    {
        mEvents.SerializePtrs(layout, rsc, depth);
        field_18.SerializePtrs(layout, rsc, depth);
    }
}