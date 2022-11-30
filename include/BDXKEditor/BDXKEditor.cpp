﻿#include "BDXKEditor.h"
#include <iostream>
#include <ranges>
#include "BDXKEngine/BDXKEngine.h"
#include "BDXKEngine/Framework/Scene.h"
#include "BDXKEngine/Framework/Behavior/Animator.h"
#include "BDXKEngine/Function/Time/Time.h"
#include "EditorWindow/Core/EditorWindow.h"
#include "imgui/imgui.h"

namespace BDXKEditor
{
    ObjectPtr<SceneWindow> EditorSystem::sceneView;
    ObjectPtr<HierarchyWindow> EditorSystem::hierarchyView;
    ObjectPtr<InspectorWindow> EditorSystem::inspectorView;


    const ObjectPtr<SceneWindow>& EditorSystem::GetSceneView()
    {
        return sceneView;
    }
    const ObjectPtr<HierarchyWindow>& EditorSystem::GetHierarchyView()
    {
        return hierarchyView;
    }
    const ObjectPtr<InspectorWindow>& EditorSystem::GetInspectorView()
    {
        return inspectorView;
    }

    void EditorSystem::OnDrawGUI()
    {
        ImGui::ShowDemoWindow();
    }
    void EditorSystem::OnAwake()
    {
        hierarchyView = EditorWindow::Create<HierarchyWindow>();
        sceneView = EditorWindow::Create<SceneWindow>();
        inspectorView = EditorWindow::Create<InspectorWindow>();

        hierarchyView->SetClickGameObjectEvent([](const ObjectPtr<GameObject>& gameObject)
        {
            sceneView->SetTarget(gameObject);
            inspectorView->SetTarget(gameObject);
        });

        hierarchyView->Show();
        sceneView->Show();
        inspectorView->Show();
    }
    void EditorSystem::OnDestroy()
    {
        sceneView = nullptr;
        hierarchyView = nullptr;
        inspectorView = nullptr;
    }
    void TestLight()
    {
        ObjectPtr<GameObject> aureole = GameObject::Create("Aureole");
        {
            aureole->SetParent(GameObject::Find("Camera"));
            aureole->SetLocalPosition({0, 0, 0.7f});

            const ObjectPtr<Animator> animator = Component::Create<Animator>(aureole);
            animator->SetAnimation(AnimatorClip::Create([](ObjectPtr<GameObject> transform)
            {
                Vector3 position = transform->GetLocalPosition();
                position.x = std::cosf(Time::GetRealtimeSinceStartup()) * 0.3f;
                position.y = std::sinf(Time::GetRealtimeSinceStartup()) * 0.3f;
                transform->SetLocalPosition(position);

                Vector3 eulerAngles = transform->GetLocalEulerAngles();
                eulerAngles.z -= 90 * Time::GetDeltaTime();
                transform->SetLocalEulerAngles(eulerAngles);
            }));
        }

        ObjectPtr<GameObject> sphere = Creation::CreateSphere("Small Sphere");
        ObjectPtr<Light> sphere_light;
        {
            sphere->SetParent(aureole);
            sphere->SetLocalScale({0.1f, 0.1f, 0.1f});

            sphere_light = Creation::CreatePointLight("Red PointLight")->GetComponent<Light>();
            sphere_light->SetType(LightType::Point);
            sphere_light->SetColor(Color::red);
            sphere_light->SetIntensity(0.5f);
            sphere_light->GetGameObject()->SetParent(sphere);

            ObjectPtr<Camera> camera = GameObject::Find("Camera")->GetComponent<Camera>();
            camera->SetClearFlags(ClearFlags::Skybox);
            RenderSettings::GetSingleton()->skybox = sphere_light->GetShadowMap().ToObjectPtr<TextureCube>();
        }

        ObjectPtr<GameObject> cube = Creation::CreateCube("Stick");
        {
            cube->SetParent(aureole);
            cube->SetLocalScale({0.2f, 0.05f, 0.05f});
        }
    }
    void Run()
    {
        BDXKEngine::Run([]
        {
            Scene::CreateDefault();
            TestLight();
            Component::Create<EditorSystem>(GameObject::Create(":EditorSystem"));
        });

        std::cout << "\n按任意键退出" << std::endl;
        std::getchar();
    }
}