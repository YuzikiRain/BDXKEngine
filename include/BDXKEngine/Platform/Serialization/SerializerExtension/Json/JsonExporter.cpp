﻿#include "JsonExporter.h"
#include <rapidjson/prettywriter.h>
#include "BDXKEngine/Base/Data/String/String.h"

namespace BDXKEngine
{
    void JsonExporter::TransferJson(std::string key, std::string& value)
    {
        auto& allocator = GetAllocator();
        rapidjson::GenericValue<rapidjson::UTF8<>> keystring = {key.c_str(), static_cast<rapidjson::SizeType>(key.length()), allocator};

        rapidjson::Document document = {&allocator};
        Import(document, value);

        GetCurrentNode().AddMember(keystring, document, GetAllocator());
    }
    bool JsonExporter::IsImporter()
    {
        return false;
    }
    void JsonExporter::Reset(std::string& data)
    {
        Export(GetDocument(), data);
        ResetNodes();
    }
    void JsonExporter::PushPath(const std::string& key)
    {
        auto& allocator = GetAllocator();
        rapidjson::GenericValue<rapidjson::UTF8<>> keystring(key.c_str(), static_cast<rapidjson::SizeType>(key.length()), allocator);

        auto& currentNode = GetCurrentNode();
        if (currentNode.IsObject() == false)currentNode.SetObject();
        currentNode.AddMember(keystring, keystring, allocator);

        PushNode(&currentNode[key.c_str()]);
    }
    void JsonExporter::PopPath(std::string& key)
    {
        PopNode();
    }
    void JsonExporter::TransferInt(const int& value)
    {
        GetCurrentNode().SetInt(value);
    }
    void JsonExporter::TransferFloat(const float& value)
    {
        GetCurrentNode().SetFloat(value);
    }
    void JsonExporter::TransferBool(const bool& value)
    {
        GetCurrentNode().SetBool(value);
    }
    void JsonExporter::TransferVector2(const Vector2& value)
    {
        auto& node = GetCurrentNode().SetArray();

        auto& allocator = GetAllocator();
        node.PushBack(value.x, allocator);
        node.PushBack(value.y, allocator);
    }
    void JsonExporter::TransferVector3(const Vector3& value)
    {
        auto& node = GetCurrentNode().SetArray();

        auto& allocator = GetAllocator();
        node.PushBack(value.x, allocator);
        node.PushBack(value.y, allocator);
        node.PushBack(value.z, allocator);
    }
    void JsonExporter::TransferVector4(const Vector4& value)
    {
        auto& node = GetCurrentNode().SetArray();

        auto& allocator = GetAllocator();
        node.PushBack(value.x, allocator);
        node.PushBack(value.y, allocator);
        node.PushBack(value.z, allocator);
        node.PushBack(value.w, allocator);
    }
    void JsonExporter::TransferMatrix4x4(const Matrix4x4& value)
    {
        auto& node = GetCurrentNode().SetArray();

        auto& allocator = GetAllocator();
        node.PushBack(value.m00, allocator);
        node.PushBack(value.m10, allocator);
        node.PushBack(value.m20, allocator);
        node.PushBack(value.m30, allocator);

        node.PushBack(value.m01, allocator);
        node.PushBack(value.m11, allocator);
        node.PushBack(value.m21, allocator);
        node.PushBack(value.m31, allocator);

        node.PushBack(value.m02, allocator);
        node.PushBack(value.m12, allocator);
        node.PushBack(value.m22, allocator);
        node.PushBack(value.m32, allocator);

        node.PushBack(value.m03, allocator);
        node.PushBack(value.m13, allocator);
        node.PushBack(value.m23, allocator);
        node.PushBack(value.m33, allocator);
    }
    void JsonExporter::TransferColor(const Color& value)
    {
        auto& node = GetCurrentNode().SetArray();

        auto& allocator = GetAllocator();
        node.PushBack(value.r, allocator);
        node.PushBack(value.g, allocator);
        node.PushBack(value.b, allocator);
        node.PushBack(value.a, allocator);
    }
    void JsonExporter::TransferRect(const Rect& value)
    {
        auto& node = GetCurrentNode().SetArray();

        auto& allocator = GetAllocator();
        node.PushBack(value.x, allocator);
        node.PushBack(value.y, allocator);
        node.PushBack(value.width, allocator);
        node.PushBack(value.height, allocator);
    }
    void JsonExporter::TransferString(const std::string& value)
    {
        GetCurrentNode().SetString(value.c_str(), static_cast<rapidjson::SizeType>(value.size()), GetAllocator());
    }
    void JsonExporter::TransferBytes(const std::vector<char>& value)
    {
        std::string base64;

        base64.resize((value.size() + 2) / 3 * 4);
        base64.resize(String::EncodingBase64(
                reinterpret_cast<const unsigned char*>(value.data()),
                static_cast<int>(value.size()),
                base64.data())
        );

        TransferString(base64);
    }
}