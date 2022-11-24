﻿#include "GameObject.h"
#include "BDXKEngine/Base/Extension/Vector.h"
#include "BDXKEngine/Platform/Resources/Resources.h"
#include "Component.h"

namespace BDXKEngine
{
    bool GameObject::GetIsActivating() const
    {
        if (parent != nullptr)
            return parent->IsActivatingAndEnabling() && ScriptableObject::GetIsActivating();
        return ScriptableObject::GetIsActivating();
    }

    ObjectPtr<GameObject> GameObject::GetParent()
    {
        return parent;
    }
    ObjectPtr<GameObject> GameObject::GetChild(int index)
    {
        return children[index];
    }
    int GameObject::GetChildCount() const
    {
        return static_cast<int>(children.size());
    }
    std::vector<ObjectPtr<Component>> GameObject::GetComponents() const
    {
        return components;
    }

    Vector3 GameObject::GetPosition() const
    {
        return position;
    }
    Vector3 GameObject::GetEulerAngles() const
    {
        return eulerAngles;
    }
    Vector3 GameObject::GetScale() const
    {
        return scale;
    }
    Vector3 GameObject::GetLocalPosition() const
    {
        return localPosition;
    }
    Vector3 GameObject::GetLocalEulerAngles() const
    {
        return localEulerAngles;
    }
    Vector3 GameObject::GetLocalScale() const
    {
        return localScale;
    }
    Matrix4x4 GameObject::GetLocalToWorldMatrix() const
    {
        return localToWorldMatrix;
    }
    Matrix4x4 GameObject::GetWorldToLocalMatrix() const
    {
        return worldToLocalMatrix;
    }
    Vector3 GameObject::GetRight() const
    {
        return localToWorldMatrix.MultiplyVector(Vector3::right);
    }
    Vector3 GameObject::GetUp() const
    {
        return localToWorldMatrix.MultiplyVector(Vector3::up);
    }
    Vector3 GameObject::GetFront() const
    {
        return localToWorldMatrix.MultiplyVector(Vector3::front);
    }

    void GameObject::SetParent(const ObjectPtr<GameObject>& newparent)
    {
        //嵌套检查
        if (newparent.IsNull() == false)
        {
            const ObjectPtr current = this;
            ObjectPtr<GameObject> newUpLayer = newparent;
            do
            {
                if (newUpLayer == current)
                    throw std::exception("你在试图让一个父物体或自身成为其的孩子，这会导致嵌套循环，是不允许的。");
                newUpLayer = newUpLayer->parent;
            }
            while (newUpLayer.IsNull() == false);
        }

        const bool oldActivating = GetIsActivating();

        if (parent.IsNull() == false)
        {
            parent->children.erase(std::find(
                parent->children.begin(),
                parent->children.end(),
                ObjectPtr<GameObject>(this)
            ));
        }
        if (newparent.IsNull() == false)newparent->children.emplace_back(this);
        this->parent = newparent;

        RenewScale();
        RenewEulerAngles();
        RenewPositionAndMatrix();

        if (oldActivating != newparent->GetIsActivating())
            SetIsActivating(!oldActivating);
    }
    void GameObject::SetLocalPosition(Vector3 value)
    {
        localPosition = value;
        RenewPositionAndMatrix();
    }
    void GameObject::SetLocalEulerAngles(Vector3 value)
    {
        value.x = static_cast<float>(std::fmod(value.x, 360));
        value.y = static_cast<float>(std::fmod(value.y, 360));
        value.z = static_cast<float>(std::fmod(value.z, 360));
        localEulerAngles = value;
        RenewEulerAngles();
        RenewPositionAndMatrix();
    }
    void GameObject::SetLocalScale(Vector3 value)
    {
        localScale = value;
        RenewScale();
        RenewPositionAndMatrix();
    }

