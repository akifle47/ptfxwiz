#include "PtxEvolution.h"
#include "../StringHash.h"

namespace rage
{
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

    void ptxEvolutionGroup::SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
    {
        mEvoList.SerializePtrs(layout, rsc, depth);
        mEvoBlendModeList.SerializePtrs(layout, rsc, depth);
    }

    void ptxEvolutionGroup::WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();
        {
            writer.String("EvoList");
            writer.StartArray();
            for(uint16_t i = 0; i < mEvoList.GetCount(); i++)
            {
                mEvoList[i]->WriteToJson(writer);
            }
            writer.EndArray();

            writer.String("EvoBlendModeList");
            writer.StartArray();
            for(uint16_t i = 0; i < mEvoBlendModeList.GetCount(); i++)
            {
                ptxEvoBlendMode& blendMode = mEvoBlendModeList[i];
                writer.StartObject();
                {
                    writer.String("field_0");
                    writer.Int(blendMode.field_0);
                    writer.String("field_4");
                    writer.Int(blendMode.field_4);
                }
                writer.EndObject();
            }
            writer.EndArray();
        }
        writer.EndObject();
    }


    ptxEvolution::ptxEvolution(const datResource& rsc) : mPropList(rsc)
    {
        if(mEvoName)
            rsc.PointerFixUp(mEvoName);
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

            writer.String("PropList");
            writer.StartArray();
            for(uint16_t i = 0; i < mPropList.GetCount(); i++)
            {
                mPropList[i]->WriteToJson(writer);
            }
            writer.EndArray();
        }
        writer.EndObject();
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

            writer.String("PropList");
            writer.StartArray();
            for(uint16_t i = 0; i < mPropList.GetCount(); i++)
            {
                mPropList[i]->WriteToJson(writer);
            }
            writer.EndArray();
        }
        writer.EndObject();
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
}