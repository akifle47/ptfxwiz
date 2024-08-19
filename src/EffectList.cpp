#include "EffectList.h"
#include "rage/StringHash.h"
#include "AString.h"

#include <vector>

namespace EffectList
{
    static std::vector<rage::grcEffect> sEffects;

    bool PreLoad(std::filesystem::path gamePath)
    {
        if(!sEffects.empty())
            return false;

        sEffects.reserve(128);
        gamePath.remove_filename();

        std::filesystem::path preloadPath = gamePath / "common/shaders/preload.list";

        if(!std::filesystem::exists(preloadPath))
        {
            Log::Error("File \"%s\". does not exist.", preloadPath.string().c_str());
            return false;
        }

        std::ifstream preloadFile(preloadPath);
        if(!preloadFile.is_open())
        {
            Log::Error("Unable to open file \"%s\".", preloadPath.string().c_str());
            return false;
        }

        Log::Info("Loading shader preload list.");

        uint32_t fileSize = (uint32_t)std::filesystem::file_size(preloadPath);
        AString fileData(fileSize);
        preloadFile.read((char*)fileData.Get(), fileSize);
        preloadFile.close();

        std::vector<AString> effectFileNames;
        effectFileNames.reserve(128);

        char* token = strtok(fileData.Get(), "\n");
        while(token) 
        {
            if(strstr(token, ".fx"))
                effectFileNames.emplace_back(token);

            token = strtok(nullptr, "\n");
        }

        for(const auto& fileName : effectFileNames)
        {
            std::filesystem::path effectPath = gamePath / "common/shaders/win32_30_nv8" / fileName.Get();
            effectPath.replace_extension("fxc");

            sEffects.emplace_back(effectPath);
        }

        Log::Info("Loaded shader preload list.");

        return true;
    }

    const rage::grcEffect* GetEffectByHash(uint32_t hash)
    {
        for(const auto& effect : sEffects)
        {
            if(effect.GetHash() == hash)
                return &effect;
        }

        Log::Error("There is no effect with the hash \"%d\"", hash);
        return nullptr;
    }

    const rage::grcEffect* EffectList::GetEffectByName(const char* name)
    {
        uint32_t hash = rage::atStringHash(name);

        for(const auto& effect : sEffects)
        {
            if(effect.GetHash() == hash)
                return &effect;
        }

        Log::Error("There is no effect with the name \"%s\"", name);
        return nullptr;
    }
}