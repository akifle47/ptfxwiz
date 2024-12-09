#pragma once
#include "rapidjson/include/document.h"

namespace JsonHelpers
{
    template<typename T>
    inline void LoadMemberObject(T& object, rapidjson::GenericObject<true, rapidjson::Value>& jsonObject, const char* objectName)
    {
        if(jsonObject.HasMember(objectName) && jsonObject[objectName].IsObject())
        {
            auto memberObject = jsonObject[objectName].Get<rapidjson::GenericObject<true, rapidjson::Value>>();
            object.LoadFromJson(memberObject);
        }
    }
}