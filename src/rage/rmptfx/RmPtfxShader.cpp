#include "RmPtfxShader.h"
#include "rage/StringHash.h"

#undef GetObject

namespace rage
{
    void rmPtfxShader::WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();
        {
            writer.String("Name");
            writer.String(mName);

            writer.String("TechniqueName");
            writer.String(mTechName);

            if(mVars.GetCount())
            {
                writer.String("Variables");
                writer.StartArray();
                {
                    for(uint16_t i = 0; i < mVars.GetCount(); i++)
                    {
                        if(mVars[i].Get())
                        {
                            mVars[i]->WriteToJson(writer);
                        }
                    }
                }
                writer.EndArray();
            }

            writer.String("field_1C");
            writer.Bool(field_1C);
        }
        writer.EndObject();
    }
}