#include "PtxEvolution.h"
#include "JsonHelpers.h"

namespace rage
{
    //hardcoding this for now until ptxEvoBlendMode is properly figured out
    static constexpr size_t EVO_BLEND_MODE_COUNT = 40;

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

    ptxEvolutionGroup::~ptxEvolutionGroup()
    {
        if(mEvoBlendModeList.GetCount())
        {
            for(uint16_t i = 0; i < mEvoBlendModeList.GetCount(); i++)
            {
                delete[] mEvoBlendModeList[i].field_0;
            }
        }
    }

    void ptxEvolutionGroup::SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
    {
        mEvoList.SerializePtrs(layout, rsc, depth);
        mEvoBlendModeList.SerializePtrs(layout, rsc, depth);
    }

    void ptxEvolutionGroup::WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();
        {
            if(mEvoList.GetCount())
            {
                writer.String("EvoList");
                writer.StartArray();
                for(uint16_t i = 0; i < mEvoList.GetCount(); i++)
                {
                    mEvoList[i]->WriteToJson(writer);
                }
                writer.EndArray();
            }

            if(mEvoBlendModeList.GetCount())
            {
                writer.String("EvoBlendModeList");
                writer.StartArray();
                for(uint16_t i = 0; i < mEvoBlendModeList.GetCount(); i++)
                {
                    ptxEvoBlendMode& blendMode = mEvoBlendModeList[i];
                    writer.StartObject();
                    {
                        writer.String("field_0");
                        writer.StartArray();
                        for(size_t j = 0; j < EVO_BLEND_MODE_COUNT; j++)
                        {
                            writer.Uint(blendMode.field_0[j]);
                        }
                        writer.EndArray();

                        //writer.String("field_4");
                        //writer.Int(blendMode.field_4);
                    }
                    writer.EndObject();
                }
                writer.EndArray();
            }

            writer.String("field_10");
            writer.StartArray();
            for(size_t i = 0; i < std::size(field_10); i++)
            {
                writer.Int(field_10[i]);
            }
            writer.EndArray();
        }
        writer.EndObject();
    }

    void ptxEvolutionGroup::LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object)
    {
        if(object.HasMember("EvoList"))
        {
            auto evoListArray = object["EvoList"].GetArray();
            mEvoList = {(uint16_t)evoListArray.Size()};
            for(auto& evoListValue : evoListArray)
            {
                auto evoListObject = evoListValue.GetObject();
                mEvoList.Append() = new ptxEvolution();
                mEvoList.Back()->LoadFromJson(evoListObject);
            }
        }

        if(object.HasMember("EvoBlendModeList"))
        {
            auto evoBlendModeArray = object["EvoBlendModeList"].GetArray();
            mEvoBlendModeList = {(uint16_t)evoBlendModeArray.Size()};
            for(auto& evoBlendModeValue : evoBlendModeArray)
            {
                mEvoBlendModeList.Append();
                mEvoBlendModeList.Back().field_0 = {new uint8_t[EVO_BLEND_MODE_COUNT]};
                mEvoBlendModeList.Back().field_4 = EVO_BLEND_MODE_COUNT;
                mEvoBlendModeList.Back().field_8 = EVO_BLEND_MODE_COUNT;
                memset(mEvoBlendModeList.Back().field_0, EVO_BLEND_MODE_COUNT, 0);

                auto field_0_array = evoBlendModeValue["field_0"].GetArray();
                uint32_t index = 0;
                for(auto& field_0_value : field_0_array)
                {
                    mEvoBlendModeList.Back().field_0[index++] = field_0_value.GetUint();
                }
                //*mEvoBlendModeList.Back().field_0 = evoListValue.GetObject()["field_0"].GetInt();
                //mEvoBlendModeList.Back().field_4 = evoListValue.GetObject()["field_4"].GetInt();
            }
        }

        if(object.HasMember("field_10"))
        {
            auto field_10Array = object["field_10"].GetArray();
            uint32_t index = 0;
            for(auto& field_10Value : field_10Array)
            {
                field_10[index] = (int8_t)field_10Value.GetInt();
                index++;
            }
        }
    }


    ptxEvolution::ptxEvolution() : mEvoName(nullptr), field_C{}
    {}

    ptxEvolution::ptxEvolution(const datResource& rsc) : mPropList(rsc)
    {
        if(mEvoName)
            rsc.PointerFixUp(mEvoName);
    }

    ptxEvolution::~ptxEvolution()
    {
        if(mEvoName)
        {
            delete[] mEvoName;
            mEvoName = nullptr;
        }

        for(uint16_t i = 0; i < mPropList.GetCount(); i++)
        {
            delete mPropList[i].Get();
        }
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

    void ptxEvolution::WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();
        {
            if(mEvoName)
            {
                writer.String("Name");
                writer.String(mEvoName);
            }

            if(mPropList.GetCount())
            {
                writer.String("PropList");
                writer.StartArray();
                for(uint16_t i = 0; i < mPropList.GetCount(); i++)
                {
                    mPropList[i]->WriteToJson(writer);
                }
                writer.EndArray();
            }
        }
        writer.EndObject();
    }

    void ptxEvolution::LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object)
    {
        if(object.HasMember("Name"))
            mEvoName = strdup(object["Name"].GetString());

        if(object.HasMember("PropList"))
        {
            auto propListArray = object["PropList"].GetArray();
            mPropList = {(uint16_t)propListArray.Size()};
            for(auto& propListValue : propListArray)
            {
                auto propListObject = propListValue.GetObject();
                mPropList.Append() = {new ptxEvoPropList()};
                mPropList.Back()->LoadFromJson(propListObject);
            }
        }
    }


    ptxEvoPropList::~ptxEvoPropList()
    {
        for(uint16_t i = 0; i < mPropList.GetCount(); i++)
        {
            delete mPropList[i].Get();
        }
    }

    void ptxEvoPropList::AddToLayout(RSC5Layout& layout, uint32_t depth)
    {
        mPropList.AddToLayout(layout, depth);
    }

    void ptxEvoPropList::SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
    {
        mPropList.SerializePtrs(layout, rsc, depth);
    }

    void ptxEvoPropList::WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();
        {
            writer.String("field_0");
            writer.Int(field_0);
            writer.String("field_4");
            writer.Int(field_4);
            writer.String("field_8");
            writer.Int(field_8);
            writer.String("field_C");
            writer.Int(field_C);
            writer.String("field_10");
            writer.Int(field_10);
            writer.String("field_14");
            writer.Int(field_14);
            writer.String("field_18");
            writer.Int(field_18);
            writer.String("field_1C");
            writer.Int(field_1C);
            writer.String("field_20");
            writer.Int(field_20);
            writer.String("field_24");
            writer.Int(field_24);

            if(mPropList.GetCount())
            {
                writer.String("PropList");
                writer.StartArray();
                for(uint16_t i = 0; i < mPropList.GetCount(); i++)
                {
                    mPropList[i]->WriteToJson(writer);
                }
                writer.EndArray();
            }
        }
        writer.EndObject();
    }

    void ptxEvoPropList::LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object)
    {
        field_0 = object["field_0"].GetInt();
        field_4 = object["field_4"].GetInt();
        field_8 = object["field_8"].GetInt();
        field_C = object["field_C"].GetInt();
        field_10 = object["field_10"].GetInt();
        field_14 = object["field_14"].GetInt();
        field_18 = object["field_18"].GetInt();
        field_1C = object["field_1C"].GetInt();
        field_20 = object["field_20"].GetInt();
        field_24 = object["field_24"].GetInt();

        if(object.HasMember("PropList"))
        {
            auto propListArray = object["PropList"].GetArray();
            mPropList = {(uint16_t)propListArray.Size()};
            for(auto& propListValue : propListArray)
            {
                auto propListObject = propListValue.GetObject();
                mPropList.Append() = {new ptxEvoProp()};
                mPropList.Back()->LoadFromJson(propListObject);
            }
        }
    }


    void ptxEvoProp::AddToLayout(RSC5Layout& layout, uint32_t depth)
    {
        mKeyFrames.AddToLayout(layout, depth);
    }

    void ptxEvoProp::SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
    {
        mKeyFrames.SerializePtrs(layout, rsc, depth);
    }

    void ptxEvoProp::WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();
        {
            writer.String("KeyFrames");
            mKeyFrames.WriteToJson(writer);

            writer.String("RegID");
            writer.Int(mRegID);
        }
        writer.EndObject();
    }

    void ptxEvoProp::LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object)
    {
        JsonHelpers::LoadMemberObject(mKeyFrames, object, "KeyFrames");
        mRegID = object["RegID"].GetInt();
    }
}