﻿#pragma once
#include <BDXKEngine.h>
#include "Editor/CreationMenu.h"
#include "AutoDestroy.h"

namespace Assembly
{
    using namespace BDXKEngine;
    using namespace BDXKEditor;

    class CreateCube : public Component, public AwakeHandler, public PostRenderHandler
    {
        ObjectPtr<Transform> transform;
        ObjectPtr<GameObject> prefab;

        void OnAwake() override
        {
            transform = GetTransform();

            prefab = CreationMenu::Object3D::Cube(L"预制体");
            //prefab->SetIsEnabling(false);
            const ObjectPtr<Renderer> meshRender = prefab->GetComponent<Renderer>();
            const ObjectPtr<Material> material = meshRender->GetMaterial();
            material->SetRenderQueue(RenderQueue::Transparent);
            const std::vector<ObjectPtr<Shader>> shaders = material->GetShaders();
            shaders[0]->SetBlend(Blend::Multiply);
            //prefab->AddComponent<AutoDestroy>();

            // prefab = GameObject::Create(L"预制体");
            // prefab->GetTransform()->SetLocalScale({1,2,3});
        }

        void OnPostRender() override
        {
            const Vector2 size = Screen::GetSize();

            if (GUI::Button({10, size.y - 40, 180, 30}, L"放置方块"))
            {
                const ObjectPtr<GameObject> instance = Instantiate(prefab.ToObject());
                const ObjectPtr<Transform> instanceTransform = instance->GetTransform();
                //instance->SetIsEnabling(true);
                instanceTransform->SetLocalPosition(transform->GetPosition());
                instanceTransform->SetLocalEulerAngles(transform->GetEulerAngles());
            }

            GUI::TextArea({200, size.y - 40, 200, 30}, L"鼠标位置:" + Input::GetMousePosition().ToString());
        }
    };
}
