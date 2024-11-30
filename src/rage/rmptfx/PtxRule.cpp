#include "PtxRule.h"

namespace rage
{
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

    void ptxRule::WriteToJsonBase(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.String("Version");
        writer.Uint(100);

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
            writer.Int(mRenderState.CullMode);

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
}