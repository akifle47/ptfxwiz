#include "Effect.h"
#include "TextureReference.h"
#include "../StringHash.h"
#include "../../EffectList.h"

namespace rage
{
    static constexpr uint8_t sParamTypeSizeFactor[] { 0, 1, 1, 1, 1, 1, 0, 1, 3, 4, 0, 0, 0, 0, 0, 0 };

    //grcEffect

    grcEffect::grcEffect(std::filesystem::path filePath)
    {
        std::filesystem::path path(filePath);
        mEffectHash = atStringHash(path.stem().string().c_str());
        mFilePath = strdup(std::filesystem::absolute(filePath).string().c_str());

        const char* absolutePath = strdup(std::filesystem::absolute(filePath).string().c_str());
        std::ifstream file(filePath, std::ios::binary);
        if(!file.is_open())
        {
            Log::Error("Failed to load effect file \"%s\".", absolutePath);
        }

        uint32_t magic;
        file.read((char*)&magic, 4);
        if(magic != rage::grcEffect::MAGIC)
        {
            Log::Error("\"%s\" is not an effect file.", absolutePath);
        }

        uint8_t shaderCount;
        file.read((char*)&shaderCount, 1);
        mVertexPrograms = atArray<grcVertexProgram>(shaderCount);
        for(uint16_t i = 0; i < shaderCount; i++)
        {
            mVertexPrograms.Append().Load(file);
        }

        file.read((char*)&shaderCount, 1);
        mPixelPrograms = atArray<grcPixelProgram>(shaderCount);
        for(uint16_t i = 0; i < shaderCount; i++)
        {
            mPixelPrograms.Append().Load(file);
        }
        
        uint8_t paramCount;
        file.read((char*)&paramCount, 1);
        atArray<grcParameter> globals(paramCount);
        for(uint8_t i = 0; i < paramCount; i++)
        {
            globals.Append().Load(file);
        }

        file.read((char*)&paramCount, 1);
        mParameters = atArray<grcParameter>(paramCount);
        for(uint8_t i = 0; i < paramCount; i++)
        {
            mParameters.Append().Load(file);
        }

        uint8_t techCount;
        file.read((char*)&techCount, 1);
        mTechniques = atArray<grcTechnique>(techCount);
        for(uint8_t i = 0; i < techCount; i++)
        {
            mTechniques.Append().Load(file);
        }
    }

    const grcParameter* grcEffect::FindParameterByName(const char* name) const
    {
        return FindParameterByHash(atStringHash(name));
    }

    const grcParameter* grcEffect::FindParameterByHash(uint32_t hash) const
    {
        for(uint16_t i = 0; i < mParameters.GetCount(); i++)
        {
            if(mParameters[i].mName1Hash == hash)
                return &mParameters[i];
        }

        return nullptr;
    }

    //grcProgram

    void grcProgram::Load(std::ifstream& file)
    {
        file.read((char*)&mParamCount, 1);

        if(mParamCount)
        {
            mParams = new Parameter[mParamCount];
            for(uint32_t i = 0; i < mParamCount; i++)
            {
                mParams[i].Load(file);
            }
        }

        uint16_t shaderSize;
        file.read((char*)&shaderSize, 2);
        //compressed size. only supported by vertex shaders and not used by any of the game's shaders so i ignore it
        file.seekg(2, std::ios_base::cur);

        file.seekg(shaderSize, std::ios_base::cur);
    }

    static inline void ReadString(std::ifstream& file, char *&dest)
    {
        uint8_t strLen;
        char str[256];
        file.read((char*)&strLen, 1);
        file.read((char*)&str, strLen);

        dest = strdup(str);
    }

    void grcProgram::Parameter::Load(std::ifstream& file)
    {
        file.read((char*)&mType, 1);
        file.read((char*)&field_1, 1);
        file.read((char*)&mRegisterIndex, 2);

        char name[256];
        uint8_t nameLen;
        file.read((char*)&nameLen, 1);
        file.read((char*)&name, nameLen);

        mNameHash = atStringHash(name);
    }


    //grcParameter

    uint32_t grcParameter::GetTotalSize() const
    {
        return mCount * 16 * sParamTypeSizeFactor[(uint32_t)mType];
    }

    void grcParameter::Load(std::ifstream& file)
    {
        file.read((char*)&mType, 1);
        file.read((char*)&mCount, 1);

        if(!mCount && mType != Type::TEXTURE)
        {
            mCount = 1;
        }

        ReadString(file, mName1);
        mName1Hash = atStringHash(mName1);
        ReadString(file, mName2);
        mName2Hash = atStringHash(mName2);

        file.read((char*)&mAnnotationCount, 1);
        
        if(mAnnotationCount)
        {
            mAnnotations = new grcAnnotation[mAnnotationCount];

            for(uint8_t i = 0; i < mAnnotationCount; i++)
            {
                mAnnotations[i].Load(file);
            }
        }

        file.read((char*)&mSize, 1);

        if(!mSize)
            return;

        if(mType == Type::TEXTURE)
        {
            mValue.AsVoid = new uint8_t[4 * mSize];
            file.read((char*)mValue.AsVoid, 4 * mSize);
        }
        else
        {
            uint32_t totalSize = 4 * mCount * sParamTypeSizeFactor[(uint8_t)mType];
            mValue.AsVoid = new uint8_t[4 * totalSize];
            memset(mValue.AsVoid, 0, 4 * totalSize);

            uint32_t srcStride = mSize / mCount;
            uint32_t dstStride = totalSize / mCount;
            uint8_t* dst = (uint8_t*)mValue.AsVoid;

            for(uint8_t i = 0; i < mCount; i++)
            {
                file.read((char*)dst, 4 * srcStride);
                dst += dstStride * 4;
            }
        }
    }


