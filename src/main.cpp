#include "rapidjson/include/document.h"
#include "rapidjson/include/prettywriter.h"
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
void ProcessParticleList(std::filesystem::path filePathIn, std::filesystem::path filePathOut);

int main(int32_t argc, char** argv)
{
    Log::mBreakOnError = true;

    char gamePath[256] {'\0'};

    std::filesystem::path cachePath {argv[0]};
    cachePath.replace_filename(CACHE_FILE_NAME);
    if(!std::filesystem::exists(cachePath))
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

        std::ofstream cacheFile(cachePath);
        cacheFile << gamePath;
    }
    else
    {
        std::ifstream cacheFile(cachePath);
        if(!cacheFile.is_open())
        {
            Log::Error("Unable to open file \"%s\".", std::filesystem::absolute(cachePath).string().c_str());
            std::ignore = getc(stdin);
            return -1;
        }
        cacheFile.getline(gamePath, 256);
    }

    if(!EffectList::PreLoad(gamePath))
    {
        std::ignore = getc(stdin);
        return -1;
    }

    if(argc == 1)
    {
        PrintHelp();

        char input[256] {0};
        wchar_t inputW[256] {0};
        DWORD bytesRead = 0;

        while(true)
        {
            printf("> ");

            ZeroMemory(input, std::size(input));
            ZeroMemory(inputW, std::size(inputW));

            std::ignore = ReadConsoleW(GetStdHandle(STD_INPUT_HANDLE), inputW, 256, &bytesRead, nullptr);
            WideCharToMultiByte(CP_UTF8, 0, inputW, -1, input, std::size(input), nullptr, nullptr);

            if(ProcessInput(input))
            {
                return 0;
            }
        }
    }
    else if(argc == 2)
    {
        ProcessParticleList(argv[1], "");
        Log::Info("Done.");
        std::ignore = getc(stdin);
    }

    return 0;
}

bool ProcessInput(const char* input)
{
    if(std::iscntrl(input[0]))
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

    if(tokens.size() > 0)
    {
        const char* inFile = tokens[0].Get();
        const char* outDir = tokens.size() == 1 ? "" : tokens[1].Get();
        ProcessParticleList(inFile, outDir);
        return false;
    }

    PrintHelp();
    return false;
}

void PrintHelp()
{
    printf("<arguments> are required and [arguments] are optional.\n");
    printf("usage: [option] <in_file> [out_directory]\n");
    printf("examples: gta_core.wpfl output/\n");
    printf("          gta_core.wpfl output/\n");
    printf("or drag and drop a file on ptfxwiz.exe\n");
    printf("if no output directory is provided the file(s) will be saved in the same directory <in_file> is in.\n");
    printf("    options:\n");
    printf("        -q, -quit\n");
    printf("        -h, -help    displays this message.\n");
#ifdef _DEBUG
    printf("        -t, -txd     loads and saves a texture dictionary as a resource (wtd).\n");
    printf("        -d, -drwbl   loads and saves a drawable as a resource (wdr).\n");
    printf("        -p, -ptx     loads and saves a particle list as a resource (wpfl)\n");
#endif
}

using Txd = rage::pgDictionary<rage::grcTexturePC>;

void LoadAndSaveTXD(std::filesystem::path filePathIn, std::filesystem::path filePathOut)
{
    std::unique_ptr<rage::datResource> rsc = std::make_unique<rage::datResource>(filePathIn.string().c_str());
    if(!ResourceLoader::Load(filePathIn, 8, rsc.get()))
        return;

    Txd* txd((Txd*)rsc->Map->Chunks->DestAddr);
    txd->Place(txd, *rsc);
    
    if(filePathOut.empty())
    {
        filePathOut = filePathIn.parent_path() / filePathIn.stem();
        filePathOut += "_out.wtd";
    }

    RSC5Layout layout;
    layout.Save(*txd, filePathOut, 8);
}

void LoadAndSaveDrawable(std::filesystem::path filePathIn, std::filesystem::path filePathOut)
{
    std::unique_ptr<rage::datResource> drsc = std::make_unique<rage::datResource>(filePathIn.string().c_str());
    if(!ResourceLoader::Load(filePathIn, 110, drsc.get()))
        return;

    auto& drwbl = *(gtaDrawable*)drsc->Map->Chunks->DestAddr;
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
        return;

    auto& ptxlist = *(rage::PtxList*)rsc->Map->Chunks->DestAddr;
    ptxlist.Place(&ptxlist, *rsc);

    if(filePathOut.empty())
    {
        filePathOut = filePathIn.parent_path() / filePathIn.stem();
        filePathOut += "_out.wdr";
    }

    RSC5Layout layout;
    layout.Save(ptxlist, filePathOut, 36);
}

void ProcessParticleList(std::filesystem::path filePathIn, std::filesystem::path filePathOut)
{
    filePathIn.make_preferred();
    filePathOut.make_preferred();
    //wpfl to json
    if (filePathIn.extension() == ".wpfl")
    {
        rage::datResource rsc {filePathIn.string().c_str()};
        if (!ResourceLoader::Load(filePathIn, 36, &rsc))
            return;

        auto& ptxList = *(rage::PtxList*)rsc.Map->Chunks->DestAddr;
        ptxList.Place(&ptxList, rsc);

        if(!filePathOut.empty())
        {
            if(filePathOut.has_filename())
                filePathOut.replace_filename(filePathIn.filename());
            else
                filePathOut.concat(filePathIn.filename().string());
        }
        else
        {
            filePathOut = filePathIn;
        }

        ptxList.SaveToJson(filePathOut);
        rsc.Map->FreeAllChunks();
    }
    //json to wpfl
    else if(filePathIn.extension() == ".json")
    {
        rage::PtxList ptxList {};
        ptxList.LoadFromJson(filePathIn);

        if(!filePathOut.empty())
        {
            if(filePathOut.has_filename())
                filePathOut.replace_filename(filePathIn.filename());
            else
                filePathOut.concat(filePathIn.filename().string());
        }
        else
        {
            filePathOut = filePathIn;
        }

        RSC5Layout layout;
        layout.Save(ptxList, filePathOut, 110);
    }
}