#include "rapidjson/include/document.h"
#include "rapidjson/include/prettywriter.h"
#include "../../AString.h"
#include "../StringHash.h"
#include "PtxList.h"

#include <unordered_map>

namespace rage
{
    void PtxList::SaveToJson(const std::filesystem::path& filePath)
    {
        std::filesystem::path texturesDir = filePath.stem().concat("_textures\\");
        std::filesystem::path drawablesDir = filePath.stem().concat("_drawables\\");

        std::filesystem::path rulesFilePath = filePath.stem().concat("_ptxRules").replace_extension("json");
        std::filesystem::path emitRulesFilePath = filePath.stem().concat("_ptxEmitRules").replace_extension("json");
        std::filesystem::path effectRulesFilePath = filePath.stem().concat("_ptxEffectRules").replace_extension("json");

        std::unordered_map<uint32_t, AString> drawableNames;

        {
            rapidjson::StringBuffer stringBuffer;
            rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(stringBuffer);

            writer.StartObject();
            {
                writer.String("Type");
                writer.String("PtxList");

                writer.String("Version");
                writer.Uint(100);

                writer.String("Textures");
                writer.String(texturesDir.string().c_str());
                texturesDir = filePath.parent_path() / texturesDir;
                std::filesystem::create_directories(texturesDir);

                writer.String("Drawables");
                writer.String(drawablesDir.string().c_str());
                drawablesDir = filePath.parent_path() / drawablesDir;
                std::filesystem::create_directories(drawablesDir);

                writer.String("Rules");
                writer.String(rulesFilePath.string().c_str());
                rulesFilePath = filePath.parent_path() / rulesFilePath;

                writer.String("EmitRules");
                writer.String(emitRulesFilePath.string().c_str());
                emitRulesFilePath = filePath.parent_path() / emitRulesFilePath;

                writer.String("EffectRules");
                writer.String(effectRulesFilePath.string().c_str());
                effectRulesFilePath = filePath.parent_path() / effectRulesFilePath;
            }
            writer.EndObject();

            std::filesystem::path outFilePath = filePath;
            outFilePath.replace_filename(filePath.stem().concat("_ptxList")).replace_extension("json");

            std::ofstream file(outFilePath);
            if(!file.is_open())
            {
                Log::Error("Unable to open file \"%s\"", outFilePath.string().c_str());
                return;
            }

            file.write(stringBuffer.GetString(), (std::streamsize)stringBuffer.GetSize());
        }

        if(mTextures.Get())
        {
            texturesDir.concat("name");
            for(uint16_t i = 0; i < mTextures->mEntries.GetCount(); i++)
            {
                grcTexturePC& texture = *mTextures->mEntries[i];
                DDS_HEADER header = texture.GenerateDDSHeader();

                texturesDir.replace_filename(texture.mName).concat(".dds");
                std::ofstream textureFile(texturesDir, std::ios::binary);
                textureFile.write((char*)&DDS_MAGIC, sizeof(DWORD));
                textureFile.write((char*)&header, sizeof(DDS_HEADER));
                textureFile.write((char*)texture.mPixelData, texture.GetSize());
            }
        }

        if(mRules.Get())
        {
            rapidjson::StringBuffer stringBuffer;
            rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(stringBuffer);

            writer.StartArray();
            for(uint16_t i = 0; i < mRules->mEntries.GetCount(); i++)
            {
                ptxRule& rule = *mRules->mEntries[i];

                rule.WriteToJson(writer);

                if(strcmp(rule.mClassName, "ptxmodel") == 0)
                {
                    ptxModel& modelRule = (ptxModel&)rule;
                    for(uint16_t i = 0; i < modelRule.mDrawables.GetCount(); i++)
                    {
                        drawableNames[rage::atStringHash(modelRule.mDrawables[i].mName)] = modelRule.mDrawables[i].mName;
                    }
                }
            }
            writer.EndArray();

            std::ofstream file(rulesFilePath);
            if(!file.is_open())
            {
                Log::Error("Unable to open file \"%s\"", rulesFilePath.string().c_str());
                return;
            }

            file.write(stringBuffer.GetString(), (std::streamsize)stringBuffer.GetSize());
        }

        //save drawables after rules so we have names for them
        if(mDrawables.Get())
        {
            drawablesDir.concat("name");
            
            for(uint16_t i = 0; i < mDrawables->mEntries.GetCount(); i++)
            {
                gtaDrawable drawable;
                memcpy(&drawable, mDrawables->mEntries[i].Get(), sizeof(rmcDrawable));
                
                //replace textures with references because the textures point to textures in PtxList not grmShaderGroup
                std::vector<std::unique_ptr<grcTextureReference>> textureRefs;
                textureRefs.reserve(50);
                if(drawable.mShaderGroup.Get())
                {
                    auto& shaders = drawable.mShaderGroup->mShaders;
                    for(uint16_t j = 0; j < shaders.GetCount(); j++)
                    {
                        if(shaders[j].Get())
                        {
                            auto& instanceData = shaders[j]->mInstanceData;
                            for(uint32_t k = 0; k < instanceData.mCount; k++)
                            {
                                if(!instanceData.mEntriesCounts[k])
                                {
                                    if(instanceData.mEntries[k].AsTexture->mResourceType == grcTexture::eType::STANDARD)
                                    {
                                        const char* name = instanceData.mEntries[k].AsTexture->mName;
                                        instanceData.mEntries[k].AsTextureRef = textureRefs.emplace_back(std::make_unique<grcTextureReference>(name)).get();
                                    }
                                }
                            }
                        }
                    }
                }

                drawablesDir.replace_filename(drawableNames[mDrawables->mCodes[i]].Get()).concat(".wdr");
                RSC5Layout layout;
                layout.Save(drawable, drawablesDir, 110);
            }
        }

        if(mEmitRules.Get())
        {
            rapidjson::StringBuffer stringBuffer;
            rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(stringBuffer);

            writer.StartArray();
            for(uint16_t i = 0; i < mEmitRules->mEntries.GetCount(); i++)
            {
                mEmitRules->mEntries[i]->WriteToJson(writer);
            }
            writer.EndArray();

            std::ofstream file(emitRulesFilePath);
            if(!file.is_open())
            {
                Log::Error("Unable to open file \"%s\"", emitRulesFilePath.string().c_str());
                return;
            }

            file.write(stringBuffer.GetString(), (std::streamsize)stringBuffer.GetSize());
        }

        if(mEffectRules.Get())
        {
            rapidjson::StringBuffer stringBuffer;
            rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(stringBuffer);

            writer.StartArray();
            for(uint16_t i = 0; i < mEffectRules->mEntries.GetCount(); i++)
            {
                mEffectRules->mEntries[i]->WriteToJson(writer);
            }
            writer.EndArray();

            std::ofstream file(effectRulesFilePath);
            if(!file.is_open())
            {
                Log::Error("Unable to open file \"%s\"", effectRulesFilePath.string().c_str());
                return;
            }

            file.write(stringBuffer.GetString(), (std::streamsize)stringBuffer.GetSize());
        }
    }
}