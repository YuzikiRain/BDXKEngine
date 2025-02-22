﻿#pragma once
#include "Core/Importer.h"

namespace BDXKEditor
{
    class SceneImporter : public Importer
    {
        ObjectPtrBase ImportAsset(const std::string& filePath) override;
    };

    CustomReflection(SceneImporter)
    CustomAssetsImporter(scene, SceneImporter)
}
