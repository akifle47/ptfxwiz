#include "PtxRule.h"

namespace rage
{
    ptxTriggerEventScalars::ptxTriggerEventScalars()
    {
        field_0 = 1.0f;
        field_4 = 1.0f;
        field_8 = 1.0f;

        field_10 = 1.0f;
        field_14 = 1.0f;

        mColorTint = 0xFFFFFFFF;
        mZoom = 100.0f;
        mFlags = 0x20;

        memset(&field_C[0], 0, 4);
    }

    ptxTriggerEvent::ptxTriggerEvent()
    {
        mType = 0;
        mTime = -1.0f;
        mToggle1 = 0;
        
        memset(&field_24[0], 0, 12);
        memset(&field_54[0], 0, 12);
        field_6D[0] = field_6D[1] = 0;
        field_6F = 0;
    }


    ptxRule::ptxRule(const char* type) : mSpawnEffectA(), mSpawnEffectB(), mName()
    {
        memset(&field_4[0], 0, 8);
        memset(&field_134[0], 0, 9);

        field_8 = 0;
        mFileVersion = 4.1999998f;
        field_10 = 0;
        field_1C = 0;
        mStopVelocity = 0.0f;
        mPhysicalRange = 0.0f;

        mFlags = 0;
        field_120 = 0;
        mPercentPhysical = 100;
        mPercentKill = 0;

        snprintf(mClassName, 10, "%s", type);

        mSpawnEffectA.mTime = 0.0f;
        mSpawnEffectB.mTime = 1.0f;
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
}