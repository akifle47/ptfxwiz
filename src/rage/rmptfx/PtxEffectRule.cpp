#include "PtxEffectRule.h"

namespace rage
{
    void ptxEffectRule::AddToLayout(RSC5Layout& layout, uint32_t depth)
    {
        mKFColorTint.AddToLayout(layout, depth);
        mKFColorTintMax.AddToLayout(layout, depth);
        mKFZoom.AddToLayout(layout, depth);
        mKFRotation.AddToLayout(layout, depth);
        mKFDataSphere.AddToLayout(layout, depth);
        mKFDataCapsuleA.AddToLayout(layout, depth);

        if(mName)
            layout.AddObject(mName, RSC5Layout::eBlockType::VIRTUAL, strlen(mName) + 1);

        mPtxEvoGroup.AddToLayout(layout, depth);

        ((ptxEffectRuleStd*)this)->AddToLayout(layout, depth);
    }

    void ptxEffectRule::SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
    {
        mKFColorTint.SerializePtrs(layout, rsc,  depth);
        mKFColorTintMax.SerializePtrs(layout, rsc,  depth);
        mKFZoom.SerializePtrs(layout, rsc,  depth);
        mKFRotation.SerializePtrs(layout, rsc,  depth);
        mKFDataSphere.SerializePtrs(layout, rsc,  depth);
        mKFDataCapsuleA.SerializePtrs(layout, rsc,  depth);

        if(mName)
            layout.SerializePtr(mName, strlen(mName) + 1);

        mPtxEvoGroup.SerializePtrs(layout, rsc,  depth);

        ((ptxEffectRuleStd*)this)->SerializePtrs(layout, rsc, depth);
    }

    void ptxEffectRule::Place(void* that, const datResource& rsc)
    {
        new(that) ptxEffectRuleStd(rsc);
    }


    void ptxEffectRuleStd::AddToLayout(RSC5Layout& layout, uint32_t depth)
    {
        mTimeline.AddToLayout(layout, depth);
    }

    void ptxEffectRuleStd::SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
    {
        mTimeline.SerializePtrs(layout, rsc, depth);
    }
}