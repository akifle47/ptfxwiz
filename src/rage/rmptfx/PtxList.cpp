#include "rapidjson/include/document.h"
#include "rapidjson/include/prettywriter.h"
#include "../../AString.h"
#include "../StringHash.h"
#include "PtxList.h"
#include "../../ResourceLoader.h"

#include <unordered_map>

//why
#undef GetObject

namespace rage
{
    static std::optional<rapidjson::Document> ReadJsonFile(const std::filesystem::path filePath)
    {
        std::ifstream file(filePath);

        if(file.is_open())
        {
            std::vector<char> fileData((size_t)std::filesystem::file_size(filePath));
            file.read(fileData.data(), fileData.size());

            rapidjson::Document document;
            if(document.Parse(fileData.data()).HasParseError())
            {
                Log::Error("Error parsing \"%s\". %d", filePath.string().c_str(), (uint32_t)document.GetParseError());
                return {};
            }

            return document;
        }
        else
        {
            Log::Error("Unable to open file \"%s\".", filePath.string().c_str());
            return {};
        }
    }

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
            outFilePath.replace_extension("json");

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
            Log::Info("Saving textures...");

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
            Log::Info("Saving rules...");

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
            Log::Info("Saving drawables...");

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
                        if(!shaders[j].Get())
                            continue;

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

