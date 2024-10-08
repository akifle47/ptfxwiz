#include "rage/Dictionary.h"
#include "rage/grcore/Texture.h"
#include "rage/rmptfx/PtxList.h"
#include "ResourceLoader.h"
#include "EffectList.h"
#include "AString.h"

#include <fstream>

static const char* CACHE_FILE_NAME = "ptfxwiz.cache";

//returns whether it should quit
bool ProcessInput(const char* input);
void PrintHelp();

void LoadAndSaveTXD(std::filesystem::path filePathIn, std::filesystem::path filePathOut);
void LoadAndSaveDrawable(std::filesystem::path filePathIn, std::filesystem::path filePathOut);
void LoadAndSaveParticleList(std::filesystem::path filePathIn, std::filesystem::path filePathOut);

int main(int32_t argc, char** argv)
{
    Log::mBreakOnError = false;

    char gamePath[256] {'\0'};

    if(!std::filesystem::exists(CACHE_FILE_NAME))
    {
        printf("Select your game's executable or launcher.\n");

        OPENFILENAMEA ofn
        {
            .lStructSize = sizeof(OPENFILENAME),
            .lpstrFilter = "LaunchGTAIV.exe, PlayGTAIV.exe or GTAIV.exe\0LaunchGTAIV.exe;PlayGTAIV.exe;GTAIV.exe\0\0",
            .nFilterIndex = 1,
            .lpstrFile = gamePath,
            .nMaxFile = sizeof(gamePath),
            .Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR 
        };

        if(!GetOpenFileNameA(&ofn))
        {
            return -1;
        }

        std::ofstream cacheFile(CACHE_FILE_NAME);
        cacheFile << gamePath;
    }
    else
    {
        std::ifstream cacheFile(CACHE_FILE_NAME);
        if(!cacheFile.is_open())
        {
            Log::Error("Unable to open file \"%s\".", std::filesystem::absolute(CACHE_FILE_NAME).string().c_str());
            return -1;
        }
        cacheFile.getline(gamePath, 256);
    }

    EffectList::PreLoad(gamePath);

    if(argc == 1)
    {
        char input[256] {};

        while(true)
        {
            printf("> ");

            fgets(input, 256, stdin);

            if(ProcessInput(input))
            {
                return 0;
            }
        }
    }

    return 0;
}

bool ProcessInput(const char* input)
{
    if(input[0] == '\n')
        return false;

    if(memcmp(input, "-q", 2) == 0 || memcmp(input, "-quit", 5) == 0)
        return true;

    if(memcmp(input, "-h", 2) == 0 || memcmp(input, "-help", 5) == 0)
    {
        PrintHelp();
        return false;
    }

    std::vector<AString> tokens;
    tokens.reserve(4);

    for(uint32_t beg = 0; beg < strlen(input) - 1; beg++)
    {
        if(input[beg] > ' ')
        {
            uint32_t end = beg;
            while(input[end] > ' ') end++;

            tokens.emplace_back((end - beg) + 1);
            memcpy(tokens.back().Get(), &input[beg], end - beg);
            beg = end;
        }
    }

    static const char* options[] {"-t", "-txd", "-d", "-drwbl", "-p", "-ptx"};
    for(uint32_t i = 0; i < std::size(options); i++)
    {
        if(tokens[0] == options[i])
        {
            if(tokens.size() > 3)
            {
                Log::Error("Too many arguments.");
                PrintHelp();
                return false;
            }
            if(tokens.size() < 2)
            {
                Log::Error("Too few arguments.");
                PrintHelp();
                return false;
            }

            AString option(options[i]);
            const char* fileIn = tokens[1].Get();
            const char* fileOut = tokens.size() != 2 ? tokens[2].Get() : "";

            if(option == "-t" || option == "-txd")
            {
                LoadAndSaveTXD(fileIn, fileOut);
            }
            else if(option == "-d" || option == "-drwbl")
            {
                LoadAndSaveDrawable(fileIn, fileOut);
            }
            else if(option == "-p" || option == "-ptx")
            {
                LoadAndSaveParticleList(fileIn, fileOut);
            }

            return false;
        }
    }

    Log::Error("First argument must be an option.");
    PrintHelp();
    return false;
}

void PrintHelp()
{
    printf("<arguments> are required and [arguments] are optional.\n");
    printf("Usage: <option> <in_file> [out_file]\n");
    printf("    options:\n");
    printf("        -q, -quit\n");
    printf("        -h, -help    displays this message.\n");
#ifdef _DEBUG
    printf("        -t, -txd     loads and saves a texture dictionary as a resource (wtd).\n");
    printf("        -d, -drwbl   loads and saves a drawable as a resource (wdr).\n");
#endif
    //todo b
    printf("        -p, -ptx     saves <in_file> as a wpfl if it's a TODO or as a TODO if it's a wpfl. if no [out_file]\n");
    printf("                     is provided the file will be saved as in_file_out.\n");
}

void LoadAndSaveTXD(std::filesystem::path filePathIn, std::filesystem::path filePathOut)
{
    std::unique_ptr<rage::datResource> rsc = std::make_unique<rage::datResource>(filePathIn.string().c_str());
    if(!ResourceLoader::Load(filePathIn, 8, rsc.get()))
    {
        return;
    }

    auto& txd = *(rage::pgDictionary<rage::grcTexturePC>*)rsc->Map->Chunks->DestAddr.get();
    txd.Place(&txd, *rsc);
    
    if(filePathOut.empty())
    {
        filePathOut = filePathIn.parent_path() / filePathIn.stem();
        filePathOut += "_out.wtd";
    }

    RSC5Layout layout;
    layout.Save(txd, filePathOut, 8);
}

void LoadAndSaveDrawable(std::filesystem::path filePathIn, std::filesystem::path filePathOut)
{
    std::unique_ptr<rage::datResource> drsc = std::make_unique<rage::datResource>(filePathIn.string().c_str());
    if(!ResourceLoader::Load(filePathIn, 110, drsc.get()))
    {
        return;
    }

    auto& drwbl = *(gtaDrawable*)drsc->Map->Chunks->DestAddr.get();
    drwbl.Place(&drwbl, *drsc);

    if(filePathOut.empty())
    {
        filePathOut = filePathIn.parent_path() / filePathIn.stem();
        filePathOut += "_out.wdr";
    }
    
    RSC5Layout layout;
    layout.Save(drwbl, filePathOut, 110);
}

void LoadAndSaveParticleList(std::filesystem::path filePathIn, std::filesystem::path filePathOut)
{
    std::unique_ptr<rage::datResource> rsc = std::make_unique<rage::datResource>(filePathIn.string().c_str());
    if(!ResourceLoader::Load(filePathIn, 36, rsc.get()))
    {
        return;
    }

    auto& ptxlist = *(rage::PtxList*)rsc->Map->Chunks->DestAddr.get();
    ptxlist.Place(&ptxlist, *rsc);

    if(filePathOut.empty())
    {
        filePathOut = filePathIn.parent_path() / filePathIn.stem();
        filePathOut += "_out.wpfl";
    }

    RSC5Layout layout;
    //layout.Save(ptxlist, filePathOut, 36);
}