#include "PtxEffectRule.h"

namespace rage
{
    void ptxEffectRule::Place(void* that, const datResource& rsc)
    {
        new(that) ptxEffectRuleStd(rsc);
    }
}