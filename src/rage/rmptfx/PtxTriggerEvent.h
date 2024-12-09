#pragma once
#include "rapidjson/include/prettywriter.h"
#include "PtxEvent.h"
#include "PtxEffectRule.h"
#include "../Resource.h"
#include "../DatRef.h"
#include "../../Utils.h"

namespace rage
{
    class ptxTriggerEvent
    {
    public:
        ptxTriggerEvent(const datResource& rsc) : mEffectRule(rsc) {}

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

                writer.String("EffectRule");
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