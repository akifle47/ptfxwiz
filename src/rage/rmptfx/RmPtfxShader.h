#pragma once
#include "../Array.h"
#include "RmPtfxShaderVar.h"
#include "../DatOwner.h"

namespace rage
{
    /*
    class rmPtfxShaderTemplate
    {
    public:
        virtual ~rmPtfxShaderTemplate()
        {
        }

        grmShaderFx* mShaderFx;
        char mName[128];
        int8_t field_88;
        atArray field_8C;
        int32_t mParamIndices[4];
        int8_t field_A4;
        int8_t field_A5;
        int8_t field_A6;
        int8_t field_A7;
    };
    ASSERT_SIZE(rmPtfxShaderTemplate, 0xA8);
    */

    class rmPtfxShader
    {
    public:
        rmPtfxShader(const datResource& rsc) : mVars(rsc), mShaderTemplate(rsc)
        {
            rsc.PointerFixUp(mName);
            rsc.PointerFixUp(mTechName);
        }

        virtual ~rmPtfxShader()
        {
            delete[] mName;
            delete[] mTechName;
        }

        char* mName;
        char* mTechName;
        //computed at runtime
        datOwner<void*/*rmPtfxShaderTemplate*/> mShaderTemplate;
        int32_t mTechIndex;
        atArray<datOwner<rmPtfxShaderVar>> mVars;
        bool field_1C;
        int8_t field_1D[3];
    };
    ASSERT_SIZE(rmPtfxShader, 0x20);
}