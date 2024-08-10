#pragma once
#include "Base.h"
#include "Array.h"
#include "DatOwner.h"
#include "../Utils.h"

namespace rage
{
    template<typename T>
    class pgDictionary : pgBase
    {
    public:
        pgDictionary() = default;

        pgDictionary(const struct datResource& rsc) : mCodes(rsc), mEntries(rsc) 
        {}

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            pgBase::AddToLayout(layout, depth);

            mCodes.AddToLayout(layout, depth);
            mEntries.AddToLayout(layout, depth);
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            pgBase::SerializePtrs(layout, rsc, depth);

            if(mParent)
            {
                mParent = nullptr;
            }

            mCodes.SerializePtrs(layout, rsc, depth);
            mEntries.SerializePtrs(layout, rsc, depth);
        }

        inline void Place(void* that, const struct datResource& rsc)
        {
            new(that) pgDictionary<T>(rsc);
        }

        pgDictionary* mParent = nullptr;
        int32_t mRefCount = 1;
        atArray<uint32_t> mCodes;
        atArray<datOwner<T>> mEntries;
    };
    ASSERT_SIZE(pgDictionary<uint32_t>, 0x20);
}