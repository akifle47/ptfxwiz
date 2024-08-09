#include "PtxEvolution.h"
#include "../StringHash.h"

namespace rage
{
    ptxEvolutionGroup::ptxEvolutionGroup(const datResource& rsc) : mEvoList(rsc), mEvoBlendModeList(rsc)
    {
        mEvoListIndices = new ptxEvoHashIndexList(mEvoList.mCount);
        for(uint16_t i = 0; i < mEvoList.mCount; i++)
        {
            mEvoListIndices->AddToList(atStringHash(mEvoList[i]->mEvoName), i);
        }
    }
}