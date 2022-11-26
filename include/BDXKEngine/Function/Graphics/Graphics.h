﻿#pragma once
#include "GraphicsInfo.h"
#include "Material.h"
#include "BDXKEngine/Platform/GL/Mesh/Mesh.h"
#include "BDXKEngine/Platform/GL/Texture/Texture2D.h"
#include "BDXKEngine/Platform/GL/Texture/TextureCube.h"
#include "BDXKEngine/Platform/Window/Window.h"

namespace BDXKEngine
{
    class Graphics
    {
    public:
        static void Initialize(Window* window);

        static void SetCameraInfo(CameraInfo cameraInfo, const ObjectPtr<TextureCube>& skybox);
        static void SetLightInfo(LightInfo lightInfo, const ObjectPtr<Texture>& shadowMap);
        static void SetCameraInfoNull();
        static void SetLightInfoNull();

        static void DrawMeshNow(const ObjectPtr<Mesh>& mesh);
        static void DrawTexture(const ObjectPtr<Texture2D>& texture, Rect screenRect);
        static void Blit(const ObjectPtr<Texture2D>& source, const ObjectPtr<Texture2D>& dest, const ObjectPtr<Material>& material);
    private:
        static ObjectPtr<Buffer> cameraInfoBuffer;
        static ObjectPtr<Buffer> lightInfoBuffer;

        static Window* window;
        static ObjectPtr<Mesh> drawTextureMesh;
        static ObjectPtr<Texture2D> defaultTexture2D;
        static ObjectPtr<TextureCube> defaultTextureCube;
    };
}
