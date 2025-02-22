﻿#pragma once
#include "BDXKEngine/Base/Object/Object.h"
#include "Core/Camera.h"

namespace BDXKEngine
{
    class RenderEvent;

    class DrawGUIHandler
    {
        friend RenderEvent;
    protected:
        virtual void OnDrawGUI() = 0;
        virtual ~DrawGUIHandler() = default;
    };

    class DrawGizmosHandler
    {
        friend RenderEvent;
    protected:
        virtual void OnDrawGizmos() = 0;
        virtual ~DrawGizmosHandler() = default;
    };

    class RenderEvent
    {
    public:
        static void Initialize();
        
        static const ObjectPtr<Camera>& GetCurrentCamera();
        static const ObjectPtr<Texture2D>& GetCameraCanvas();
    private:
        static ObjectPtr<Camera> currentCamera;
        static ObjectPtr<Texture2D> cameraCanvas;//默认RenderTarget
        static ObjectPtr<Shader> blitShader;

        static void Render();
    };
}
