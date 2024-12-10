#include "JsonHelpers.h"
#include "PtxRule.h"

#undef GetObject

namespace rage
{
    ptxRule::ptxRule(const char* className) : field_8(0), mFileVersion(4.3f), field_10(0), field_14{0, 0, 0, 0, 0, 0, 0, 0}, field_1C(0),
                                              mSpawnEffectA(), mSpawnEffectB(), mRenderState(), mPhysicalRange(0.0f), mStopVelocity(0.0f), mFlags(0),
                                              field_120(0), mName(nullptr), mClassName{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                              mPercentPhysical(100), mPercentKill(0), field_134{0, 0, 0, 0, 0, 0, 0, 0, 0}
    {
        strncpy(mClassName, className, 10);
    }

    void ptxRule::AddToLayout(RSC5Layout& layout, uint32_t depth)
    {
        mSpawnEffectA.AddToLayout(layout, depth);
        mSpawnEffectB.AddToLayout(layout, depth);

        layout.AddObject(mName, RSC5Layout::eBlockType::VIRTUAL, strlen(mName) + 1);

        if(!strcmp(mClassName, "ptxsprite"))
        {
            ((ptxSprite*)this)->AddToLayout(layout, depth);
        }
        else if(!strcmp(mClassName, "ptxmodel"))
        {
            ((ptxModel*)this)->AddToLayout(layout, depth);
        }
        else
        {
            Log::Error("Invalid ptxRule type - %s", mClassName);
        }
    }

    void ptxRule::SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
    {
        mSpawnEffectA.SerializePtrs(layout, rsc, depth);
        mSpawnEffectB.SerializePtrs(layout, rsc, depth);

        layout.SerializePtr(mName, strlen(mName) + 1);

        if(!strcmp(mClassName, "ptxsprite"))
        {
            ((ptxSprite*)this)->SerializePtrs(layout, rsc, depth);
        }
        else if(!strcmp(mClassName, "ptxmodel"))
        {
            ((ptxModel*)this)->SerializePtrs(layout, rsc, depth);
        }
        else
        {
            Log::Error("Invalid ptxRule type - %s", mClassName);
        }
    }

    void ptxRule::Place(void* that, const datResource& rsc)
    {
        if(!strcmp(mClassName, "ptxsprite"))
        {
            new(that) ptxSprite(rsc);
        }
        else if(!strcmp(mClassName, "ptxmodel"))
        {
            new(that) ptxModel(rsc);
        }
        else
        {
            Log::Error("Invalid ptxRule type - %s", mClassName);
        }
    }

    const char* rage::ptxRule::PtxCullModeToString(ePtxCullMode cull)
    {
        static const char* lut[] {"NONE", "CLOCKWISE", "COUNTERCLOCKWISE"};
        if((uint32_t)cull >= (uint32_t)ePtxCullMode::COUNT)
            return "INVALID";
        else
            return lut[(uint32_t)cull];
    }

    ePtxCullMode rage::ptxRule::StringToPtxCullMode(const char* name)
    {
        static const char* lut[] {"NONE", "CLOCKWISE", "COUNTERCLOCKWISE"};
        for(size_t i = 0; i < (size_t)ePtxCullMode::COUNT; i++)
        {
            if(stricmp(name, lut[i]) == 0)
                return (ePtxCullMode)i;
        }

        return ePtxCullMode::COUNT;
    }

    void ptxRule::WriteToJsonBase(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.String("Version");
        writer.Uint(100);

        writer.String("field_14");
        writer.StartArray();
        for(size_t i = 0; i < std::size(field_14); i++)
        {
            writer.Int(field_14[i]);
        }
        writer.EndArray();

        writer.String("Name");
        writer.String(mName);

        writer.String("Type");
        writer.String(mClassName);

        writer.String("field_10");
        writer.Int(field_10);

        writer.String("field_1C");
        writer.Int(field_1C);

        writer.String("SpawnEffectA");
        mSpawnEffectA.WriteToJson(writer);
        writer.String("SpawnEffectB");
        mSpawnEffectB.WriteToJson(writer);

        writer.String("RenderState");
        writer.StartObject();
        {
            writer.String("CullMode");
            writer.String(PtxCullModeToString(mRenderState.CullMode));

            writer.String("BlendSet");
            writer.Int(mRenderState.BlendSet);

            writer.String("DepthBias");
            writer.Double((double)mRenderState.DepthBias);

            writer.String("LightingMode");
            writer.Int(mRenderState.LightingMode);

            writer.String("DepthWrite");
            writer.Bool(mRenderState.DepthWrite);

            writer.String("DepthTest");
            writer.Bool(mRenderState.DepthTest);

            writer.String("AlphaBlend");
            writer.Bool(mRenderState.AlphaBlend);

            writer.String("field_13");
            writer.Int(mRenderState.field_13);
        }
        writer.EndObject();

        writer.String("PhysicalRange");
        writer.Double((double)mPhysicalRange);

        writer.String("StopVelocity");
        writer.Double((double)mStopVelocity);

        writer.String("Flags");
        writer.Uint(mFlags);

        writer.String("PercentPhysical");
        writer.Uint(mPercentPhysical);

        writer.String("PercentKill");
        writer.Uint(mPercentKill);
    }

    void ptxRule::LoadFromJsonBase(rapidjson::GenericObject<true, rapidjson::Value>& object)
    {
        if(object.HasMember("field_14"))
        {
            auto field_14Array = object["field_14"].GetArray();
            uint32_t index = 0;
            for(auto& field_14Value : field_14Array)
            {
                field_14[index] = (int8_t)field_14Value.GetInt();
                index++;
            }
        }

        mName = strdup(object["Name"].GetString());

        field_10 = object["field_10"].GetInt();
        field_1C = object["field_1C"].GetInt();

        JsonHelpers::LoadMemberObject(mSpawnEffectA, object, "SpawnEffectA");
        JsonHelpers::LoadMemberObject(mSpawnEffectB, object, "SpawnEffectB");

        if(object.HasMember("RenderState"))
        {
            if(object["RenderState"].IsObject())
            {
                auto renderStateObject = object["RenderState"].GetObject();

                mRenderState.CullMode = StringToPtxCullMode(renderStateObject["CullMode"].GetString());
                mRenderState.BlendSet = renderStateObject["BlendSet"].GetInt();
                mRenderState.DepthBias = renderStateObject["DepthBias"].GetFloat();
                mRenderState.LightingMode = renderStateObject["LightingMode"].GetUint();
                mRenderState.DepthWrite = renderStateObject["DepthWrite"].GetBool();
                mRenderState.DepthTest = renderStateObject["DepthTest"].GetBool();
                mRenderState.AlphaBlend = renderStateObject["AlphaBlend"].GetBool();
                mRenderState.field_13 = renderStateObject["field_13"].GetInt();
            }
        }

        mPhysicalRange = object["PhysicalRange"].GetFloat();
        mStopVelocity = object["StopVelocity"].GetFloat();
        mFlags = object["Flags"].GetUint();
        mPercentPhysical = object["PercentPhysical"].GetUint();
        mPercentKill = object["PercentKill"].GetUint();
    }


    ptxSprite::ptxSprite() : ptxRule("ptxsprite"), field_140(0.0f), field_144(0.0f), field_148(0.0f), field_14C(0), field_150(0.0f), field_154(0.0f),
                             field_158(0.0f), field_15C(0), mAxisAlligned{.x = 0.0f, .y = 0.0f, .z = 0.0f}, field_16C(0), mStartTexFrameIndex(0), 
                             mEndTexFrameIndex(0), mEndTexAnimFrame(-1), mNumTextureTilesX(1), mNumTextureTilesY(1), field_184(nullptr), mProps(),
                             mShader(), mTrimCornersAmmount(0.0f), mTrimCorners(false), field_69D{0, 0}, field_69F(0)
    {}


    void ptxSprite::WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();
        {
            WriteToJsonBase(writer);

            writer.String("field_140");
            writer.Double((double)field_140);

            writer.String("field_144");
            writer.Double((double)field_144);

            writer.String("field_148");
            writer.Double((double)field_148);

            writer.String("field_14C");
            writer.Int(field_14C);

            writer.String("field_150");
            writer.Double((double)field_150);

            writer.String("field_154");
            writer.Double((double)field_154);

            writer.String("field_158");
            writer.Double((double)field_158);

            writer.String("field_15C");
            writer.Int(field_15C);

            writer.SetFormatOptions(rapidjson::kFormatSingleLineArray);
            writer.String("AxisAlligned");
            writer.StartArray();
            {
                writer.Double((double)mAxisAlligned.x);
                writer.Double((double)mAxisAlligned.y);
                writer.Double((double)mAxisAlligned.z);
            }
            writer.EndArray();
            writer.SetFormatOptions(rapidjson::kFormatDefault);

            writer.String("field_16C");
            writer.Int(field_15C);

            writer.String("StartTexFrameIndex");
            writer.Int(mStartTexFrameIndex);
            writer.String("EndTexFrameIndex");
            writer.Int(mEndTexFrameIndex);
            writer.String("EndTexAnimFrame");
            writer.Int(mEndTexAnimFrame);

            writer.String("NumTextureTilesX");
            writer.Int(mNumTextureTilesX);
            writer.String("NumTextureTilesY");
            writer.Int(mNumTextureTilesY);

            writer.String("Props");
            mProps.WriteToJson(writer);

            writer.String("Shader");
            mShader.WriteToJson(writer);
        }
        writer.EndObject();
    }

