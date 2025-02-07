﻿#pragma once
#include "Core/Importer.h"

namespace BDXKEditor
{
    class ObjectImporter : public Importer
    {
        ObjectPtrBase ImportAsset(const std::string& filePath) override;
    };

    CustomReflection(ObjectImporter)
    CustomAssetsImporter(material, ObjectImporter)
    CustomAssetsImporter(physicMaterial, ObjectImporter)
    CustomAssetsImporter(prefab, ObjectImporter)
}
