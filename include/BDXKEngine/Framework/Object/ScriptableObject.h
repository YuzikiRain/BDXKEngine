﻿#pragma once
#include <unordered_set>
#include "SwitchableObject.h"

namespace BDXKEngine
{
    class ScriptableObject : public SwitchableObject
    {
    public:
        template <typename TObject>
        static std::vector<TObject*> FindScriptableObjectsOfType()
        {
            std::vector<TObject*> result{};
            for (const auto& item : allScriptableObjects)
            {
                TObject* object = dynamic_cast<TObject*>(item);
                if (object != nullptr)result.push_back(object);
            }

            return result;
        }

    protected:
        static std::unordered_set<ScriptableObject*> allScriptableObjects;

        void Enable() override;
        void Disable() override;
    };
}