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

    const char* ptxEvent::TypeToString(eEventType type)
    {
        static const char* lut[] {"EMITTER", "EFFECT"};

        if((uint32_t)type > (uint32_t)eEventType::COUNT)
            return "INVALID_TYPE";
        else
            return lut[(uint32_t)type];
    }

    ptxEvent::eEventType ptxEvent::StringToType(const char* str)
    {
        static const char* lut[] {"EMITTER", "EFFECT"};

        if(!str)
            return eEventType::COUNT;

        for(size_t i = 0; i < std::size(lut); i++)
        {
            if(stricmp(str, lut[i]) == 0)
                return (eEventType)i;
        }

        return eEventType::COUNT;
    }

    void ptxEvent::WriteToJsonBase(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.String("Type");
        writer.String(TypeToString(mType));

        writer.String("field_4");
        writer.Int(field_4);

        writer.String("TriggerTime");
        writer.Double((double)mTriggerTime);

        if(mEvoGroup.Get())
        {
            writer.String("EvoGroup");
            mEvoGroup->WriteToJson(writer);
        }

        writer.String("field_10");
        writer.Double((double)field_10);

        writer.String("TriggerCap");
        writer.Int(mTriggerCap);

        writer.String("field_1C");
        writer.Int(field_1C);
    }


    void ptxEffectOverridables::WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();
        {
            writer.SetFormatOptions(rapidjson::kFormatSingleLineArray);

            writer.String("SizeScale");
            writer.StartArray();
            {
                writer.Double((double)mSizeScale.x);
                writer.Double((double)mSizeScale.y);
                writer.Double((double)mSizeScale.z);
            }
            writer.EndArray();

            writer.String("Duration");
            writer.Double((double)mDuration);

            writer.String("PlaybackRate");
            writer.Double((double)mPlaybackRate);

            writer.String("ColorTint");
            writer.StartArray();
            {
                writer.Uint((uint32_t)mColorTint.Red);
                writer.Uint((uint32_t)mColorTint.Green);
                writer.Uint((uint32_t)mColorTint.Blue);
                writer.Uint((uint32_t)mColorTint.Alpha);
            }
            writer.EndArray();

            writer.String("Zoom");
            writer.Double((double)mZoom);

            writer.String("WhichFields");
            writer.Uint(mWhichFields);

            writer.SetFormatOptions(rapidjson::kFormatDefault);
        }
        writer.EndObject();
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

    void ptxEventEffect::WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();
        {
            if(mEffectName)
            {
                writer.String("EffectName");
                writer.String(mEffectName);
            }

            WriteToJsonBase(writer);

            writer.SetFormatOptions(rapidjson::kFormatSingleLineArray);
            writer.String("RotationMin");
            writer.StartArray();
            {
                writer.Double((double)mRotationMin.x);
                writer.Double((double)mRotationMin.y);
                writer.Double((double)mRotationMin.z);
            }
            writer.EndArray();
            writer.SetFormatOptions(rapidjson::kFormatDefault);

            mOverrideMins.WriteToJson(writer);
            mOverrideMaxes.WriteToJson(writer);

            writer.String("field_98");
            writer.Uint(field_98);

            writer.String("EmitterDomain");
            if(mEmitterDomain.Get())
                mEmitterDomain->WriteToJson(writer);
            else
                writer.Null();

            writer.String("field_A4");
            writer.Int(field_A4);
            writer.String("field_AF");
            writer.Int(field_AF);
        }
        writer.EndObject();
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

    void ptxEventEmitter::WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();
        {
            WriteToJsonBase(writer);

            if(mEmmiterRuleName)
            {
                writer.String("EmmiterRuleName");
                writer.String(mEmmiterRuleName);
            }
            if(mPtxRuleName)
            {
                writer.String("PtxRuleName");
                writer.String(mPtxRuleName);
            }

            writer.String("EmitRule");
            if(mEmitRule.Get() && mEmitRule->mName)
                writer.String(mEmitRule->mName);
            else
                writer.Null();

            writer.String("Rule");
            if(mRule.Get() && mRule->mName)
                writer.String(mRule->mName);
            else
                writer.Null();

            writer.String("DurationScalarMin");
            writer.Double((double)mDurationScalarMin);
            writer.String("DurationScalarMax");
            writer.Double((double)mDurationScalarMax);
            writer.String("TimeScalarMin");
            writer.Double((double)mTimeScalarMin);
            writer.String("TimeScalarMax");
            writer.Double((double)mTimeScalarMax);
            writer.String("ZoomMin");
            writer.Double((double)mZoomMin);
            writer.String("ZoomMax");
            writer.Double((double)mZoomMax);

            writer.SetFormatOptions(rapidjson::kFormatSingleLineArray);
            writer.String("ColorTintMin");
            writer.StartArray();
            {
                writer.Uint((uint32_t)mColorTintMin.Red);
                writer.Uint((uint32_t)mColorTintMin.Green);
                writer.Uint((uint32_t)mColorTintMin.Blue);
                writer.Uint((uint32_t)mColorTintMin.Alpha);
            }
            writer.EndArray();

            writer.String("ColorTintMax");
            writer.StartArray();
            {
                writer.Uint((uint32_t)mColorTintMax.Red);
                writer.Uint((uint32_t)mColorTintMax.Green);
                writer.Uint((uint32_t)mColorTintMax.Blue);
                writer.Uint((uint32_t)mColorTintMax.Alpha);
            }
            writer.EndArray();
            writer.SetFormatOptions(rapidjson::kFormatDefault);
        }
        writer.EndObject();
    }
}