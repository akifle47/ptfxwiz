#include "PtxEvent.h"
#include "PtxEmitRule.h"
#include "PtxRule.h"

namespace rage
{
    void ptxEvent::AddToLayout(RSC5Layout& layout, uint32_t depth)
    {
        mEvoGroup.AddToLayout(layout, depth);

        if(mType == eEventType::EMITTER)
        {
            ((ptxEventEmitter*)this)->AddToLayout(layout, depth);
        }
        else if(mType == eEventType::EFFECT)
        {
            ((ptxEventEffect*)this)->AddToLayout(layout, depth);
        }
        else
        {
            Log::Error("Invalid ptxEvent type - %d", (uint32_t)mType);
        }
    }

    void ptxEvent::SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
    {
        mEvoGroup.SerializePtrs(layout, rsc, depth);

        if(mType == eEventType::EMITTER)
        {
            ((ptxEventEmitter*)this)->SerializePtrs(layout, rsc, depth);
        }
        else if(mType == eEventType::EFFECT)
        {
            ((ptxEventEffect*)this)->SerializePtrs(layout, rsc, depth);
        }
        else
        {
            Log::Error("Invalid ptxEvent type - %d", (uint32_t)mType);
        }
    }

    void ptxEvent::Place(void* that, const datResource& rsc)
    {
        if(mType == eEventType::EMITTER)
        {
            new(that) ptxEventEmitter(rsc);
        }
        else if(mType == eEventType::EFFECT)
        {
            new(that) ptxEventEffect(rsc);
        }
        else
        {
            Log::Error("Invalid ptxEvent type - %d", (uint32_t)mType);
        }
    }

    uint32_t ptxEvent::GetObjectSize() const
    {
        if(mType == eEventType::EMITTER)
        {
            return sizeof(ptxEventEmitter);
        }
        else if(mType == eEventType::EFFECT)
        {
            return sizeof(ptxEventEffect);
        }
        else
        {
            Log::Error("Invalid ptxEvent type - %d", (uint32_t)mType);
        }
    }


    ptxEventEffect::ptxEventEffect(const datResource& rsc) : ptxEvent(rsc), field_94(rsc), mEmitterDomain(rsc)
    {
        if(mEffectName)
            rsc.PointerFixUp(mEffectName);
    }

    void ptxEventEffect::AddToLayout(RSC5Layout& layout, uint32_t depth)
    {
        field_94.AddToLayout(layout, depth);
        mEmitterDomain.AddToLayout(layout, depth);

        if(mEffectName)
            layout.AddObject(mEffectName, RSC5Layout::eBlockType::VIRTUAL, strlen(mEffectName) + 1);
    }

    void ptxEventEffect::SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
    {
        field_94.SerializePtrs(layout, rsc, depth);
        mEmitterDomain.SerializePtrs(layout, rsc, depth);

        if(mEffectName)
            layout.SerializePtr(mEffectName, strlen(mEffectName) + 1);
    }


    ptxEventEmitter::ptxEventEmitter(const datResource& rsc) : ptxEvent(rsc), mEmitRule(rsc), mRule(rsc)
    {
        if(mEmmiterRuleName)
            rsc.PointerFixUp(mEmmiterRuleName);
        if(mPtxRuleName)
            rsc.PointerFixUp(mPtxRuleName);
    }

    void ptxEventEmitter::AddToLayout(RSC5Layout& layout, uint32_t depth)
    {
        mEmitRule.AddToLayout(layout, depth);
        mRule.AddToLayout(layout, depth);

        if(mEmmiterRuleName)
            layout.AddObject(mEmmiterRuleName, RSC5Layout::eBlockType::VIRTUAL, strlen(mEmmiterRuleName) + 1);
        if(mPtxRuleName)
            layout.AddObject(mPtxRuleName, RSC5Layout::eBlockType::VIRTUAL, strlen(mPtxRuleName) + 1);
    }

    void ptxEventEmitter::SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
    {
        mEmitRule.SerializePtrs(layout, rsc, depth);
        mRule.SerializePtrs(layout, rsc, depth);

        if(mEmmiterRuleName)
            layout.SerializePtr(mEmmiterRuleName, strlen(mEmmiterRuleName) + 1);
        if(mPtxRuleName)
            layout.SerializePtr(mPtxRuleName, strlen(mPtxRuleName) + 1);
    }
}