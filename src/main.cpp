#include "rage/Dictionary.h"
#include "rage/Texture.h"
#include "rage/rmptfx/PtxList.h"
#include "ResourceLoader.h"
#include "Log.h"

#include <fstream>

void LoadTXD(std::filesystem::path filePath);
void LoadParticleLibrary(std::filesystem::path filePath);

int main()
{
    //LoadTXD("test-files/policedb.wtd");
    LoadParticleLibrary("test-files/gta_core.wpfl");

    return 0;
}

void LoadTXD(std::filesystem::path filePath)
{
    std::unique_ptr<rage::datResource> rsc = std::make_unique<rage::datResource>(filePath.string().c_str());
    if(!ResourceLoader::Load(filePath, 8, rsc.get()))
    {
        return;
    }

    auto& txd = *(rage::pgDictionary<rage::grcTexturePC>*)rsc->Map->Chunks->DestAddr.get();
    txd.Place(&txd, *rsc);

    RSC5Layout layout;
    layout.Save(txd, filePath.parent_path() / "txd.wtd", 8);
}

void LoadParticleLibrary(std::filesystem::path filePath)
{
    std::unique_ptr<rage::datResource> rsc = std::make_unique<rage::datResource>(filePath.string().c_str());
    if(!ResourceLoader::Load(filePath, 36, rsc.get()))
    {
        return;
    }

    auto& ptxLib = *(rage::PtxList*)rsc->Map->Chunks->DestAddr.get();
    ptxLib.Place(&ptxLib, *rsc);

    RSC5Layout layout;
    //layout.Save(ptxLib, filePath.parent_path() / "ptxlib.wpfl", 8);
}