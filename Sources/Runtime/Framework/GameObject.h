﻿#pragma once
#include<vector>
#include "Base/Object/ObjectPtr.h"
#include "Framework/Component.h"
#include "Framework/Components/Transform.h"

namespace BDXKEngine
{
    class GameObjectManager;

    class GameObject : public ObjectSwitchable
    {
        friend Component;
        friend Transform;
        friend GameObjectManager;
    public:
        static ObjectPtr<GameObject> Create(const std::wstring& name = L"New GameObject");

        template <typename TComponent>
        ObjectPtr<TComponent> GetComponent()
        {
            for (ObjectPtr<Component>& component : components)
            {
                ObjectPtr<TComponent> target = component.ToObjectPtr<TComponent>();
                if (target.IsNull() == false)
                    return target;
            }
            return nullptr;
        }
        std::vector<ObjectPtr<Component>> GetComponents();
        ObjectPtr<Transform> GetTransform();
        bool GetIsActivating() const override;
        void SetIsEnabling(bool state) override;


        template <typename TComponent>
        ObjectPtr<TComponent> AddComponent()
        {
            //初始化Component
            TComponent source = {};
            source.gameObject = this;
            ObjectPtr<TComponent> result = Instantiate<TComponent>(&source);
            components.emplace_back(result);
            return result;
        }

        static ObjectPtr<GameObject> Find(const std::wstring& name);
    private:
        //所有物体(由GameObject负责增减)
        static std::vector<ObjectPtr<GameObject>> allGameObjects;

        //当前物体拥有的组件(由Component负责增减)
        std::vector<ObjectPtr<Component>> components;
        ObjectPtr<Transform> transform;

        void OnUpdateActivating(bool state) override;

        void Transfer(Transferrer& transferrer) override;
        void Awake() override;
        void Destroy() override;
    };
}