    void GameObject::RenewPositionAndMatrix(bool renewChild)
    {
        position = parent.IsNull() ? localPosition : parent->GetLocalToWorldMatrix().MultiplyPoint(localPosition);
        const Matrix4x4 parentLocalToWorldMatrix = parent.IsNull() ? Matrix4x4::identity : parent->GetLocalToWorldMatrix();

        localToWorldMatrix = parentLocalToWorldMatrix * Matrix4x4::TRS(localPosition, localEulerAngles, localScale);
        worldToLocalMatrix = localToWorldMatrix.GetInverse();

        if (renewChild)
            for (const ObjectPtr<GameObject>& child : children)
                child->RenewPositionAndMatrix();
    }
    void GameObject::RenewEulerAngles(bool renewChild)
    {
        const Vector3 parentalEulerAngles = parent.IsNull() ? Vector3::zero : parent->GetEulerAngles();

        eulerAngles.x = static_cast<float>(std::fmod(parentalEulerAngles.x + localEulerAngles.x, 360));
        eulerAngles.y = static_cast<float>(std::fmod(parentalEulerAngles.y + localEulerAngles.y, 360));
        eulerAngles.z = static_cast<float>(std::fmod(parentalEulerAngles.z + localEulerAngles.z, 360));

        if (renewChild)
            for (const ObjectPtr<GameObject>& child : children)
                child->RenewEulerAngles();
    }
    void GameObject::RenewScale(bool renewChild)
    {
        const Vector3 parentalScale = parent.IsNull() ? Vector3::one : parent->GetScale();

        scale.x = parentalScale.x * localScale.x;
        scale.y = parentalScale.y * localScale.y;
        scale.z = parentalScale.z * localScale.z;

        if (renewChild)
            for (const ObjectPtr<GameObject>& child : children)
                child->RenewScale();
    }

    void GameObject::AddComponent(const ObjectPtr<Component>& component)
    {
        if (component->GetGameObject() != this)throw std::exception("目标组件未绑定到该物体");

        const auto target = std::find(components.begin(), components.end(), component);
        if (target != components.end())throw std::exception("对应组件已添加");
        components.emplace_back(component);
    }
    void GameObject::RemoveComponent(const ObjectPtr<Component>& component)
    {
        if (component->GetGameObject() != this)throw std::exception("目标组件未绑定到该物体");

        const auto target = std::find(components.begin(), components.end(), component);
        if (target == components.end())throw std::exception("未查询到对应组件");
        components.erase(target);
    }

    void GameObject::Transfer(Transferer& transferer)
    {
        ScriptableObject::Transfer(transferer);

        TransferFieldInfo(localPosition);
        TransferFieldInfo(localEulerAngles);
        TransferFieldInfo(localScale);
        TransferFieldInfo(parent);
        TransferFieldInfo(children);
        TransferFieldInfo(components);
    }
    void GameObject::Awake()
    {
        ScriptableObject::Awake();

        RenewScale(false);
        RenewEulerAngles(false);
        RenewPositionAndMatrix(false);
        
        const Reflection gameObjectReflection = Reflection::GetReflection(GetTypeIDOf<GameObject>());
        for (ObjectPtr<GameObject>& child : children)
        {
            if (child->GetParent() != this)
            {
                child = Resources::Clone<GameObject>(child, false);
                gameObjectReflection.GetFieldOf<ObjectPtr<GameObject>>(child.ToObjectBase(), "parent") = this;
                Instantiate(child);
            }
        }

        const Reflection componentReflection = Reflection::GetReflection(GetTypeIDOf<Component>());
        for (ObjectPtr<Component>& component : components)
        {
            if (component->GetGameObject() != this)
            {
                component = Resources::Clone<Component>(component, false);
                componentReflection.GetFieldOf<ObjectPtr<GameObject>>(component.ToObjectBase(), "gameObject") = this;
                Instantiate(component);
            }
        }
    }
    void GameObject::Destroy()
    {
        for (const ObjectPtr<GameObject>& child : children)
            DestroyImmediate(child);
        children.clear();
        for (const ObjectPtr<Component>& component : components)
            DestroyImmediate(component);
        components.clear();

        ScriptableObject::Destroy();
    }
    void GameObject::Enable()
    {
        ScriptableObject::Enable();

        for (const ObjectPtr<GameObject>& child : children)
            child->SetIsActivating(true);
        for (const ObjectPtr<Component>& component : components)
            component->SetIsActivating(true);
    }
    void GameObject::Disable()
    {
        for (const ObjectPtr<GameObject>& child : children)
            child->SetIsActivating(false);
        for (const ObjectPtr<Component>& component : components)
            component->SetIsActivating(false);

        ScriptableObject::Disable();
    }
}