    void ptxSprite::LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object)
    {
        LoadFromJsonBase(object);

        field_140 = object["field_140"].GetFloat();
        field_144 = object["field_144"].GetFloat();
        field_148 = object["field_148"].GetFloat();

        field_14C = object["field_14C"].GetInt();

        field_150 = object["field_150"].GetFloat();
        field_154 = object["field_154"].GetFloat();
        field_158 = object["field_158"].GetFloat();

        field_15C = object["field_15C"].GetInt();

        mAxisAlligned.x = object["AxisAlligned"].GetArray()[0].GetFloat();
        mAxisAlligned.y = object["AxisAlligned"].GetArray()[1].GetFloat();
        mAxisAlligned.z = object["AxisAlligned"].GetArray()[2].GetFloat();

        field_15C = object["field_16C"].GetInt();

        mStartTexFrameIndex = object["StartTexFrameIndex"].GetInt();
        mEndTexFrameIndex = object["EndTexFrameIndex"].GetInt();
        mEndTexAnimFrame = object["EndTexAnimFrame"].GetInt();

        mNumTextureTilesX = object["NumTextureTilesX"].GetInt();
        mNumTextureTilesY = object["NumTextureTilesY"].GetInt();

        JsonHelpers::LoadMemberObject(mProps, object, "Props");
        JsonHelpers::LoadMemberObject(mShader, object, "Shader");
    }


    ptxModel::ptxModel() : ptxRule("ptxmodel"), mRotation{.x = 0.0f, .y = 0.0f, .z = 0.0f}, field_14C{0, 0, 0, 0}, mRotationVar{.x = 0.0f, .y = 0.0f, .z = 0.0f},
                           field_15C{0, 0, 0, 0}, mRotationSpeedVar{.x = 0.0f, .y = 0.0f}, mDrawables(), field_170(nullptr), mProps(), field_698{0, 0, 0, 0, 0, 0, 0}, field_69F(0)
    {}

    void ptxModel::AssignDrawables(pgDictionary<rmcDrawable>& drawables)
    {
        for(uint16_t i = 0; i < mDrawables.GetCount(); i++)
        {
            mDrawables[i].mDrawable = {drawables.Find(rage::atStringHash(mDrawables[i].mName))};
        }
    }

    void ptxModel::WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();
        {
            WriteToJsonBase(writer);

            writer.SetFormatOptions(rapidjson::kFormatSingleLineArray);
            writer.String("Rotation");
            writer.StartArray();
            {
                writer.Double((double)mRotation.x);
                writer.Double((double)mRotation.y);
                writer.Double((double)mRotation.z);
            }
            writer.EndArray();

            writer.String("RotationVar");
            writer.StartArray();
            {
                writer.Double((double)mRotationVar.x);
                writer.Double((double)mRotationVar.y);
                writer.Double((double)mRotationVar.z);
            }
            writer.EndArray();

            writer.String("RotationSpeedVar");
            writer.StartArray();
            {
                writer.Double((double)mRotationSpeedVar.x);
                writer.Double((double)mRotationSpeedVar.y);
            }
            writer.EndArray();

            if(mDrawables.GetCount())
            {
                writer.String("Drawables");
                writer.StartArray();
                for(uint16_t i = 0; i < mDrawables.GetCount(); i++)
                {
                    writer.String(mDrawables[i].mName);
                }
                writer.EndArray();
            }

            writer.SetFormatOptions(rapidjson::kFormatDefault);

            writer.String("Props");
            mProps.WriteToJson(writer);

            writer.String("field_69F");
            writer.Int(field_69F);
        }
        writer.EndObject();
    }

    void ptxModel::LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object)
    {
        LoadFromJsonBase(object);

        mRotation.x = object["Rotation"].GetArray()[0].GetFloat();
        mRotation.y = object["Rotation"].GetArray()[1].GetFloat();
        mRotation.z = object["Rotation"].GetArray()[2].GetFloat();

        mRotationVar.x = object["RotationVar"].GetArray()[0].GetFloat();
        mRotationVar.y = object["RotationVar"].GetArray()[1].GetFloat();
        mRotationVar.z = object["RotationVar"].GetArray()[2].GetFloat();

        mRotationSpeedVar.x = object["RotationSpeedVar"].GetArray()[0].GetFloat();
        mRotationSpeedVar.y = object["RotationSpeedVar"].GetArray()[1].GetFloat();

        if(object.HasMember("Drawables"))
        {
            auto drawablesArray = object["Drawables"].GetArray();
            mDrawables = {(uint16_t)drawablesArray.Size()};
            for(auto& drawableValue : drawablesArray)
            {
                mDrawables.Append().mName = strdup(drawableValue.GetString());
            }

            JsonHelpers::LoadMemberObject(mProps, object, "Props");
            field_69F = object["field_69F"].GetInt();
        }
    }
}