#pragma once
#include "rapidjson/include/prettywriter.h"
#include "PtxEvent.h"
#include "PtxEffectRule.h"
#include "../Resource.h"
#include "../DatRef.h"
#include "../../Utils.h"

#undef GetObject

namespace rage
{
    class ptxTriggerEvent
    {
    public:
        ptxTriggerEvent() : mOverrideMins(), mOverrideMaxes(), mEffectRule(nullptr), mType(0), mTime(-1.0f), mToggle1(false), field_6D{0, 0}, field_6F{0}
        {}

        ptxTriggerEvent(const datResource& rsc) : mOverrideMins(rsc), mOverrideMaxes(rsc), mEffectRule(rsc) {}

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            mEffectRule.AddToLayout(layout, depth);
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            mEffectRule.SerializePtrs(layout, rsc, depth);
        }

        void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
        {
            writer.StartObject();
            {
                writer.String("OverrideMins");
                mOverrideMins.WriteToJson(writer);
                writer.String("OverrideMaxes");
                mOverrideMaxes.WriteToJson(writer);

                if(mEffectRule.Get())
                {
                    writer.String("EffectRule");
                    writer.String(mEffectRule->mName);
                }

                writer.String("Type");
                writer.Int(mType);

                writer.String("Time");
                writer.Double((double)mTime);

                writer.String("Toggle1");
                writer.Bool(mToggle1);

                writer.String("field_6F");
                writer.Int(field_6F);
            }
            writer.EndObject();
        }

        void LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object)
        {
            if(object.HasMember("OverrideMins"))
            {
                if(object["OverrideMins"].IsObject())
                {
                    auto overrideObject = object["OverrideMins"].GetObject();
                    mOverrideMins.LoadFromJson(overrideObject);
                }
            }
            if(object.HasMember("OverrideMaxes"))
            {
                if(object["OverrideMaxes"].IsObject())
                {
                    auto overrideObject = object["OverrideMaxes"].GetObject();
                    mOverrideMaxes.LoadFromJson(overrideObject);
                }
            }

            if(object.HasMember("EffectRule"))
            {
                //this stinks but i cant think of a way of doing this that stinks less rn
                char* effectRuleName = new char[32];
                strncpy(effectRuleName, object["EffectRule"].GetString(), 32);
                mEffectRule = {(ptxEffectRule*)effectRuleName};
            }

            mType = object["Type"].GetInt();
            mTime = object["Time"].GetFloat();
            mToggle1 = object["Toggle1"].GetBool();
            field_6F = object["field_6F"].GetInt();
        };

        ptxEffectOverridables mOverrideMins;
        ptxEffectOverridables mOverrideMaxes;
        datRef<ptxEffectRule> mEffectRule;
        int32_t mType;
        float mTime;
        bool mToggle1;
        int8_t field_6D[0x2];
        int8_t field_6F;
    };
    ASSERT_SIZE(ptxTriggerEvent, 0x70);
}