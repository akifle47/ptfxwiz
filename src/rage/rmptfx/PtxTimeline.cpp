#include "PtxTimeline.h"
#include "PtxEffectRule.h"

namespace rage
{
    ptxTimeLine::ptxTimeLine(const datResource& rsc) : mEvents(rsc), mEffectRule(rsc) {}

    ptxTimeLine::~ptxTimeLine()
    {
        for(uint16_t i = 0; i < mEvents.GetCount(); i++)
        {
            if(mEvents[i].Get())
            {
                delete mEvents[i].Get();
                mEvents[i] = {nullptr};
            }
        }
    }

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
            writer.String("field_4");
            writer.Double((double)field_4);

            writer.String("PreUpdate");
            writer.Double((double)mPreUpdate);

            writer.String("NumLoops");
            writer.Int(mNumLoops);

            if(mEvents.GetCount())
            {
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
        }
        writer.EndObject();
    }

    void ptxTimeLine::LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object)
    {
        field_4 = object["field_4"].GetFloat();
        mPreUpdate = object["PreUpdate"].GetFloat();
        mNumLoops = object["NumLoops"].GetInt();

        if(object.HasMember("Events"))
        {
            auto eventsArray = object["Events"].GetArray();
            mEvents = {(uint16_t)eventsArray.Size()};
            for(auto& eventValue : eventsArray)
            {
                auto eventObject = eventValue.GetObject();
                ptxEvent::eEventType type = ptxEvent::StringToType(eventObject["Type"].GetString());

                if(type == ptxEvent::eEventType::EMITTER)
                    mEvents.Append() = new ptxEventEmitter();
                else if(type == ptxEvent::eEventType::EFFECT)
                    mEvents.Append() = new ptxEventEffect();
                else
                    continue;

                mEvents.Back()->LoadFromJson(eventObject);
            }
        }
    }
}