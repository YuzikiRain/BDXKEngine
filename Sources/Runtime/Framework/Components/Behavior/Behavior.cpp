﻿#include "Behavior.h"
#include "BehaviorManager.h"

namespace BDXKEngine
{
    void Behavior::Awake()
    {
        startHandler = dynamic_cast<StartHandler*>(this);
        updateHandler = dynamic_cast<UpdateHandler*>(this);
        lateUpdateHandler = dynamic_cast<LateUpdateHandler*>(this);

        Component::Awake();
    }
    void Behavior::Destroy()
    {
        if (isAwakened)
        {
            if (const auto handler = dynamic_cast<DestroyHandler*>(this); handler != nullptr)
                handler->OnDestroy();
        }

        Component::Destroy();
    }

    void Behavior::Enable()
    {
        Component::Enable();

        if (isAwakened == false)
        {
            if (const auto handler = dynamic_cast<AwakeHandler*>(this); handler != nullptr)
                handler->OnAwake();
            isAwakened = true;
        }

        if (startHandler != nullptr)
        {
            BehaviorManager::allStartHandlers[startHandler] = true;
            startHandler = nullptr;
        }
        if (updateHandler != nullptr) BehaviorManager::allUpdateHandlers[updateHandler] = true;
        if (lateUpdateHandler != nullptr) BehaviorManager::allLateUpdateHandlers[lateUpdateHandler] = true;

        if (const auto handler = dynamic_cast<EnableHandler*>(this); handler != nullptr)
            handler->OnEnable();
    }

    void Behavior::Disable()
    {
        if (const auto handler = dynamic_cast<DisableHandler*>(this); handler != nullptr)
            handler->OnDisable();

        if (updateHandler != nullptr) BehaviorManager::allUpdateHandlers[updateHandler] = false;
        if (lateUpdateHandler != nullptr) BehaviorManager::allLateUpdateHandlers[lateUpdateHandler] = false;

        Component::Disable();
    }
}
