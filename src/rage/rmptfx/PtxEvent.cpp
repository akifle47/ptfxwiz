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

    void ptxEvent::LoadFromJsonBase(rapidjson::GenericObject<true, rapidjson::Value>& object)
    {
        field_4 = object["field_4"].GetInt();
        mTriggerTime = object["TriggerTime"].GetFloat();

        if(object.HasMember("EvoGroup"))
        {
            mEvoGroup = {new ptxEvolutionGroup()};
            JsonHelpers::LoadMemberObject(*mEvoGroup, object, "EvoGroup");
        }

        field_10 = object["field_10"].GetFloat();
        mTriggerCap = object["TriggerCap"].GetInt();
        field_1C = object["field_1C"].GetInt();
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

    void ptxEffectOverridables::LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object)
    {
        mSizeScale.x = object["SizeScale"].GetArray()[0].GetFloat();
        mSizeScale.y = object["SizeScale"].GetArray()[1].GetFloat();
        mSizeScale.z = object["SizeScale"].GetArray()[1].GetFloat();

        mDuration = object["Duration"].GetFloat();
        mPlaybackRate = object["PlaybackRate"].GetFloat();

        auto colorTintArray = object["ColorTint"].GetArray();
        mColorTint.Red   = (uint8_t)colorTintArray[0].GetUint();
        mColorTint.Green = (uint8_t)colorTintArray[1].GetUint();
        mColorTint.Blue  = (uint8_t)colorTintArray[2].GetUint();
        mColorTint.Alpha = (uint8_t)colorTintArray[3].GetUint();

        mZoom = object["Zoom"].GetFloat();
        mWhichFields = object["WhichFields"].GetUint();
    }


    ptxEventEffect::ptxEventEffect(const datResource& rsc) : ptxEvent(rsc), mEffectRule(rsc), mEmitterDomain(rsc)
    {
        if(mEffectName)
            rsc.PointerFixUp(mEffectName);
    }

    void ptxEventEffect::AddToLayout(RSC5Layout& layout, uint32_t depth)
    {
        mEffectRule.AddToLayout(layout, depth);
        mEmitterDomain.AddToLayout(layout, depth);

        if(mEffectName)
            layout.AddObject(mEffectName, RSC5Layout::eBlockType::VIRTUAL, strlen(mEffectName) + 1);
    }

    void ptxEventEffect::SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
    {
        mEffectRule.SerializePtrs(layout, rsc, depth);
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

            if(mEffectRule.Get())
            {
                writer.String("EffectRule");
                writer.String(mEffectRule->mName);
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

            writer.String("OverrideMins");
            mOverrideMins.WriteToJson(writer);
            writer.String("OverrideMaxes");
            mOverrideMaxes.WriteToJson(writer);

            writer.String("field_98");
            writer.Uint(field_98);

            if(mEmitterDomain.Get())
            {
                writer.String("EmitterDomain");
                mEmitterDomain->WriteToJson(writer);
            }

            writer.String("field_A4");
            writer.Int(field_A4);
        }
        writer.EndObject();
    }

    static ptxDomain* CreateDomain(uint32_t domainFunction, ptxDomain::eDomainType type)
    {
        switch(type)
        {
            case ptxDomain::eDomainType::BOX:
                return new ptxDomainBox(domainFunction);
            break;

            case ptxDomain::eDomainType::SPHERE:
                return new ptxDomainSphere(domainFunction);
            break;

            case ptxDomain::eDomainType::CYLINDER:
                return new ptxDomainCylinder(domainFunction);
            break;

            case ptxDomain::eDomainType::VORTEX:
                return new ptxDomainVortex(domainFunction);
            break;

            default:
                return nullptr;
        }
    }

    void ptxEventEffect::LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object)
    {
        LoadFromJsonBase(object);
        
        if(object.HasMember("EffectName"))
            mEffectName = strdup(object["EffectName"].GetString());

        if(object.HasMember("EffectRule"))
            mEffectRule = {(ptxEffectRule*)strdup(object["EffectName"].GetString())};

        mRotationMin.x = object["RotationMin"].GetArray()[0].GetFloat();
        mRotationMin.y = object["RotationMin"].GetArray()[1].GetFloat();
        mRotationMin.z = object["RotationMin"].GetArray()[2].GetFloat();

        JsonHelpers::LoadMemberObject(mOverrideMins, object, "OverrideMins");
        JsonHelpers::LoadMemberObject(mOverrideMaxes, object, "OverrideMaxes");

        field_98 = object["field_98"].GetUint();

        if(object.HasMember("EmitterDomain") && object["EmitterDomain"].IsObject())
        {
            auto domainObj = object["EmitterDomain"].GetObject();
            ptxDomain::eDomainType type = ptxDomain::StringToType(domainObj["Type"].GetString());
            
            mEmitterDomain = { CreateDomain(0, type) };
            if(mEmitterDomain.Get())
                mEmitterDomain->LoadFromJson(domainObj);
            else
            {
                Log::Error("ptxEventEffect \"%s\"'s EmitterDomain uses an invalid domain type - %d", mEffectName ? mEffectName : "null", type);
            }
        }

        field_A4 = object["field_A4"].GetInt();
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

    void ptxEventEmitter::LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object)
    {
        LoadFromJsonBase(object);

        if(object.HasMember("EmmiterRuleName"))
            mEmmiterRuleName = strdup(object["EmmiterRuleName"].GetString());

        if(object.HasMember("PtxRuleName"))
            mPtxRuleName = strdup(object["PtxRuleName"].GetString());

        mDurationScalarMin = object["DurationScalarMin"].GetFloat();
        mDurationScalarMax = object["DurationScalarMax"].GetFloat();
        mTimeScalarMin = object["TimeScalarMin"].GetFloat();
        mTimeScalarMax = object["TimeScalarMax"].GetFloat();
        mZoomMin = object["ZoomMin"].GetFloat();
        mZoomMax = object["ZoomMax"].GetFloat();

        mColorTintMin.Red   = (uint8_t)object["ColorTintMin"].GetArray()[0].GetUint();
        mColorTintMin.Green = (uint8_t)object["ColorTintMin"].GetArray()[1].GetUint();
        mColorTintMin.Blue  = (uint8_t)object["ColorTintMin"].GetArray()[2].GetUint();
        mColorTintMin.Alpha = (uint8_t)object["ColorTintMin"].GetArray()[3].GetUint();

        mColorTintMax.Red   = (uint8_t)object["ColorTintMax"].GetArray()[0].GetUint();
        mColorTintMax.Green = (uint8_t)object["ColorTintMax"].GetArray()[1].GetUint();
        mColorTintMax.Blue  = (uint8_t)object["ColorTintMax"].GetArray()[2].GetUint();
        mColorTintMax.Alpha = (uint8_t)object["ColorTintMax"].GetArray()[3].GetUint();
    }
}