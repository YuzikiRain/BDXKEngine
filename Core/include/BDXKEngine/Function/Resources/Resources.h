﻿#pragma once
#include "BDXKEngine/Base/Object/Core/ObjectPtr.h"
#include "BDXKEngine/Base/Object/Core/ObjectPtrBase.h"
#include "BDXKEngine/Base/Object/Serializer/ObjectSerializer.h"

namespace BDXKEngine
{
    class Resources
    {
    public:
        static ObjectPtrBase Load(const std::string& path);
        template <typename TObject>
        static ObjectPtr<TObject> Load(const std::string& path)
        {
            return Load(path).ToObject<TObject>();
        }
        static void Save(const std::string& path, const ObjectPtrBase& objectPtr);

        static bool IsExisting(const std::string& path);

        static Reflective* FindSerializationFallback(const Guid& guid);
    private:
        static void StaticConstructor();
        CustomStaticConstructor(StaticConstructor)
    };
}