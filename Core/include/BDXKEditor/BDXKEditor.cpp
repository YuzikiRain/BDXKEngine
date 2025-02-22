﻿#include "BDXKEditor.h"
#include "BDXKEngine/Base/Data/String/String.h"
#include "BDXKEngine/Framework/Scene.h"
#include "BDXKEngine/Framework/Behavior/Animator.h"
#include "BDXKEngine/Platform/Serialization/Serialization.h"
#include "BDXKEngine/Platform/GUI/GUI.h"
#include "Function/Assets.h"
#include "Function/AssetsBuiltIn.h"
#include "Function/ProjectSettings.h"
#include "Framework/Menu/CreateAssetMenu.h"
#include "Framework/Menu/GameObjectMenu.h"

namespace BDXKEditor
{
    ObjectPtr<EditorSystem> EditorSystem::editorSystem;
    ObjectPtr<SceneWindow> EditorSystem::sceneWindow;
    ObjectPtr<HierarchyWindow> EditorSystem::hierarchyWindow;
    ObjectPtr<InspectorWindow> EditorSystem::inspectorWindow;
    ObjectPtr<ProfilerWindow> EditorSystem::profilerWindow;
    ObjectPtr<ConsoleWindow> EditorSystem::consoleWindow;
    ObjectPtr<ProjectWindow> EditorSystem::projectWindow;
    ObjectPtr<GameWindow> EditorSystem::gameWindow;
    bool EditorSystem::isPlaying = false;
    std::string EditorSystem::playScene = {};
    ObjectPtrBase EditorSystem::copying = {};

    const ObjectPtr<SceneWindow>& EditorSystem::GetSceneView()
    {
        return sceneWindow;
    }
    const ObjectPtr<HierarchyWindow>& EditorSystem::GetHierarchyView()
    {
        return hierarchyWindow;
    }
    const ObjectPtr<InspectorWindow>& EditorSystem::GetInspectorView()
    {
        return inspectorWindow;
    }

    void EditorSystem::NewScene()
    {
        Scene::Create("Untitled.scene");
    }
    void EditorSystem::SaveScene()
    {
        auto scene = Scene::GetCurrentScene();
        Assets::Save(Scene::GetCurrentSceneName(), scene);
        Scene::Save(); //提供给非编辑器状态用
    }
    void EditorSystem::Copy()
    {
        copying = inspectorWindow->GetTarget();
    }
    void EditorSystem::Paste()
    {
        if (copying.IsNull()) return;

        if (copying.ToObject<GameObject>() != nullptr)
        {
            ObjectPtrBase clone = Serialization::Clone(copying);
        }
        else if (Serialization::IsPersistent(copying))
        {
            ObjectPtrBase clone = Serialization::Clone(copying);
            const std::string path = Assets::GetAssetPath(copying);
            const std::string::size_type splitIndex = path.rfind('.'); //没后缀也不支持导入啊，所以百分百能搜到
            const std::string fullName = path.substr(0, splitIndex);
            const std::string extension = path.substr(splitIndex);

            int cloneID = 0;
            std::string clonePath = {};
            do
            {
                cloneID++;
                clonePath = std::format("{} {}{}", fullName, cloneID, extension);
            }
            while (Assets::IsExisting(clonePath) == true);

            Assets::Save(clonePath, clone);
        }
    }
    void EditorSystem::Play()
    {
        if (isPlaying == false)
        {
            playScene = Scene::GetCurrentSceneName();
            SaveScene();
            SetConstructedObjectEvent({});
            Scene::Load(Scene::GetCurrentSceneName(), false);
            isPlaying = true;
        }
        else
        {
            //取消用户物体的隐藏
            for (const ObjectPtr<GameObject>& gameObject : GameObject::GetGameObjectsHiding())
            {
                if (gameObject->GetName().find("BDXKEditor") != std::string::npos)
                    continue;

                GameObject::UnHide(gameObject);
            }
            //创建物体需要资源化
            SetConstructedObjectEvent([](const Object* object)
            {
                Serialization::MarkPersistent(object->GetInstanceID());
            });
            Scene::Load(playScene, true);
            isPlaying = false;
        }
    }

    void EditorSystem::DrawMenu(const std::vector<std::string>& path, const std::function<void()>& func, size_t layer)
    {
        if (layer > path.size())
            return;

        if (layer == path.size() - 1)
        {
            if (ImGui::MenuItem(path.back().c_str()))
                func();
        }
        else
        {
            if (ImGui::BeginMenu(path[layer].c_str()))
            {
                DrawMenu(path, func, layer + 1);
                ImGui::EndMenu();
            }
        }
    }

