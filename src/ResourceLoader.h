#pragma once
#include "rage/Resource.h"

namespace ResourceLoader
{
    bool Load(std::filesystem::path filePath, uint32_t expectedVersion, rage::datResource* rscOut, rage::datResourceInfo* infoOut = nullptr);
}