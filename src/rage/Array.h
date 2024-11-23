#pragma once
#include "Resource.h"
#include "../RSC5Layout.h"
#include "../Log.h"

namespace rage
{
    template <typename T, typename CounterT = uint16_t>
    class atArray
    {
    public:
        atArray() : mElements(nullptr), mCount(0), mCapacity(0) {}

        atArray(CounterT capacity)
        {
            mElements = new T[capacity];
            mCount = 0;
            mCapacity = capacity;
        }

        atArray(const atArray<T, CounterT>& that)
        {
            mCount = that.mCount;
            mCapacity = that.mCapacity;
            mElements = new T[mCapacity];

            for(CounterT i = 0; i < mCount; i++)
            {
                mElements[i] = that.mElements[i];
            }
        }

        atArray(const datResource& rsc)
        {
            if(!mElements)
                return;

            rsc.PointerFixUp(mElements);

            if constexpr(requires { mElements[0].Place(&mElements[0], rsc); })
            {
                for(CounterT i = 0; i < mCount; i++)
                {
                    mElements[i].Place(&mElements[i], rsc);
                }
            }
        }

        ~atArray()
        {
            Clear();
        }

        atArray<T, CounterT>& operator=(const atArray<T, CounterT>& that)
        {
            if(this == &that)
            {
                return *this;
            }

            Clear();

            mCount = that.mCount;
            mCapacity = that.mCapacity;
            mElements = new T[mCapacity];

            for(CounterT i = 0; i < mCount; i++)
            {
                mElements[i] = that.mElements[i];
            }

            return *this;
        }

        void Clear()
        {
            if(mElements)
            {
                delete[] mElements;
                mElements = nullptr;
            }
        }

        T& operator[](CounterT index)
        {
            return mElements[index];
        }

        const T& operator[](CounterT index) const
        {
            return mElements[index];
        }
        
        void Insert(T &entry, CounterT index)
        {
            if(mCount == mCapacity)
            {
                Grow();
            }

            for(CounterT i = mCount; i > index; i--)
            {
                mElements[i] = mElements[i - 1];
            }

            mCount++;
            mElements[index] = entry;
        }

        T& Append()
        {
            assert(mCount <= mCapacity);
            return mElements[mCount++];
        }

        T& Grow(CounterT allocStep = 16)
        {
            if(mCount == mCapacity)
            {
                mCapacity = CounterT(mCapacity + allocStep);

                T* newElements = new T[mCapacity];
                for(CounterT i = 0; i < mCount; i++)
                {
                    newElements[i] = mElements[i];
                }
                delete[] mElements;

                mElements = newElements;
            }

            return mElements[mCount++];
        }

        CounterT GetCount() const
        {
            return mCount;
        }

        CounterT GetCapacity() const
        {
            return mCapacity;
        }

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            if(!mElements)
                return;

            layout.AddObject(mElements, RSC5Layout::eBlockType::VIRTUAL, mCapacity);

            if constexpr(requires { mElements[0].AddToLayout(layout, depth); })
            {
                for(CounterT i = 0; i < mCount; i++)
                {
                    mElements[i].AddToLayout(layout, depth + 1);
                }
            }
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            if(!mElements)
                return;

            if constexpr(requires { mElements[0].SerializePtrs(layout, rsc, depth + 1); })
            {
                for(CounterT i = 0; i < mCount; i++)
                {
                    mElements[i].SerializePtrs(layout, rsc, depth + 1);
                }
            }

            layout.SerializePtr(mElements, sizeof(T) * mCapacity);
        }

        inline void Place(void* that, const datResource& rsc)
        {
            new(that) atArray<T>(rsc);
        }

    private:
        T* mElements;
        CounterT mCount;
        CounterT mCapacity;
    };
    ASSERT_SIZE(atArray<uint32_t>, 0x8);
}