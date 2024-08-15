#include "RSC5Layout.h"

#include <algorithm>

static constexpr uint8_t MAX_PAGE_SIZE_FOR_MERGE = 5;
static constexpr bool USE_SMALL_PAGES = true;
static constexpr bool MERGE_PAGES = true;

struct RSC5Info
{
    uint32_t PageCount      : 7;
    uint32_t Page2          : 1;
    uint32_t Page4          : 1;
    uint32_t Page8          : 1;
    uint32_t Page16         : 1;
    uint32_t PageSizeFactor : 4;
    uint32_t PageSize;
};

void RSC5Layout::Create()
{
    std::vector<RSC5Object> virtualObjects;
    std::vector<RSC5Object> physicalObjects;

    for(const auto& object : mObjects)
    {
        if(object.BlockType == eBlockType::VIRTUAL)
        {
            virtualObjects.push_back(object);
        }
        else if(object.BlockType == eBlockType::PHYSICAL)
        {
            physicalObjects.push_back(object);
        }
    }

    mResourceInfo.IsResource   = true;
    mResourceInfo.IsCompressed = true;

    struct
    {
        bool operator()(const RSC5Layout::RSC5Object &a, const RSC5Layout::RSC5Object &b) const { return a.Size > b.Size; }
    } sortBySize;

    std::sort(virtualObjects.begin(), virtualObjects.end(), sortBySize);
    std::sort(physicalObjects.begin(), physicalObjects.end(), sortBySize);

    ProcessObjects(virtualObjects);
    Log::Info("Done processing virtual objects.");
    ProcessObjects(physicalObjects);
    Log::Info("Done processing physical objects.");
}