    void EditorSystem::OnDrawGUI()
    {
        ImGui::ShowDemoWindow();

        //绘制菜单栏
        ImGui::PushStyleColor(ImGuiCol_MenuBarBg, isPlaying ? Color::darkGreen : Color{ImGui::GetStyleColorVec4(ImGuiCol_MenuBarBg)});
        if (ImGui::BeginMainMenuBar())
        {
            //菜单项
            std::vector<std::string> path = {};
            for (auto& [name,func] : Menu::GetMenuItems())
            {
                String::Split(name, "/", path);
                DrawMenu(path, func);
            }

            //其他信息
            ImGui::PushStyleColor(ImGuiCol_Text, Color::yellow);
            {
                //复制信息
                if (copying.IsNotNull() && ImGui::MenuItem(("Copying:" + copying->GetType()).c_str()))
                    copying = nullptr;
                //当前场景名字
                static char sceneName[64];
                strcpy_s(sceneName, Scene::GetCurrentSceneName().data());
                ImGui::InputText("##CurrentSceneName", sceneName, sizeof (sceneName));
                Scene::GetCurrentSceneName() = sceneName;
            }
            ImGui::PopStyleColor();

            ImGui::EndMainMenuBar();
        }
        ImGui::PopStyleColor();
    }

    void EditorSystem::Initialize(const std::string& sceneName)
    {
        AddQuitCondition([] { return false; });

        AssetsBuiltIn::Initialize();

        //初始化编辑器窗口
        hierarchyWindow = EditorWindow::Create<HierarchyWindow>(false);
        sceneWindow = EditorWindow::Create<SceneWindow>(false);
        inspectorWindow = EditorWindow::Create<InspectorWindow>(false);
        profilerWindow = EditorWindow::Create<ProfilerWindow>(false);
        consoleWindow = EditorWindow::Create<ConsoleWindow>(false);
        projectWindow = EditorWindow::Create<ProjectWindow>(false);
        gameWindow = EditorWindow::Create<GameWindow>(false);
        EditorWindow::Create<GameWindow>()->Show(); //两个游戏窗口便于观看
        hierarchyWindow->SetClickGameObjectEvent([](const ObjectPtr<GameObject>& gameObject)
        {
            sceneWindow->SetTarget(gameObject);
            inspectorWindow->SetTarget(gameObject);
        });
        profilerWindow->SetClickObjectEvent([](const ObjectPtrBase& object)
        {
            sceneWindow->SetTarget(object.ToObject<GameObject>());
            inspectorWindow->SetTarget(object);
        });
        projectWindow->SetClickObjectEvent([](const ObjectPtrBase& object)
        {
            inspectorWindow->SetTarget(object);
        });
        hierarchyWindow->Show();
        sceneWindow->Show();
        inspectorWindow->Show();
        profilerWindow->Show();
        consoleWindow->Show();
        projectWindow->Show();
        gameWindow->Show();

        //启动编辑器系统
        editorSystem = Create<EditorSystem>();

        //非运行模式下创建的物体都是标记持久化的物体
        SetConstructedObjectEvent([](const Object* object)
        {
            Serialization::MarkPersistent(object->GetInstanceID());
        });

        //加载场景
        if (Assets::IsExisting(sceneName) == false)
        {
            //创建默认场景
            Scene::Create(sceneName);
            RenderSettings::SetSkybox(CreateAssetMenu::CreateSkyboxMaterial());
            const ObjectPtr<GameObject> sun = GameObjectMenu::CreateDirectionalLight();
            sun->SetName("Sun");
            sun->SetLocalEulerAngles({45, -45, 0});
            const ObjectPtr<GameObject> ground = GameObjectMenu::CreatePlane();
            ground->SetName("Ground");
            const ObjectPtr<GameObject> sphere = GameObjectMenu::CreateSphere();
            sphere->SetName("Sphere");
            sphere->SetLocalPosition({0, 0.5f, 0});
            const ObjectPtr<GameObject> camera = GameObjectMenu::CreateCamera();
            camera->SetName("Camera");
            camera->SetLocalPosition({0, 1, -10});
            SaveScene();
        }
        else Assets::Load<Scene>(sceneName, true);
    }
    void EditorSystem::OnEngineBegin()
    {
        ImGui::LoadIniSettingsFromDisk(ProjectSettings::ParsePath("EditorWindow.settings").c_str());

        const Settings* settings = ProjectSettings::Load<Settings>("EditorSettings.settings");
        if (settings != nullptr)
        {
            Initialize(settings->sceneName);
            delete settings;
            return;
        }

        Initialize("Main.scene");
    }
    void EditorSystem::OnEngineEnd()
    {
        ImGui::SaveIniSettingsToDisk(ProjectSettings::ParsePath("EditorWindow.settings").c_str());

        Settings settings;
        settings.sceneName = isPlaying ? playScene : Scene::GetCurrentSceneName();
        ProjectSettings::Save("EditorSettings.settings", &settings);
    }
}
