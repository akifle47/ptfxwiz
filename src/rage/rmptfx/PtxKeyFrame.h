#pragma once
#include "rapidjson/include/document.h"
#include "rapidjson/include/prettywriter.h"
#include "../../Utils.h"
#include "../Base.h"
#include "../Array.h"
#include "../math/Vector.h"

#undef GetObject

namespace rage
{
    class ptxNetObject : public datBase
    {
    public:
        ptxNetObject() = default;
        ptxNetObject(const rage::datResource&) {}

        int32_t field_4;
        int32_t field_8;
        int32_t field_C;
        int32_t field_10;
    };
    ASSERT_SIZE(ptxNetObject, 0x14);


    struct ptxKeyFrameEntry
    {
        float Time;

        int8_t field_4[0xC];

        Vector4 Value;
        Vector4 Delta;
    };
    ASSERT_SIZE(ptxKeyFrameEntry, 0x30);


    class rmPtfxKeyframe : public ptxNetObject
    {
    public:
        rmPtfxKeyframe() = default;

        rmPtfxKeyframe(const datResource& rsc) : ptxNetObject(rsc), mEntries(rsc) 
        {}

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            mEntries.AddToLayout(layout, depth);
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            mEntries.SerializePtrs(layout, rsc, depth);
        }

        void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
        {
            writer.StartObject();
            {
                if(mEntries.GetCount())
                {
                    writer.String("KeyFrameEntries");
                    writer.StartArray();
                    {
                        for(uint16_t i = 0; i < mEntries.GetCount(); i++)
                        {
                            writer.StartObject();
                            {
                                ptxKeyFrameEntry& entry = mEntries[i];

                                writer.String("Time");
                                writer.Double((double)entry.Time);

                                writer.SetFormatOptions(rapidjson::kFormatSingleLineArray);
                                writer.String("Value");
                                writer.StartArray();
                                {
                                    writer.Double((double)entry.Value.x);
                                    writer.Double((double)entry.Value.y);
                                    writer.Double((double)entry.Value.z);
                                    writer.Double((double)entry.Value.w);
                                }
                                writer.EndArray();

                                writer.String("Delta");
                                writer.StartArray();
                                {
                                    writer.Double((double)entry.Delta.x);
                                    writer.Double((double)entry.Delta.y);
                                    writer.Double((double)entry.Delta.z);
                                    writer.Double((double)entry.Delta.w);
                                }
                                writer.EndArray();
                                writer.SetFormatOptions(rapidjson::kFormatDefault);
                            }
                            writer.EndObject();
                        }
                    }
                    writer.EndArray();
                }

                writer.String("field_20");
                writer.Int(field_20);

                writer.String("field_24");
                writer.Int(field_24);
            }
            writer.EndObject();
        }

        void LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object)
        {
            if(object.HasMember("KeyFrameEntries") && object["KeyFrameEntries"].IsArray())
            {
                auto keyFrameEntriesArray = object["KeyFrameEntries"].GetArray();
                mEntries = {(uint16_t)keyFrameEntriesArray.Size()};
                for(const auto& keyFrameEntryValue : keyFrameEntriesArray)
                {
                    if(keyFrameEntryValue.IsObject())
                    {
                       auto keyFrameEntryObject = keyFrameEntryValue.GetObject();
                       ptxKeyFrameEntry& entry = mEntries.Append();

                       entry.Time = keyFrameEntryObject["Time"].GetFloat();

                       entry.Value.x = keyFrameEntryObject["Value"].GetArray()[0].GetFloat();
                       entry.Value.y = keyFrameEntryObject["Value"].GetArray()[1].GetFloat();
                       entry.Value.z = keyFrameEntryObject["Value"].GetArray()[2].GetFloat();
                       entry.Value.w = keyFrameEntryObject["Value"].GetArray()[3].GetFloat();

                       entry.Delta.x = keyFrameEntryObject["Delta"].GetArray()[0].GetFloat();
                       entry.Delta.y = keyFrameEntryObject["Delta"].GetArray()[1].GetFloat();
                       entry.Delta.z = keyFrameEntryObject["Delta"].GetArray()[2].GetFloat();
                       entry.Delta.w = keyFrameEntryObject["Delta"].GetArray()[3].GetFloat();
                    }
                }
            }

            field_20 = object["field_20"].GetUint();
            field_24 = object["field_24"].GetUint();
        }

        int8_t field_15;
        atArray<ptxKeyFrameEntry> mEntries;
        int32_t field_20;
        int32_t field_24;
    };
    ASSERT_SIZE(rmPtfxKeyframe, 0x28);
}