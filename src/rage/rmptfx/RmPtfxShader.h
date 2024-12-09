#pragma once
#include "rapidjson/include/prettywriter.h"
#include "../Array.h"
#include "RmPtfxShaderVar.h"
#include "../DatOwner.h"

namespace rage
{
    /*
    class rmPtfxShaderTemplate
    {
    public:
        virtual ~rmPtfxShaderTemplate()
        {
        }

        grmShaderFx* mShaderFx;
        char mName[128];
        int8_t field_88;
        atArray field_8C;
        int32_t mParamIndices[4];
        int8_t field_A4;
        int8_t field_A5;
        int8_t field_A6;
        int8_t field_A7;
    };
    ASSERT_SIZE(rmPtfxShaderTemplate, 0xA8);
    */

    class rmPtfxShader
    {
    public:
        rmPtfxShader(const datResource& rsc) : mVars(rsc), mShaderTemplate(rsc)
        {
            rsc.PointerFixUp(mName);
            rsc.PointerFixUp(mTechName);
        }

        virtual ~rmPtfxShader() = default;

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            mVars.AddToLayout(layout, depth);
            mShaderTemplate.AddToLayout(layout, depth);

            layout.AddObject(mName, RSC5Layout::eBlockType::VIRTUAL, strlen(mName) + 1);
            layout.AddObject(mTechName, RSC5Layout::eBlockType::VIRTUAL, strlen(mTechName) + 1);
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            mVars.SerializePtrs(layout, rsc, depth);
            mShaderTemplate.SerializePtrs(layout, rsc, depth);

            layout.SerializePtr(mName, strlen(mName) + 1);
            layout.SerializePtr(mTechName, strlen(mTechName) + 1);
        }

        void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
        {
            writer.StartObject();
            {
                writer.String("Name");
                writer.String(mName);

        void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);

                writer.String("field_1C");
                writer.Bool(field_1C);
            }
            writer.EndObject();
        }

        char* mName;
        char* mTechName;
        //computed at runtime
        datOwner<void*/*rmPtfxShaderTemplate*/> mShaderTemplate;
        int32_t mTechIndex;
        atArray<datOwner<rmPtfxShaderVar>> mVars;
        bool field_1C;
        int8_t field_1D[3];
    };
    ASSERT_SIZE(rmPtfxShader, 0x20);
}