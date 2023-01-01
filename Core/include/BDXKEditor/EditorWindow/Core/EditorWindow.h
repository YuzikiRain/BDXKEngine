﻿#pragma once
#include "BDXKEngine/Base/Object/Serializer/ObjectGuid.h"
#include "BDXKEngine/Base/Reflection/Reflection.h"
#include "BDXKEngine/Framework/Core/ScriptableObject.h"
#include "BDXKEngine/Framework/Renderer/RenderEvent.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;

    class EditorWindow : public ScriptableObject, public DrawGUIHandler
    {
    public:
        template <typename TEditorWindow>
        static ObjectPtr<TEditorWindow> Create()
        {
            ObjectPtr window = new TEditorWindow{};
            Reflection::GetReflection<ScriptableObject>().GetFieldOf<bool>(window.ToObjectBase(), "isEnabling") = false;
            ObjectGuid::RemoveGuid(window.GetInstanceID());
            Instantiate(window);
            return window;
        }

        void Show();
    protected:
        virtual bool HasMenu();
        virtual void OnGUI() = 0;
    private:
        static std::vector<ObjectPtr<EditorWindow>> windows;

        void OnDrawGUI() override;
        void Awake() override;
        void Destroy() override;
    };
}