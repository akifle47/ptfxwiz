#include "PtxRule.h"

namespace rage
{
    ptxRule_obj1::ptxRule_obj1()
    {
        field_0 = 1.0f;
        field_4 = 1.0f;
        field_8 = 1.0f;
        field_10 = 1.0f;
        field_14 = 1.0f;
        field_18 = -1;
        field_1C = 100.0f;
        field_20 = 0x20u;
        field_30 = 1.0f;
        field_34 = 1.0f;
        field_38 = 1.0f;
        field_40 = 1.0f;
        field_44 = 1.0f;
        field_48 = -1;
        field_4C = 100.0f;
        field_50 = 0x20;

        field_64 = 0;
        field_68 = -1.0f;
        field_6C = 0;
        
        memset(&field_C[0], 0, 4);
        memset(&field_24[0], 0, 12);
        memset(&field_3C[0], 0, 4);
        memset(&field_54[0], 0, 12);
        field_6D[0] = field_6D[1] = 0;
        field_6F = 0;
    }


    ptxRule::ptxRule(const char* type) : field_20(), field_90(), field_124()
    {
        memset(&field_4[0], 0, 8);
        memset(&field_134[0], 0, 9);

        field_8 = 0;
        field_C = 4.1999998f;
        field_10 = 0;
        field_1C = 0;
        field_118 = 0.0f;
        field_114 = 0.0f;

        field_100 = 2;
        field_104 = 0;
        field_108 = 0.0f;
        field_10C = 1;
        field_110 = 0;
        field_111 = 1;
        field_112 = 1;
        field_11C = 0;
        field_120 = 0;
        field_132 = 100;
        field_133 = 0;

        snprintf(mType, 10, "%s", type);

        field_20.field_68 = 0.0f;
        field_90.field_68 = 1.0f;
    }

    void ptxRule::Place(void* that, const datResource& rsc)
    {
        if(!strcmp(mType, "ptxsprite"))
        {
            new(that) ptxSprite(rsc);
        }
        else if(!strcmp(mType, "ptxmodel"))
        {
            //todo: new(that) ptxModel(rsc);
        }
        else
        {
            Log::Error("Invalid ptxRule type - %s", mType);
        }
    }
}