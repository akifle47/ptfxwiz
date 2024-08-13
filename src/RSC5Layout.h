#pragma once
#include "rage/Resource.h"

#include <vector>
#include <map>
#include <span>
#include <cassert>

//i dont really understand anything going on here pretty much everything is just copied from libertyFourXYZ

namespace rage
{
    struct datResourceInfo;
}

class RSC5Layout
{
public:
    enum class eBlockType : uint8_t
    {
        VIRTUAL  = 5,
        PHYSICAL = 6,
    };

    RSC5Layout() : mMainObjectSize(0), mResourceInfo(), mResource() {};

    template<typename T> 
    void Save(T& object, std::filesystem::path filePath, uint32_t version)
    {
        mMainObjectSize = sizeof(object);
        object.AddToLayout(*this, 0);
        Create();

        mResource = rage::datResource(mResourceInfo);
        object.SerializePtrs(*this, mResource, 0);

        void* mainObjFixUp = (void*)rage::datResourceFileHeader::VIRTUAL_BASE;
        mResource.PointerFixUp(mainObjFixUp);
        memcpy(mainObjFixUp, &object, sizeof(object));

        DeSerializePtrs();

        mResource.SaveToDisk(filePath, version, mResourceInfo);
    }

    //ideally these would be private functions that can be acessed by serializable classes in some way like declaring them friends but that would 
    //mean declaring every single serializable class a friend since friendship doesnt transfer to children and i cant think of a better way to do this rn
    template<typename T> 
    void AddObject(const T* object, eBlockType blockType, uint32_t count = 1)
    {
        assert(object != nullptr);
        assert(sizeof(T) > 0);
        assert(count > 0);

        RSC5Object rscObject
        {
            .Ptr = object,
            .Size = sizeof(T),
            .Count = count,
            .BlockType = blockType,
        };

        if(std::find(mObjects.cbegin(), mObjects.cend(), rscObject) != mObjects.end())
            return;

        mObjects.push_back(rscObject);
    }

    template<typename T> 
    void SerializePtr(T* &obj, uint32_t objSize)
    {
        mOldObjectsPositions.push_back(std::make_pair<void*, void*>((void*)&obj, (void*)*(uint32_t*)&obj));

        assert(obj != nullptr);
        assert(mObjectsPositions.find(obj) != mObjectsPositions.end());

        T* oldObj = obj;
        obj = (T*)mObjectsPositions[obj];
        memcpy((uint8_t*)obj + mResource.GetFixUp(obj), oldObj, objSize);
    }

    void BackupPtr(void* ptr)
    {
        mOldObjectsPositions.push_back(std::make_pair<void*, void*>((void*)&ptr, (void*)*(uint32_t*)&ptr));
    }

private:
    struct RSC5Object
    {
        uint32_t GetSize() const
        {
            return Size * Count;
        }

        bool operator==(const RSC5Object& rhs) const
        {
            return Ptr == rhs.Ptr;
        }

        const void* Ptr;
        uint32_t Size;
        uint32_t Count;

        eBlockType BlockType;
    };

private:
    void Create();
    void DeSerializePtrs();
    void ProcessObjects(std::span<RSC5Object> objects);

    rage::datResource mResource;
    rage::datResourceInfo mResourceInfo;

    std::vector<RSC5Object> mObjects;
    std::map<const void*, const void*> mObjectsPositions;
    std::vector<std::pair<void*, void*>> mOldObjectsPositions;

    uint32_t mMainObjectSize;
};