    void grcAnnotation::Load(std::ifstream& file)
    {
        char* s = 0;
        ReadString(file, s);
        delete s;

        uint8_t type;
        file.read((char*)&type, 1);
        mType = (AnnotationType)type;

        if(mType == AnnotationType::UNK_2)
        {
            uint8_t size;
            file.read((char*)&size, 1);
            mValue.AsVoid = new uint8_t[size];
            file.read((char*)mValue.AsVoid, size);
        }
        else
        {
            file.read((char*)&mValue.AsFloat, 4);
        }
    }


    //grcTechnique

    void grcTechnique::Load(std::ifstream& file)
    {
        ReadString(file, mName);
        mNameHash = atStringHash(mName);

        uint8_t passCount;
        file.read((char*)&passCount, 1);
        mPasses = atArray<grcPass>(passCount);
        for(uint8_t i = 0; i < passCount; i++)
        {
            mPasses.Append().Load(file);
        }
    }


    //grPasses

    void grcPasses::Load(std::ifstream& file)
    {
        mPasses[0].Load(file);
        mPasses[1].Load(file);

        uint8_t stateCount;
        file.read((char*)&stateCount, 1);
        mRenderStates = atArray<grcRenderState>(stateCount);
        for(uint8_t i = 0; i < stateCount; i++)
        {
            grcRenderState& renderState = mRenderStates.Append();
            file.read((char*)&renderState.State, 4);
            file.read((char*)&renderState.Value, 4);
        }
    }

    void grcPass::Load(std::ifstream& file)
    {
        file.read((char*)&mProgramIndex, 1);
    }


    //grcInstanceData
    grcInstanceData::grcInstanceData(const datResource& rsc)
    {
        if(mEntries)
            rsc.PointerFixUp(mEntries);
        if(mEntriesCounts)
            rsc.PointerFixUp(mEntriesCounts);
        if(mEntriesHashes)
            rsc.PointerFixUp(mEntriesHashes);

        for(uint32_t i = 0; i < mCount; i++)
        {
            if(!mEntries[i].AsVoid)
                continue;

            rsc.PointerFixUp(mEntries[i].AsVoid);

            if(!mEntriesCounts[i])
            {
                if(mEntries[i].AsTextureRef->mResourceType == grcTexture::eType::REFERENCE)
                {
                    new(mEntries[i].AsTextureRef) grcTextureReference(rsc);
                }
            }
        }

        mEffect = EffectList::GetEffectByHash(mEffectHash);
        assert(mEffect != nullptr);
    }

    void grcInstanceData::AddToLayout(RSC5Layout& layout, uint32_t depth)
    {
        if(!mEntries)
            return;

        layout.AddObject((uint8_t*)mEntries, RSC5Layout::eBlockType::VIRTUAL, mTotalSize);

        for(uint8_t i = 0; i < mCount; i++)
        {
            if(!mEntries[i].AsVoid)
                continue;

            if(!mEntriesCounts[i])
            {
                if(mEntries[i].AsTextureRef->mResourceType == grcTexture::eType::REFERENCE)
                {
                    layout.AddObject(mEntries[i].AsTextureRef, RSC5Layout::eBlockType::VIRTUAL);
                    mEntries[i].AsTextureRef->AddToLayout(layout, depth);
                }
            }
        }
    }

    void grcInstanceData::SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
    {
        if(!mEntries)
            return;

        uint32_t countsOffset = 4 * ((mCount + 3) & ~3u); //alligned to 4
        uint32_t hashesOffset = ((mCount + 15) & ~15) + countsOffset; //alligned to 16
        uint32_t entriesOffset = countsOffset + hashesOffset;

        Entry* entriesO = mEntries;
        layout.SerializePtr(mEntries, mTotalSize);
        Entry* newEntries = (Entry*)((uint8_t*)(mEntries) + rsc.GetFixUp((void*)mEntries));

        for(uint8_t i = 0; i < mCount; i++)
        {
            if(!newEntries[i].AsVoid)
                continue;

            const grcParameter* param = mEffect->FindParameterByHash(mEntriesHashes[i]);

            if(param->mType == grcParameter::Type::TEXTURE)
            {
                if(newEntries[i].AsTextureRef->mResourceType == grcTexture::eType::REFERENCE)
                {
                    entriesO[i].AsTextureRef->SerializePtrs(layout, rsc, depth + 1);
                    layout.SerializePtr(entriesO[i].AsTextureRef, sizeof(grcTextureReference));
                    newEntries[i].AsTextureRef = entriesO[i].AsTextureRef;
                    continue;
                }
            }

            newEntries[i].AsVoid = ((uint8_t*)mEntries + entriesOffset);
            entriesOffset += param->GetTotalSize();
        }

        layout.BackupPtr(mEntriesCounts);
        mEntriesCounts = (uint8_t*)((uintptr_t)mEntries + countsOffset);

        layout.BackupPtr(mEntriesHashes);
        mEntriesHashes = (uint32_t*)((uintptr_t)mEntries + hashesOffset);
    }
}