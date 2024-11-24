#include "PtxEvolution.h"
#include "../StringHash.h"

namespace rage
{
    ptxEvolutionGroup::ptxEvolutionGroup(const datResource& rsc) : mEvoList(rsc), mEvoBlendModeList(rsc)
    {
        //created at runtime
        /*
        mEvoListIndices = new ptxEvoHashIndexList(mEvoList.GetCount());
        for(uint16_t i = 0; i < mEvoList.GetCount(); i++)
        {
            mEvoListIndices->AddToList(atStringHash(mEvoList[i]->mEvoName), i);
        }
        */
    }

    void ptxEvolutionGroup::AddToLayout(RSC5Layout& layout, uint32_t depth)
    {
        mEvoList.AddToLayout(layout, depth);
        mEvoBlendModeList.AddToLayout(layout, depth);
    }

    void ptxEvolutionGroup::SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
    {
        mEvoList.SerializePtrs(layout, rsc, depth);
        mEvoBlendModeList.SerializePtrs(layout, rsc, depth);
    }


    ptxEvolution::ptxEvolution(const datResource& rsc) : mPropList(rsc)
    {
        if(mEvoName)
            rsc.PointerFixUp(mEvoName);
    }

    void ptxEvolution::AddToLayout(RSC5Layout& layout, uint32_t depth)
    {
        mPropList.AddToLayout(layout, depth);

        if(mEvoName)
            layout.AddObject(mEvoName, RSC5Layout::eBlockType::VIRTUAL, strlen(mEvoName) + 1);
    }

    void ptxEvolution::SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
    {
        mPropList.SerializePtrs(layout, rsc, depth);

        if(mEvoName)
            layout.SerializePtr(mEvoName, strlen(mEvoName) + 1);
    }


    void ptxEvoPropList::AddToLayout(RSC5Layout& layout, uint32_t depth)
    {
        mPropList.AddToLayout(layout, depth);
    }

    void ptxEvoPropList::SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
    {
        mPropList.SerializePtrs(layout, rsc, depth);
    }


    void ptxEvoProp::AddToLayout(RSC5Layout& layout, uint32_t depth)
    {
        mKeyFrames.AddToLayout(layout, depth);
    }

    void ptxEvoProp::SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
    {
        mKeyFrames.SerializePtrs(layout, rsc, depth);
    }
}