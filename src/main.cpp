#include "rage/Dictionary.h"
#include "rage/Texture.h"
#include "rage/rmptfx/PtxList.h"
#include "ResourceLoader.h"
#include "Log.h"

#include <fstream>

void LoadTXD(std::filesystem::path filePath);
void LoadDrawable(std::filesystem::path filePath);
void LoadParticleLibrary(std::filesystem::path filePath);

int main()
{
    //LoadTXD("test-files/policedb.wtd");
    LoadDrawable("test-files/amb_burg.wdr");
    //LoadParticleLibrary("test-files/gta_core.wpfl");

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

void LoadDrawable(std::filesystem::path filePath)
{
    std::unique_ptr<rage::datResource> drsc = std::make_unique<rage::datResource>(filePath.string().c_str());
    if(!ResourceLoader::Load(filePath, 110, drsc.get()))
    {
        return;
    }

    auto& drwbl = *(gtaDrawable*)drsc->Map->Chunks->DestAddr.get();
    drwbl.Place(&drwbl, *drsc);

    RSC5Layout layout;
    layout.Save(drwbl, filePath.parent_path() / "drawable.wdr", 110);
}

void LoadParticleLibrary(std::filesystem::path filePath)
{
    std::unique_ptr<rage::datResource> rsc = std::make_unique<rage::datResource>(filePath.string().c_str());
    if(!ResourceLoader::Load(filePath, 36, rsc.get()))
    {
        return;
    }

    auto& ptxlist = *(rage::PtxList*)rsc->Map->Chunks->DestAddr.get();
    ptxlist.Place(&ptxlist, *rsc);

    RSC5Layout layout;
    //layout.Save(ptxlist, filePath.parent_path() / "ptxlist.wpfl", 36);
}