#include "PtxRule.h"

namespace rage
{
    void ptxRule::AddToLayout(RSC5Layout& layout, uint32_t depth)
    {
        mSpawnEffectA.AddToLayout(layout, depth);
        mSpawnEffectB.AddToLayout(layout, depth);

        layout.AddObject(mName, RSC5Layout::eBlockType::VIRTUAL, strlen(mName) + 1);

        if(!strcmp(mClassName, "ptxsprite"))
        {
            ((ptxSprite*)this)->AddToLayout(layout, depth);
        }
        else if(!strcmp(mClassName, "ptxmodel"))
        {
            ((ptxModel*)this)->AddToLayout(layout, depth);
        }
        else
        {
            Log::Error("Invalid ptxRule type - %s", mClassName);
        }
    }

    void ptxRule::SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
    {
        mSpawnEffectA.SerializePtrs(layout, rsc, depth);
        mSpawnEffectB.SerializePtrs(layout, rsc, depth);

        layout.SerializePtr(mName, strlen(mName) + 1);

        if(!strcmp(mClassName, "ptxsprite"))
        {
            ((ptxSprite*)this)->SerializePtrs(layout, rsc, depth);
        }
        else if(!strcmp(mClassName, "ptxmodel"))
        {
            ((ptxModel*)this)->SerializePtrs(layout, rsc, depth);
        }
        else
        {
            Log::Error("Invalid ptxRule type - %s", mClassName);
        }
    }

    void ptxRule::Place(void* that, const datResource& rsc)
    {
        if(!strcmp(mClassName, "ptxsprite"))
        {
            new(that) ptxSprite(rsc);
        }
        else if(!strcmp(mClassName, "ptxmodel"))
        {
            new(that) ptxModel(rsc);
        }
        else
        {
            Log::Error("Invalid ptxRule type - %s", mClassName);
        }
    }
}