#include "RmPtfxShader.h"
#include "rage/StringHash.h"

#undef GetObject

namespace rage
{
    void rmPtfxShader::AssignTextureVariables(pgDictionary<grcTexturePC>& txd)
    {
        for(uint16_t i = 0; i < mVars.GetCount(); i++)
        {
            if(!mVars[i].Get() || mVars[i]->mType != rmPtfxShaderVar::eVarType::TEXTURE)
                continue;

            rmPtfxShaderVar_Texture& var = (rmPtfxShaderVar_Texture&)*mVars[i];
            var.mTexture = txd.Find(rage::atStringHash(var.mTextureName));
        }
    }

    void rmPtfxShader::WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();
        {
            writer.String("Name");
            writer.String(mName);

            writer.String("TechniqueName");
            writer.String(mTechName);

            if(mVars.GetCount())
            {
                writer.String("Variables");
                writer.StartArray();
                {
                    for(uint16_t i = 0; i < mVars.GetCount(); i++)
                    {
                        if(mVars[i].Get())
                        {
                            mVars[i]->WriteToJson(writer);
                        }
                    }
                }
                writer.EndArray();
            }

            writer.String("field_1C");
            writer.Bool(field_1C);
        }
        writer.EndObject();
    }

    void rmPtfxShader::LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object)
    {
        mName = strdup(object["Name"].GetString());
        mTechName = strdup(object["TechniqueName"].GetString());

        if(object.HasMember("Variables") && object["Variables"].IsArray())
        {
            auto varsArray = object["Variables"].GetArray();
            mVars = {(uint16_t)varsArray.Size()};
            for(auto& varValue : varsArray)
            {
                rmPtfxShaderVar::eVarType varType = rmPtfxShaderVar::StringToType(varValue["Type"].GetString());
                switch(varType)
                {
                    case rmPtfxShaderVar::eVarType::BOOL:
                        mVars.Append() = {new rmPtfxShaderVar_Bool()};
                    break;

                    case rmPtfxShaderVar::eVarType::INT:
                        mVars.Append() = {new rmPtfxShaderVar_Int()};
                    break;

                    case rmPtfxShaderVar::eVarType::FLOAT:
                        mVars.Append() = {new rmPtfxShaderVar_Float()};
                    break;

                    case rmPtfxShaderVar::eVarType::FLOAT2:
                        mVars.Append() = {new rmPtfxShaderVar_Float2()};
                    break;

                    case rmPtfxShaderVar::eVarType::FLOAT3:
                        mVars.Append() = {new rmPtfxShaderVar_Float3()};
                    break;

                    case rmPtfxShaderVar::eVarType::FLOAT4:
                        mVars.Append() = {new rmPtfxShaderVar_Float4()};
                    break;

                    case rmPtfxShaderVar::eVarType::TEXTURE:
                        mVars.Append() = {new rmPtfxShaderVar_Texture()};
                    break;

                    case rmPtfxShaderVar::eVarType::KEYFRAME:
                        mVars.Append() = {new rmPtfxShaderVar_Keyframe()};
                    break;

                    default:
                        Log::Error("Shader variable \"%s\" in shader \"%s\" uses an invalid type - %d", 
                                    varValue.HasMember("Name") ? varValue["Name"].GetString() : "null", mName ? mName : "null", (uint32_t)varType);
                }

                auto varObject = varValue.GetObject();
                mVars.Back()->LoadFromJson(varObject);
            }
        }

        field_1C = object["field_1C"].GetBool();
    }
}