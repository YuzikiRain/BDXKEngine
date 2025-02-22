﻿#include "ShaderImporter.h"

#include "BDXKEngine/Platform/GL/Shader/ShaderImport.h"

namespace BDXKEditor
{
    ObjectPtrBase ShaderImporter::ImportAsset(const std::string& filePath)
    {
        return static_cast<ObjectPtrBase>(ShaderImport::Hlsl(filePath));
    }
}