void RSC5Layout::ProcessObjects(std::span<RSC5Object> objects)
{
    constexpr uint32_t basePageSize = 4096;

    if(objects.empty())
    {
        return;
    }
    
    //*should be the same for every entry otherwise something has gone catastrophically wrong
    eBlockType blockType = objects.front().BlockType;
    RSC5Info info {};

    if(MERGE_PAGES)
    {
        info.PageSizeFactor = MAX_PAGE_SIZE_FOR_MERGE;
        info.PageSize = basePageSize << info.PageSizeFactor;
    }

    //*ensure page can fit the largest object (?)
    for(const auto& object : objects)
    {
        while(object.Size > info.PageSize)
            info.PageSize = basePageSize << ++info.PageSizeFactor;
    }

    std::vector<uint32_t> pageMapping(1, info.PageSize);
    std::vector<uint8_t> isObjectUsed(objects.size());
    
    uint32_t pageSize = pageMapping.back();
    uint32_t posInPage = 0;
    uint32_t pagePos = 0;
    uint8_t  pageIndex = 0;
    uint32_t base = (uint32_t)blockType << 28;
    info.PageCount = 1;

    auto alignTo16 = [](uint32_t value) -> uint32_t { return (value + 15) & ~15; };

    if(blockType == eBlockType::VIRTUAL)
    {
        posInPage += alignTo16(mMainObjectSize);
    }

    uint32_t smallPagesSizes[] {0, 0, 0, 0};
    uint32_t usedSmallPages [] {info.Page2, info.Page4, info.Page8, info.Page16};

    for(uint8_t pass = 0; pass < 1 + USE_SMALL_PAGES; pass++)
    {
        while(!std::all_of(isObjectUsed.crbegin(), isObjectUsed.crend(), [](bool b) { return b == true; } ))
        {
            uint32_t selectedObjectSize = 0;
            int32_t selectedObjectIndex = -1;

            //this is probably only faster in debug builds but i'll go insane if i have to wait 2 minutes every time i have to test my code
            uint8_t* isObjectUsedPtr = isObjectUsed.data();
            uint32_t objCount = objects.size();
            //*find the largest unused object
            for(uint32_t i = 0; i < objCount; i++)
            {
                if(isObjectUsedPtr[i]) 
                    continue;

                uint32_t objectSize = objects[i].Size;
                if(posInPage + objectSize <= pageSize && objectSize > selectedObjectSize)
                {
                    selectedObjectSize = objectSize;
                    selectedObjectIndex = i;
                }
            }

            if(selectedObjectIndex != -1)
            {
                mObjectsPositions.insert(std::make_pair(objects[selectedObjectIndex].Ptr, (void*)(posInPage + base + pagePos)));
                posInPage += alignTo16(selectedObjectSize);
                isObjectUsed[selectedObjectIndex] = true;
            }
            // if we have not found an object or the position in the page is equal to its size, go to the next page or create a new one
            else if(selectedObjectIndex == -1 || posInPage == pageSize)
            {
                pagePos += pageSize;
                pageIndex++;
                if(pageIndex >= pageMapping.size())
                {
                    assert(pass != 1);

                    pageMapping.push_back(info.PageSize);
                    info.PageCount++;
                }

                pageSize = pageMapping[pageIndex];
                posInPage = 0;
            }
        }

        if(pass == 1)
        {
            // remove unused small pages
            while(pageIndex != pageMapping.size() - 1)
            {
                for(int8_t i = 3; i >= 0; i--)
                {
                    if(usedSmallPages[i])
                    {
                        usedSmallPages[i] = 0;
                        pageMapping.pop_back();
                        break;
                    }
                }
            }

            uint8_t lastPageCount = usedSmallPages[0] + usedSmallPages[1] + usedSmallPages[2] + usedSmallPages[3];
            // if not used last small page and block have small pages
            if(!usedSmallPages[3] && lastPageCount)
            {
                uint8_t usePage = 0;

                // check what small page size we need
                for(int8_t i = 3; i >= 0; i--)
                {
                    if(posInPage <= smallPagesSizes[i])
                    {
                        usePage = i; 
                        break;
                    }
                }

                // delete the last small page, no matter what size it is
                for(int8_t i = 3; i >= 0; i--)
                {
                    if(usedSmallPages[i])
                    {
                        usedSmallPages[i] = 0;
                        pageMapping.pop_back();
                        break;
                    }
                }

                // add the last small page
                usedSmallPages[usePage] = 1;
                pageMapping.push_back(smallPagesSizes[usePage]);
            }
        }


        if(MERGE_PAGES && pass == 0)
        {
            // if we have only one large page and if we use the minimum size for the page, 
            // then if necessary it needs to be reduced so that it does not waste more space
            if(pageMapping.size() == 1)
            {
                while(info.PageSizeFactor != 0 && posInPage <= info.PageSize / 2)
                    info.PageSize = basePageSize << --info.PageSizeFactor;

                pageMapping[0] = info.PageSize;
            }
        }

        // we have a page size, so we generate the size of small pages
        if(pass == 0)
        {
            smallPagesSizes[0] = info.PageSize / 2;
            smallPagesSizes[1] = info.PageSize / 4;
            smallPagesSizes[2] = info.PageSize / 8;
            smallPagesSizes[3] = info.PageSize / 16;
        }

        // if this is the first pass and if we need to use small pages, we need to check if the last large page can be split
        if(pass == 0 && USE_SMALL_PAGES)
        {
            bool canLastPageBeSplit = true;
            std::vector<std::pair<uint32_t, uint32_t>> posInPageAndSmallPageIndex;
            std::vector<uint32_t> objIndex;

            uint32_t lastPagePos = 0;
            for(uint8_t i = 0; i < pageMapping.size() - 1; i++) 
            {
                lastPagePos += pageMapping[i];
            }

            uint32_t usedMemInSmallPages[] {0, 0, 0, 0};
            if(info.PageCount == 1 && blockType == eBlockType::VIRTUAL) 
            {
                // if this is the first page, we need to reserve space for the main structure
                usedMemInSmallPages[0] = alignTo16(mMainObjectSize);
            }
            
            // checking each last page object to see if we can split the last page
            for(uint32_t i = 0; i < objects.size(); i++)
            {
                // if the object position is duplicated, cancel the entire process
                if(mObjectsPositions.count(objects[i].Ptr) != 1)
                {
                    return;
                }

                uint32_t currPtr = (uint32_t)mObjectsPositions[objects[i].Ptr];
                if(currPtr >> 28 == (uint32_t)blockType) 
                    currPtr &= 0x0FFFFFFF;
                else
                    continue;

                if(currPtr < lastPagePos) 
                    continue;

                uint32_t objSize = objects[i].Size;
                bool ok = false;

                // check in which small page this object can be placed
                for(int8_t j = 3; j >= 0; j--)
                {
                    uint32_t freeInPage = smallPagesSizes[j] - usedMemInSmallPages[j];

                    if(objSize < freeInPage)
                    {
                        objIndex.push_back(i);

                        for(uint8_t k = 0; k < 4; k++)
                        {
                            if(k == j) 
                                usedSmallPages[j] = 1;
                        }

                        usedMemInSmallPages[j] += alignTo16(objSize);
                        ok = true;
                        break;
                    }
                }

                // if not possible, we will return the pages to their standard position
                if(!ok)
                {
                    canLastPageBeSplit = false;
                    break;
                }
            }

            // if the page can be split, we need to remove objects from the last page to create a layout for them in the second pass.
            if(canLastPageBeSplit)
            {
                if(info.PageCount == 1 && blockType == eBlockType::VIRTUAL) 
                    // dont forget to reserve a place for the main structure
                    posInPage = alignTo16(mMainObjectSize);
                else 
                    posInPage = 0;

                info.PageCount--;

                pagePos = lastPagePos; // set the current position in the layout creation algorithm to the position of the first small page

                // fixing the array with pages
                pageMapping.pop_back();
                pageIndex = (uint8_t)pageMapping.size();

                for(uint8_t i = 0; i < 4; i++)
                {
                    if(usedSmallPages[i])
                        pageMapping.push_back(smallPagesSizes[i]);
                }
                pageSize = pageMapping[pageIndex];

                // clear the position of elements that are on the last page
                for(uint32_t i = 0; i < objIndex.size(); i++)
                {
                    mObjectsPositions.erase(objects[objIndex[i]].Ptr);
                    isObjectUsed[objIndex[i]] = false;
                }
            }
            else 
            {
                break;
            }
        }

        // we need at least one large page
        if(pass == 1 && info.PageCount < 1)
        {
            bool ok = false;
            for(uint8_t i = 0; i < 4; i++)
            {
                if(usedSmallPages[i] && info.PageSizeFactor > i)
                {
                    info.PageCount++;
                    info.PageSizeFactor -= i + 1;
                    info.PageSize = basePageSize << info.PageSizeFactor;

                    usedSmallPages[i] = 0;
                    for(uint8_t j = i + 1; j < 4; j++)
                    {
                        if(usedSmallPages[j])
                        {
                            usedSmallPages[j - i - 1] = 1;
                            usedSmallPages[j] = 0;
                        }
                    }

                    ok = true;
                    break;
                }
            }

            if(!ok)
            {
                info.PageCount = 1;
                usedSmallPages[0] = 0;
                usedSmallPages[1] = 0;
                usedSmallPages[2] = 0;
                usedSmallPages[3] = 0;
            }
        }
    }

    info.Page2  = usedSmallPages[0];
    info.Page4  = usedSmallPages[1];
    info.Page8  = usedSmallPages[2];
    info.Page16 = usedSmallPages[3];

    if (blockType == eBlockType::VIRTUAL)
    {
        mResourceInfo.VirtualChunk0 = info.PageCount;
        mResourceInfo.VirtualChunk1 = info.Page2;
        mResourceInfo.VirtualChunk2 = info.Page4;
        mResourceInfo.VirtualChunk3 = info.Page8;
        mResourceInfo.VirtualChunk4 = info.Page16;
        mResourceInfo.VirtualSize   = info.PageSizeFactor;
    }
    else if (blockType == eBlockType::PHYSICAL)
    {
        mResourceInfo.PhysicalChunk0 = info.PageCount;
        mResourceInfo.PhysicalChunk1 = info.Page2;
        mResourceInfo.PhysicalChunk2 = info.Page4;
        mResourceInfo.PhysicalChunk3 = info.Page8;
        mResourceInfo.PhysicalChunk4 = info.Page16;
        mResourceInfo.PhysicalSize   = info.PageSizeFactor;
    }
}

void RSC5Layout::DeSerializePtrs()
{
    for(auto& oldObj : mOldObjectsPositions)
    {
        *(uint32_t**)oldObj.first = (uint32_t*)oldObj.second;
    }
}