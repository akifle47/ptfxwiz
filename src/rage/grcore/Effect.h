#pragma once
#include "../math/Matrix.h"
#include "../Array.h"

#include <fstream>
#include <filesystem>

namespace rage
{
    class grcInstanceData
    {
    public:
        struct Entry 
        {
            union 
            {
                void* AsVoid;
                class grcTexturePC* AsTexture;
                class grcTextureReference* AsTextureRef;
                Vector4* AsVector4;
                Matrix44* AsMatrix44;
            };
        };

    public:
        friend class grcEffect;

        grcInstanceData() = default;
        grcInstanceData(const datResource& rsc);

        void AddToLayout(RSC5Layout& layout, uint32_t depth);
        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth);

        Entry* mEntries;
        class grcEffect* mEffect;
        uint32_t mCount;
        uint32_t mTotalSize;
        uint8_t* mEntriesCounts;
        uint32_t mEffectHash;
        void* field_18;
        uint32_t field_1C;
        uint32_t* mEntriesHashes;
        class grcEffect* mNextEffect;
        uint32_t field_28;
        uint8_t* mTechNameIndices;
    };
    ASSERT_SIZE(grcInstanceData, 0x30);


    struct grcRenderState
    {
        enum class Type : uint32_t
        {
            ZENABLE = 0, FILLMODE, ZWRITEENABLE, ALPHATESTENABLE, SRCBLEND, DESTBLEND, CULLMODE, ZFUNC, ALPHAREF, ALPHAFUNC, ALPHABLENDENABLE,
            STENCILENABLE, STENCILFAIL, STENCILZFAIL, STENCILPASS, STENCILFUNC, STENCILREF, STENCILMASK, STENCILWRITEMASK, COLORWRITEENABLE,
            COLORWRITEENABLE1, COLORWRITEENABLE2, COLORWRITEENABLE3, BLENDOP, BLENDOPALPHA, SEPARATEALPHABLENDENABLE, SRCBLENDALPHA, 
            DESTBLENDALPHA, INVALID_28, SLOPESCALEDEPTHBIAS, DEPTHBIAS, BLENDFACTOR, INVALID_32, INVALID_33, INVALID_34, TWOSIDEDSTENCILMODE,
            CCW_STENCILFAIL, CCW_STENCILZFAIL, CCW_STENCILPASS, CCW_STENCILFUNC, INVALID_40, INVALID_41, INVALID_42
        };
        
        enum class eBoolValue   : uint32_t { GRC_FALSE = 0, GRC_TRUE };
        enum class eZBufferType : uint32_t { GRC_FALSE = 0, GRC_TRUE, USE_W };
        enum class eFillMode    : uint32_t { POINT = 1, WIREFRAME, SOLID};
        enum class eBlendMode   : uint32_t { ZERO = 1, ONE, SRCCOLOR, INVSRCCOLOR, SRCALPHA, INVSRCALPHA, DESTALPHA, INVDESTALPHA, DESTCOLOR, 
                                             INVDESTCOLOR, SRCALPHASAT, BOTHSRCALPHA, BOTHINVSRCALPHA, BLENDFACTOR, INVBLENDFACTOR, SRCCOLOR2, INVSRCCOLOR2 };
        enum class eCullMode    : uint32_t { NONE = 1, CLOCKWISE, COUNTER_CLOCKWISE};
        enum class eCmpFunc     : uint32_t { NEVER = 1, LESS, EQUAL, LESS_EQUAL, GREATER, NOT_EQUAL, GREATER_EQUAL, ALWAYS };
        enum class eStencilOp   : uint32_t { KEEP = 1, ZERO, REPLACE, INCREMENT_SATURATE, DECREMENT_SATURATE, INVERT, INCREMENT, DECREMENT };
        enum class eColorWrite  : uint32_t { RED = 0x1, GREEN = 0x2, BLUE = 0x4, ALPHA = 0x8, ALL = 0xF };
        enum class eBlendOp     : uint32_t { ADD = 1, SUBTRACT, REV_SUBTRACT, MIN, MAX };

        Type State;
        struct 
        {
            union
            {
                eZBufferType ZEnable;
                eFillMode    FillMode;
                eBoolValue   ZWriteEnable;
                eBoolValue   AlphaTestEnable;
                eBlendMode   SrcBlend;
                eBlendMode   DestBlend;
                eCullMode    CullMode;
                eCmpFunc     ZFunc;
                uint8_t      AlphaRef;
                eCmpFunc     AlphaFunc;
                eBoolValue   AlphaBlendEnable;
                eBoolValue   StencilEnable;
                eStencilOp   StencilFail;
                eStencilOp   StencilZFail;
                eStencilOp   StencilPass;
                eCmpFunc     StencilFunc;
                uint32_t     StencilRef;
                uint32_t     StencilMask;
                uint32_t     StencilWriteMask;
                eColorWrite  ColorWriteEnable;
                eColorWrite  ColorWriteEnable1;
                eColorWrite  ColorWriteEnable2;
                eColorWrite  ColorWriteEnable3;
                eBlendOp     BlendOp;
                eBlendOp     BlendOpAlpha;
                eBoolValue   SeparateBlendAlphaEnable;
                eBlendOp     SrcBlendAlpha;
                eBlendOp     DestBlendAlpha;
                float        SlopeScaleDepthBias;
                float        DepthBias;
                uint32_t     BlendColorFactor;
                eBoolValue   TwoSidedStencilMode;
                eStencilOp   CounterClockwiseStencilFail;
                eStencilOp   CounterClockwiseStencilZFail;
                eStencilOp   CounterClockwiseStencilPass;
                eStencilOp   CounterClockwiseStencilFunc;
            };
        } Value;
    };


    class grcPass
    {
    public:
        friend class grcEffect;

        void Load(std::ifstream& file);
    
    private:
        uint32_t mProgramIndex;
        uint32_t field_4;
        uint16_t field_8;
        uint16_t mParamCount;
    };
    ASSERT_SIZE(grcPass, 0xC);

    class grcPasses
    {
    public:
        friend class grcEffect;

        void Load(std::ifstream& file);

    private:
        grcPass mPasses[2];
        atArray<grcRenderState> mRenderStates;
    };
    ASSERT_SIZE(grcPasses, 0x20);


    class grcTechnique
    {
    public:
        friend class grcEffect;

        grcTechnique() : mNameHash(0), mName(nullptr), mPasses() {}

        ~grcTechnique()
        {
            if(mName)
            {
                delete mName;
                mName = nullptr;
            }
        }

        void Load(std::ifstream& file);

    private:
        uint32_t mNameHash;
        char* mName;
        atArray<grcPass> mPasses;
    };
    ASSERT_SIZE(grcTechnique, 0x10);


    class grcAnnotation
    {
    public:
        enum class AnnotationType : uint32_t
        {
            NONE = 0, UNK_2 = 2
        };

    public:
        ~grcAnnotation()
        {
            if(mType == AnnotationType::UNK_2 && mValue.AsVoid)
            {
                delete[] mValue.AsVoid;
            }
        }

        void Load(std::ifstream& file);

        char* mName;
        AnnotationType mType;

    private:
        struct
        {
            union 
            {
                float AsFloat;
                void* AsVoid;
            };
        } mValue;
    };
    ASSERT_SIZE(grcAnnotation, 0xC);


    class grcParameter
    {
    public:
        friend class grcEffect;
        friend class grcInstanceData;

        enum class Type : uint8_t
        {
            NONE = 0, INT, FLOAT, VECTOR2, VECTOR3, VECTOR4, TEXTURE, BOOL, MATRIX3X4, MATRIX4X4, STRING, COUNT
        };

        
    public:
        grcParameter() : mName1(nullptr), mName2(nullptr)
        {
            mValue.AsVoid = nullptr;
        }

        ~grcParameter()
        {
            if(mName1)
            {
                delete mName1;
                mName1 = nullptr;
            }
            if(mName2)
            {
                delete mName2;
                mName2 = nullptr;
            }
            if(mValue.AsVoid)
            {
                delete[] mValue.AsVoid;
                mValue.AsVoid = nullptr;
            }
        }

        const char* GetName1() const { return mName1; };
        const char* GetName2() const { return mName2; };
        uint32_t GetName1Hash() const { return mName1Hash; };
        uint32_t GetName2Hash() const { return mName2Hash; };

        Type GetType() const { return mType; }

        uint32_t GetTotalSize() const;
        void Load(std::ifstream& file);

    private:
        Type mType;
        uint8_t mCount;
        uint8_t mSize;
        uint8_t mAnnotationCount;
        char* mName1;
        char* mName2;
        uint32_t mName1Hash;
        uint32_t mName2Hash;
        grcAnnotation* mAnnotations;
        struct
        {
            union 
            {
                int32_t AsInt;
                float AsFloat;
                Vector2* AsVector2;
                Vector3* AsVector3;
                Vector4* AsVector4;
                char* AsString;
                void* AsVoid;
            };
        } mValue;
        uint16_t mVertexRegister;
        uint16_t mPixelRegister;
        uint16_t field_20;
        uint16_t field_22;
        int8_t field_24[12];
    };
    ASSERT_SIZE(grcParameter, 0x30);


    class grcProgram
    {
    public:
        class Parameter
        {
        public:
            enum class Type : uint8_t
            {
                NONE = 0, INT, FLOAT, VECTOR2, VECTOR3, VECTOR4, TEXTURE, BOOL, MATRIX3X4, MATRIX4X4, STRING, COUNT
            };

        public:
            Parameter() : mType(Type::NONE), field_1(0), mRegisterIndex(0xFFFF), mNameHash(0) {};

            void Load(std::ifstream& file);

            Type mType;
            int8_t field_1;
            uint16_t mRegisterIndex;
            uint32_t mNameHash;
        };
        ASSERT_SIZE(Parameter, 0x8);

    public:
        grcProgram() : mParamCount(0), mParams(nullptr), mShader(nullptr) {}

        ~grcProgram()
        {
            if(mParams)
            {
                for(uint32_t i = 0; i < mParamCount; i++)
                {
                    delete[] mParams;
                    mParams = nullptr;
                }
            }
        }

        //rage has a different load function for each program type but i dont care about d3d shaders for this so i only use 1
        void Load(std::ifstream& file);

    protected:
        uint32_t mParamCount;
        Parameter* mParams;
        struct IDirect3DResource9* mShader;
    };
    ASSERT_SIZE(grcProgram, 0xC);

    using grcVertexProgram = grcProgram;
    using grcPixelProgram  = grcProgram;


    class grcEffect
    {
    public:
        grcEffect() = default;

        grcEffect(std::filesystem::path filePath);

        ~grcEffect()
        {
            if(mFilePath)
            {
                delete mFilePath;
                mFilePath = nullptr;
            }
        }

        const grcParameter* FindParameterByName(const char* name) const;
        const grcParameter* FindParameterByHash(uint32_t hash) const;

        uint32_t GetHash() const { return mEffectHash; }

        static constexpr uint32_t MAGIC = (uint32_t)'axgr';

    private:
        atArray<grcTechnique> mTechniques;
        atArray<grcParameter> mParameters;
        atArray<grcVertexProgram> mVertexPrograms;
        atArray<grcPixelProgram> mPixelPrograms;
        char* mFilePath;
        uint32_t mEffectHash;
        uint64_t mFileTime;
        grcInstanceData mInstanceData;
    };
    ASSERT_SIZE(grcEffect, 0x60);
}