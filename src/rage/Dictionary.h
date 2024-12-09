#pragma once
#include "Base.h"
#include "Array.h"
#include "DatOwner.h"
#include "StringHash.h"
#include "../Utils.h"

namespace rage
{
    template<typename T>
    class pgDictionary : pgBase
    {
    public:
        pgDictionary() = default;

        pgDictionary(uint16_t capacity) : mCodes(capacity), mEntries(capacity) {}

        pgDictionary(const struct datResource& rsc) : mCodes(rsc), mEntries(rsc) {}

        ~pgDictionary()
        {
            mCodes.~atArray();

            if constexpr(requires { mEntries[0]->~T(); })
            {
                for(uint16_t i = 0; i < mEntries.GetCount(); i++)
                {
                    if(mEntries[i].Get())
                        mEntries[i]->~T();
                }
            }

            mEntries.~atArray();
        };

        void AddEntry(const char* name, T* entry)
        {
            AddEntry(rage::atStringHash(name), entry);
        }

        void AddEntry(uint32_t code, T* entry)
        {
            uint16_t index = 0;
            for(; index < mCodes.GetCount(); index++)
            {
                if(mCodes[index] > code || mCodes[index] == code)
                    break;
            }

            mCodes.Insert(index) = code;
            mEntries.Insert(index) = entry;
        }

        T* Find(uint32_t code)
        {
            int32_t left = 0;
            int32_t right = mCodes.GetCount() - 1;

            while(left <= right)
            {
                uint32_t middle = (left + right) / 2;

                uint32_t element = mCodes[middle];
                if(element == code)
                    return mEntries[middle].Get();
                else if(element < code)
                    left = middle + 1;
                else if(element > code)
                    right = middle - 1;
            }

            return nullptr;
        }

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