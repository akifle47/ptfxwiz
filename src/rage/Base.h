#pragma once
#include "../RSC5Layout.h"

namespace rage
{
    class datBase
    {
    public:
        datBase() {};
        virtual ~datBase() {};
    };

    class pgBasePageMap
    {
    public:
        uint8_t mVirtualCount;
        uint8_t mPhysicalCount;
        uint8_t field_2[2];
        union
        {
            uint8_t* mData[128];
            uint32_t mDataOffset[128];
        };

        void FillMap(const rage::datResource& rsc)
        {
            mVirtualCount  = (uint8_t)rsc.Map->VirtualCount;
            mPhysicalCount = (uint8_t)rsc.Map->PhysicalCount;

            for(uint8_t i = 0; i < mVirtualCount + mPhysicalCount; i++)
            {
                mData[i] = (uint8_t*)rsc.Map->Chunks[i].SrcAddr;
            }
        }
    };

    class pgBase : public datBase
    {
    public:
        pgBase() {}

        pgBase(const pgBase& rhs)
        {
            mPageMap = nullptr;
        }

        ~pgBase()
        {
            DestroyPageMap();
        }

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            if(depth)
            {
                DestroyPageMap();
            }
            else
            {
                if(!mPageMap)
                {
                    mPageMap = new pgBasePageMap();
                }

                layout.AddObject(mPageMap, RSC5Layout::eBlockType::VIRTUAL);
            }
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            if(depth)
            {
                if(mPageMap)
                {
                    DestroyPageMap();
                }
            }
            else
            {
                mPageMap->FillMap(rsc);

                layout.SerializePtr(mPageMap, 4 + (rsc.Map->VirtualCount + rsc.Map->PhysicalCount) * sizeof(uint32_t));
            }
        }

        inline void Place(void*, struct datResource& rsc)
        {
            DestroyPageMap();
        }

        void DestroyPageMap()
        {
            if(mPageMap)
            {
                delete mPageMap;
            }

            mPageMap = nullptr;
        }

        pgBasePageMap* mPageMap = nullptr;
    };
    ASSERT_SIZE(pgBase, 0x8);
}