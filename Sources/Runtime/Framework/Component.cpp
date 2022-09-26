﻿#include "Component.h"
#include "GameObject.h"


namespace BDXKEngine
{
    ObjectPtr<GameObject> Component::GetGameObject() { return gameObject; }
    ObjectPtr<Transform> Component::GetTransform() { return GetGameObject()->GetComponent<Transform>(); }

    void Component::Export(Exporter& exporter)
    {
        Object::Export(exporter);

        exporter.TransferObject(gameObject);
    }

    void Component::Import(Importer& importer)
    {
        Object::Import(importer);

        gameObject = importer.TransferObject();
    }

    void Component::Awake()
    {
        Object::Awake();

        gameObject->components.push_back(this);
    }

    void Component::Destroy()
    {
        gameObject->components.erase(std::find(gameObject->components.begin(), gameObject->components.end(),
                                               ObjectPtr<Component>{this}));

        Object::Destroy();
    }

    void Component::SetEnabling(bool state)
    {
        if (gameObject->GetIsEnabling() == false)
            Object::SetEnabling(state);
    }
}
