﻿#pragma once
#include <BDXKEngine.h>
#include <imgui/imgui.h>

namespace BDXKEditor
{
    using namespace BDXKEngine;

    class SceneHUD : public Component, public PostRenderHandler, TransformEditor, GL
    {
    private:
        ObjectPtr<ObjectSwitchable> focus;
        int frameRate = 0;

        float ShowGameObjectInfo(ObjectPtr<GameObject> target, float drawY, int order = 0)
        {
            auto drawX = static_cast<float>(10 + order * 30);
            //显示GameObject
            {
                Rect rect = {drawX, drawY, 160, 25};
                GUI::TextArea(rect, target->GetName());
                if (Event::IsFocus(rect))
                    focus = target;
            }
            //添加Transform事件
            const ObjectPtr<Transform> transform = target->GetTransform();
            {
                Rect transformRect = {drawX + 165, drawY, 160, 20};
                //使其可拖拽
                if (Event::IsDrag(transformRect, transform))
                {
                    GL2D::DrawRectangleCenter(Input::GetMousePosition(), transformRect.GetSize(), false);
                }
                //使其可拖入
                ObjectPtr<Transform> otherTransform = nullptr;
                if (Event::IsDrop(transformRect, &otherTransform))
                {
                    if (otherTransform != nullptr)
                    {
                        try
                        {
                            otherTransform->SetParent(transform);
                            Event::Use();
                        }
                        catch (std::exception& e)
                        {
                            Debug::Log(e);
                        }
                    }
                }
            }
            //显示Components
            std::vector<ObjectPtr<Component>> components = target->GetComponents();
            for (ObjectPtr<Component> component : components)
            {
                drawX += 165;
                Rect rect = {drawX, drawY, 160, 20};

                Component* componentPtr = component.ToObject();
                String fullName = typeid(*componentPtr).name();
                GUI::TextArea(rect, fullName.substr(fullName.find(L':') + 2), 15);

                if (Event::IsFocus(rect))focus = component;
            }
            //显示子物体
            if (transform->GetChildCount() != 0)
            {
                drawY += 30;
                for (int index = 0; index < transform->GetChildCount(); index++)
                {
                    drawY = ShowGameObjectInfo(transform->GetChild(index)->GetGameObject(), drawY, order + 1);
                }
                return drawY;
            }

            return drawY + 30;
        }

        void OnPostRender() override
        {
            {
                static bool show_demo_window = true;
                static bool show_another_window = false;
                static float f = 0.0f;
                static int counter = 0;

                ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

                ImGui::Text("This is some useful text."); // Display some text (you can use a format strings too)
                ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
                ImGui::Checkbox("Another Window", &show_another_window);

                ImGui::SliderFloat("float", &f, 0.0f, 1.0f); // Edit 1 float using a slider from 0.0f to 1.0f

                if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
                    counter++;
                ImGui::SameLine();
                ImGui::Text("counter = %d", counter);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }

            return;
            
            //显示所有游戏物体
            float drawY = 10;
            std::vector<ObjectPtr<Transform>> rootTransforms = TransformEditor::GetRootTransforms();
            for (ObjectPtr<Transform> transform : rootTransforms)
            {
                drawY = ShowGameObjectInfo(transform->GetGameObject(), drawY);
            }

            //显示描述
            const Vector2 screenSize = Screen::GetSize();
            Rect rect = {};
            rect.SetSize(Vector2(screenSize.x / 4, screenSize.y / 2));
            rect.SetPosition({screenSize.x - rect.width - 10, 10});
            if (focus.IsNotNull())GUI::TextArea(rect, focus->ToString());
            else GUI::TextArea(rect, L"");

            rect.y += rect.height + 10;
            rect.height = 25;

            //显示帧率
            GUI::TextArea(rect, L"帧率:" + std::to_wstring(frameRate = (int)std::lerp(frameRate, 1 / Time::GetDeltaTime(), 0.1f)));
            rect.y += rect.height + 10;

            //孤儿箱:用来将节点父亲设为空
            GUI::TextArea(rect, L"孤儿箱");
            ObjectPtr<Transform> transform = nullptr;
            if (Event::IsDrop(rect, reinterpret_cast<ObjectPtr<Component>*>(&transform)))
            {
                if (transform != nullptr)
                {
                    transform->SetParent(nullptr);
                }
            }
            rect.y += rect.height + 10;

            if (focus.IsNotNull() && GUI::Button(rect, L"切换启用状态"))
            {
                focus->SetIsEnabling(!focus->GetIsEnabling());
            }
            rect.y += rect.height + 10;
        }
    };
}