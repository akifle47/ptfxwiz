#include "PtxEmitRule.h"

namespace rage
{
    void rage::ptxEmitRule::Place(void* that, const datResource& rsc)
    {
        new(that) ptxEmitRuleStd(rsc);
    }
}