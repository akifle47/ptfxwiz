#pragma once
#include "rapidjson/include/prettywriter.h"
#include "../../Utils.h"
#include "../Resource.h"
#include "../DatRef.h"
#include "PtxKeyFrame.h"
#include "../grcore/Texture.h"

namespace rage
{
    class rmPtfxShaderVar
    {
    public:
        enum class eVarType : uint8_t
        {
            BOOL = 0,
            INT,
            FLOAT,
            FLOAT2,
            FLOAT3,
            FLOAT4,
            TEXTURE,
            KEYFRAME,
            COUNT
        };

        rmPtfxShaderVar() {};

        rmPtfxShaderVar(const datResource& rsc) {};

        virtual ~rmPtfxShaderVar() = default;
        virtual void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) = 0;

        void AddToLayout(RSC5Layout& layout, uint32_t depth);
        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth);

        static const char* TypeToString(eVarType type)
        {
            static const char* lut[]
            {"BOOL", "INT", "FLOAT", "FLOAT2", "FLOAT3", "FLOAT4", "TEXTURE", "KEYFRAME"};

            if((uint32_t)type >= (uint32_t)eVarType::COUNT)
                return "INVALID_TYPE";
            else
                return lut[(uint32_t)type];
        }

        static eVarType StringToType(const char* str)
        {
            static const char* lut[]
            {"BOOL", "INT", "FLOAT", "FLOAT2", "FLOAT3", "FLOAT4", "TEXTURE", "KEYFRAME"};

            if(!str)
                return eVarType::COUNT;

            for(size_t i = 0; i < std::size(lut); i++)
            {
                if(stricmp(str, lut[i]) == 0)
                    return (eVarType)i;
            }

            return eVarType::COUNT;
        }

        void Place(void* that, const datResource& rsc);
        uint32_t GetObjectSize() const;

        int32_t field_4;
        char* mName;
        int32_t mIndex;
        int8_t field_10;
        int8_t field_11;
        eVarType mType;
        int8_t field_13[13];

    protected:
        void WriteToJsonBase(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
        {
            writer.String("Name");
            writer.String(mName);

            writer.String("Type");
            writer.String(TypeToString(mType));

            writer.String("Index");
            writer.Int(mIndex);
        }
    };
    ASSERT_SIZE(rmPtfxShaderVar, 0x20);


    class rmPtfxShaderVar_Bool : public rmPtfxShaderVar
    {
    public:
        rmPtfxShaderVar_Bool(const datResource& rsc) {}

        rmPtfxShaderVar_Bool(const datResource& rsc) : rmPtfxShaderVar(rsc) {}

        void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override
        {
            writer.StartObject();
            {
                WriteToJsonBase(writer);

                writer.String("Value");
                writer.Bool(mValue);
            }
            writer.EndObject();
        }

        bool mValue;
        int8_t pad[15];
    };
    ASSERT_SIZE(rmPtfxShaderVar_Bool, 0x30);


    class rmPtfxShaderVar_Int : public rmPtfxShaderVar
    {
    public:
        rmPtfxShaderVar_Int(const datResource& rsc) {}
        rmPtfxShaderVar_Int(const datResource& rsc) : rmPtfxShaderVar(rsc) {}

        void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override
        {
            writer.StartObject();
            {
                WriteToJsonBase(writer);

                writer.String("Value");
                writer.Int(mValue);
            }
            writer.EndObject();
        }

        int32_t mValue;
        int8_t pad[12];
    };
    ASSERT_SIZE(rmPtfxShaderVar_Int, 0x30);


    class rmPtfxShaderVar_Float : public rmPtfxShaderVar
    {
    public:
        rmPtfxShaderVar_Float(const datResource& rsc) {}

        rmPtfxShaderVar_Float(const datResource& rsc) : rmPtfxShaderVar(rsc) {}

        void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override
        {
            writer.StartObject();
            {
                WriteToJsonBase(writer);

                writer.String("Value");
                writer.Double((double)mValue);
            }
            writer.EndObject();
        }

        float mValue;
        int8_t pad[12];
    };
    ASSERT_SIZE(rmPtfxShaderVar_Float, 0x30);


    class rmPtfxShaderVar_Float2 : public rmPtfxShaderVar
    {
    public:
        rmPtfxShaderVar_Float2(const datResource& rsc) {}

        rmPtfxShaderVar_Float2(const datResource& rsc) : rmPtfxShaderVar(rsc) {}

        void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override
        {
            writer.StartObject();
            {
                WriteToJsonBase(writer);

                writer.SetFormatOptions(rapidjson::kFormatSingleLineArray);
                writer.String("Value");
                writer.StartArray();
                {
                    writer.Double((double)mValue[0]);
                    writer.Double((double)mValue[1]);
                }
                writer.EndArray();
                writer.SetFormatOptions(rapidjson::kFormatDefault);
            }
            writer.EndObject();
        }

        float mValue[2];
        int8_t pad[8];
    };
    ASSERT_SIZE(rmPtfxShaderVar_Float2, 0x30);


    class rmPtfxShaderVar_Float3 : public rmPtfxShaderVar
    {
    public:
        rmPtfxShaderVar_Float3(const datResource& rsc) {}
        rmPtfxShaderVar_Float3(const datResource& rsc) : rmPtfxShaderVar(rsc) {}
        
        void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override
        {
            writer.StartObject();
            {
                WriteToJsonBase(writer);

                writer.SetFormatOptions(rapidjson::kFormatSingleLineArray);
                writer.String("Value");
                writer.StartArray();
                {
                    writer.Double((double)mValue[0]);
                    writer.Double((double)mValue[1]);
                    writer.Double((double)mValue[2]);
                }
                writer.EndArray();
                writer.SetFormatOptions(rapidjson::kFormatDefault);
            }
            writer.EndObject();
        }

        float mValue[3];
        int8_t pad[4];
    };
    ASSERT_SIZE(rmPtfxShaderVar_Float3, 0x30);


    class rmPtfxShaderVar_Float4 : public rmPtfxShaderVar
    {
    public:
        rmPtfxShaderVar_Float4(const datResource& rsc) {}
        rmPtfxShaderVar_Float4(const datResource& rsc) : rmPtfxShaderVar(rsc) {}

        void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override
        {
            writer.StartObject();
            {
                WriteToJsonBase(writer);

                writer.SetFormatOptions(rapidjson::kFormatSingleLineArray);
                writer.String("Value");
                writer.StartArray();
                {
                    writer.Double((double)mValue[0]);
                    writer.Double((double)mValue[1]);
                    writer.Double((double)mValue[2]);
                    writer.Double((double)mValue[3]);
                }
                writer.EndArray();
                writer.SetFormatOptions(rapidjson::kFormatDefault);
            }
            writer.EndObject();
        }

        float mValue[4];
    };
    ASSERT_SIZE(rmPtfxShaderVar_Float4, 0x30);


    class rmPtfxShaderVar_Texture : public rmPtfxShaderVar
    {
    public:
        rmPtfxShaderVar_Texture(const datResource& rsc) : mTexture(rsc)
        rmPtfxShaderVar_Texture(const datResource& rsc) : rmPtfxShaderVar(rsc), mTexture(rsc)
        {
            rsc.PointerFixUp(mTextureName);
        }

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            mTexture.AddToLayout(layout, depth);
            layout.AddObject(mTextureName, RSC5Layout::eBlockType::VIRTUAL, strlen(mTextureName) + 1);
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            mTexture.SerializePtrs(layout, rsc, depth);
            layout.SerializePtr(mTextureName, strlen(mTextureName) + 1);
        }

        void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override
        {
            writer.StartObject();
            {
                WriteToJsonBase(writer);

                writer.String("Value");
                writer.String(mTextureName);
            }
            writer.EndObject();
        }

        datRef<grcTexturePC> mTexture;
        char* mTextureName;
        int8_t pad[8];
    };
    ASSERT_SIZE(rmPtfxShaderVar_Texture, 0x30);


    class rmPtfxShaderVar_Keyframe : public rmPtfxShaderVar
    {
    public:
        rmPtfxShaderVar_Keyframe(const datResource& rsc) : mKeyframe(rsc) {}
        rmPtfxShaderVar_Keyframe(const datResource& rsc) : rmPtfxShaderVar(rsc), mKeyframe(rsc) {}

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            mKeyframe.AddToLayout(layout, depth);
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            mKeyframe.SerializePtrs(layout, rsc, depth);
        }

        void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override
        {
            writer.StartObject();
            {
                WriteToJsonBase(writer);

                writer.String("Value");
                mKeyframe.WriteToJson(writer);
            }
            writer.EndObject();
        }

        int8_t field_20[8];
        rmPtfxKeyframe mKeyframe;
    };
    ASSERT_SIZE(rmPtfxShaderVar_Keyframe, 0x50);
}