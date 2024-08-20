#pragma once
#include "rage/grcore/Effect.h"

#include <memory>

namespace EffectList
{
    bool PreLoad(std::filesystem::path gamePath);
    rage::grcEffect* GetEffectByHash(uint32_t hash);
    rage::grcEffect* GetEffectByName(const char* name);
}