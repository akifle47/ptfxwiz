#include "PtxTimeline.h"
#include "PtxEffectRule.h"

namespace rage
{
    ptxTimeLine::ptxTimeLine(const datResource& rsc) : mEvents(rsc), mEffectRule(rsc) {}

    void ptxTimeLine::AddToLayout(RSC5Layout& layout, uint32_t depth)
    {
        mEvents.AddToLayout(layout, depth);
        mEffectRule.AddToLayout(layout, depth);
    }

    void ptxTimeLine::SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
    {
        mEvents.SerializePtrs(layout, rsc, depth);
        mEffectRule.SerializePtrs(layout, rsc, depth);
    }

    void ptxTimeLine::WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();
        {
            if(mEffectRule.Get() && mEffectRule->mName)
            {
                writer.String("EffectRule");
                writer.String(mEffectRule->mName);
            }

            writer.String("field_4");
            writer.Double((double)field_4);

            writer.String("mPreUpdate");
            writer.Double((double)mPreUpdate);

            writer.String("mNumLoops");
            writer.Int(mNumLoops);

            writer.String("Events");
            writer.StartArray();
            for(uint16_t i = 0; i < mEvents.GetCount(); i++)
            {
                if(mEvents[i].Get())
                {
                    mEvents[i]->WriteToJson(writer);
                }
            }
            writer.EndArray();
        }
        writer.EndObject();
    }
}