                drawablesDir.replace_filename(drawableNames[mDrawables->mCodes[i]].Get()).concat(".wdr");
                RSC5Layout layout;
                layout.Save(drawable, drawablesDir, 110);
            }
        }

        if(mEmitRules.Get())
        {
            Log::Info("Saving emit rules...");

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
            Log::Info("Saving effect rules...");

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

    bool PtxList::LoadFromJson(const std::filesystem::path& filePath)
    {
        auto documentOpt = ReadJsonFile(filePath);
        if(!documentOpt.has_value())
            return false;

        rapidjson::Document& document = documentOpt.value();

        if(!document.IsObject())
        {
            Log::Error("Root of ptxList must be an object.");
            return false;
        }

        if(!document.HasMember("Type") || strcmp("PtxList", document["Type"].GetString()) != 0)
            return false;


        if(document.HasMember("Textures"))
        {
            Log::Info("Loading textures...");
            std::filesystem::path texturesDir = filePath.parent_path() / document["Textures"].GetString();
            LoadTextures(texturesDir);
        }

        if(document.HasMember("Drawables"))
        {
            Log::Info("Loading drawables...");
            std::filesystem::path drawablesDir = filePath.parent_path() / document["Drawables"].GetString();
            LoadDrawables(drawablesDir);
        }

        if(document.HasMember("Rules"))
        {
            Log::Info("Loading rules...");

            auto rulesDocumentOpt = ReadJsonFile(filePath.parent_path() / document["Rules"].GetString());
            if(rulesDocumentOpt.has_value())
            {
                rapidjson::Document& rulesDocument = rulesDocumentOpt.value();
                if(rulesDocument.IsArray())
                {
                    auto rulesArray = rulesDocument.GetArray();
                    mRules = {new pgDictionary<ptxRule>((uint16_t)rulesArray.Size())};
                    for(const auto& ruleValue : rulesArray)
                    {
                        if(ruleValue.IsObject())
                        {
                            auto obj = ruleValue.GetObject();
                            
                            const char* name = obj.HasMember("Name") ? obj["Name"].GetString() : "null";
                            const char* type = obj.HasMember("Type") ? obj["Type"].GetString() : "null";
                            ptxRule* rule = nullptr;

                            if(stricmp("ptxsprite", type) == 0)
                            {
                                rule = new ptxSprite();
                            }
                            else if(stricmp("ptxmodel", type) == 0)
                            {
                                rule = new ptxModel();
                            }
                            else
                            {
                                Log::Info("Invalid rule type \"%s\".", type);
                                continue;
                            }

                            rule->LoadFromJson(obj);

                            if(stricmp("ptxsprite", type) == 0)
                            {
                                ptxSprite* sprite = (ptxSprite*)rule;
                                sprite->mShader.AssignTextureVariables(*mTextures);
                            }
                            else
                            {
                                ptxModel* model = (ptxModel*)rule;
                                model->AssignDrawables(*mDrawables);
                            }

                            mRules->AddEntry(name, rule);
                        }
                    }
                }
            }
        }

        if(document.HasMember("EmitRules"))
        {
            Log::Info("Loading emit rules...");

            auto emitRulesDocumentOpt = ReadJsonFile(filePath.parent_path() / document["EmitRules"].GetString());
            if(emitRulesDocumentOpt.has_value())
            {
                rapidjson::Document& emitRulesDocument = emitRulesDocumentOpt.value();
                if(emitRulesDocument.IsArray())
                {
                    auto emitRulesArray = emitRulesDocument.GetArray();
                    mEmitRules = {new pgDictionary<ptxEmitRule>((uint16_t)emitRulesArray.Size())};
                    for(const auto& emitRuleValue : emitRulesArray)
                    {
                        if(emitRuleValue.IsObject())
                        {
                            auto obj = emitRuleValue.GetObject();
                            const char* name = obj.HasMember("Name") ? obj["Name"].GetString() : "null";

                            ptxEmitRule* emitRule = new ptxEmitRule();
                            emitRule->LoadFromJson(obj);
                            mEmitRules->AddEntry(name, emitRule);
                        }
                    }
                }
            }
        }

        if(document.HasMember("EffectRules"))
        {
            Log::Info("Loading effect rules...");

            auto effectRulesDocumentOpt = ReadJsonFile(filePath.parent_path() / document["EffectRules"].GetString());
            if(effectRulesDocumentOpt.has_value())
            {
                rapidjson::Document& effectRulesDocument = effectRulesDocumentOpt.value();
                if(effectRulesDocument.IsArray())
                {
                    auto effectRulesArray = effectRulesDocument.GetArray();
                    mEffectRules = {new pgDictionary<ptxEffectRule>((uint16_t)effectRulesArray.Size())};
                    for(const auto& effectRuleValue : effectRulesArray)
                    {
                        if(effectRuleValue.IsObject())
                        {
                            auto obj = effectRuleValue.GetObject();
                            const char* name = obj.HasMember("Name") ? obj["Name"].GetString() : "null";

                            ptxEffectRuleStd* effectRule = new ptxEffectRuleStd();
                            effectRule->LoadFromJson(obj);

                            auto& events = effectRule->mTimeline.mEvents;
                            for(uint16_t i = 0; i < events.GetCount(); i++)
                            {
                                if(events[i]->mType == ptxEvent::eEventType::EMITTER)
                                {
                                    ptxEventEmitter& emitter = (ptxEventEmitter&)*events[i];
                                    emitter.mEmitRule = mEmitRules->Find(rage::atStringHash(emitter.mEmmiterRuleName));
                                    emitter.mRule = mRules->Find(rage::atStringHash(emitter.mPtxRuleName));
                                }
                            }

                            mEffectRules->AddEntry(name, effectRule);
                        }
                    }
                }
            }
        }

        if(mRules.Get())
        {
            for(uint16_t i = 0; i < mRules->mCodes.GetCount(); i++)
            {
                if(mRules->mEntries[i]->mSpawnEffectA.mEffectRule.Get())
                {
                    char* effectRuleNameA = (char*)mRules->mEntries[i]->mSpawnEffectA.mEffectRule.Get();
                    mRules->mEntries[i]->mSpawnEffectA.mEffectRule = {mEffectRules->Find(rage::atStringHash(effectRuleNameA))};

                    delete[] effectRuleNameA;
                }

                if(mRules->mEntries[i]->mSpawnEffectB.mEffectRule.Get())
                {
                    char* effectRuleNameB = (char*)mRules->mEntries[i]->mSpawnEffectB.mEffectRule.Get();
                    mRules->mEntries[i]->mSpawnEffectB.mEffectRule = {mEffectRules->Find(rage::atStringHash(effectRuleNameB))};

                    delete[] effectRuleNameB;
                }
            }
        }
    }

    void PtxList::LoadTextures(std::filesystem::path& texturesDir)
    {
        std::vector<std::filesystem::path> texturePaths;
        for(auto& dirEntry : std::filesystem::recursive_directory_iterator(texturesDir))
        {
            if(dirEntry.path().extension() == ".dds")
                texturePaths.emplace_back(dirEntry.path());
        }

        mTextures = {new pgDictionary<grcTexturePC>((uint16_t)texturePaths.size())};
        std::vector<uint8_t> pixelData(512 * 512 * 7);
        for(auto& texturePath : texturePaths)
        {
            char name[256];
            ZeroMemory(name, 256);
            strncpy(name, texturePath.stem().string().c_str(), 256);

            std::ifstream file(texturePath, std::ios::binary);
            if(!file.is_open())
                continue;

            DDS_HEADER header{};
            size_t textureSize = (size_t)std::filesystem::file_size(texturePath) - (sizeof(DDS_MAGIC) + sizeof(DDS_HEADER));
            if(pixelData.capacity() < textureSize)
                pixelData.reserve(textureSize);
            file.seekg(sizeof(DDS_MAGIC));
            file.read((char*)&header, sizeof(DDS_HEADER));
            file.read((char*)pixelData.data(), (std::streamsize)textureSize);

            grcTexturePC* texture = new grcTexturePC(name);
            texture->LoadFromDDS(header, pixelData.data());
            mTextures->AddEntry(name, texture);

            pixelData.clear();
        }
    }

    void PtxList::LoadDrawables(std::filesystem::path& drawablesDir)
    {
        std::vector<std::filesystem::path> drawablePaths;
        for(auto& dirEntry : std::filesystem::recursive_directory_iterator(drawablesDir))
        {
            if(dirEntry.path().extension() == ".wdr")
                drawablePaths.emplace_back(dirEntry.path());
        }

        mDrawables = {new pgDictionary<rmcDrawable>((uint16_t)drawablePaths.size())};
        for(auto& drawablePath : drawablePaths)
        {
            char name[256];
            ZeroMemory(name, 256);
            strncpy(name, drawablePath.stem().string().c_str(), 256);

            datResource rsc {name};
            if(!ResourceLoader::Load(drawablePath, 110, &rsc))
                continue;

            rmcDrawable& drawable = *(rmcDrawable*)rsc.Map->Chunks->DestAddr;
            drawable.Place(&drawable, rsc);

            //convert texture references
            auto& shaderGroup = drawable.mShaderGroup;
            if(shaderGroup.Get())
            {
                auto& shaders = drawable.mShaderGroup->mShaders;
                for(uint16_t j = 0; j < shaders.GetCount(); j++)
                {
                    if(!shaders[j].Get())
                        continue;

                    auto& instanceData = shaders[j]->mInstanceData;
                    for(uint32_t k = 0; k < instanceData.mCount; k++)
                    {
                        if(!instanceData.mEntriesCounts[k])
                        {
                            const char* name = instanceData.mEntries[k].AsTextureRef->mName;
                            grcTexturePC* texture = mTextures->Find(rage::atStringHash(name));
                            if(texture)
                            {
                                instanceData.mEntries[k].AsTexture = texture;
                            }
                        }
                    }
                }
            }

            mDrawables->AddEntry(name, &drawable);
        }
    }
}