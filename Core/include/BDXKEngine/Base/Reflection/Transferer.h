﻿#pragma once
#include <functional>
#include <unordered_map>
#include "Type.h"

namespace BDXKEngine
{
    //只专注于传送功能，能传送各种东西
    class Transferer
    {
    public:
#define TransferFieldNestedInfo(fieldName) transferer.TransferFieldNested(#fieldName, fieldName)
#define TransferFieldInfo(fieldName) transferer.TransferField(#fieldName, fieldName)
#define TransferFieldInfoOf(fieldName,transferType) transferer.TransferFieldOf<transferType>(#fieldName, fieldName)
        //下方这些宏和反射功能不兼容，不建议使用，请尝试使用IsInput()来实现类似功能
        // #define TransferProperty(propertyName) auto (propertyName) = Get##propertyName();\
        //                 transferer.TransferField(#propertyName,propertyName);\
        //                 Set##propertyName(propertyName)
        // #define TransferPropertyReadOnly(propertyName) auto (propertyName) = Get##propertyName();\
        // transferer.TransferField(#propertyName,propertyName)


        virtual ~Transferer() = default;

        std::function<void(void*, const Type&)> GetTransferFunc(const Type& type);
        int GetTypeSize(const Type& type);
        bool IsContainTransferFunc(const Type& type) const;
        template <typename TValue>
        void SetTransferFunc(std::function<void(TValue& value)> func)
        {
            const Type type = GetTypeOf<TValue>();
            transferFuncs[type] = [=](void* value, const Type&)
            {
                TValue* target = static_cast<TValue*>(value);
                func(*target);
            };
            typeSizes[type] = sizeof(TValue);
        }

        virtual void PushPath(const std::string& key);
        virtual void PopPath(std::string& key);

        void TransferField(std::string key, void* value, const Type& type);
        template <typename TTransfer, typename TField>
        void TransferFieldOf(std::string key, TField& value)
        {
            PushPath(key);

            TransferValueOf<TTransfer>(value);

            PopPath(key);
        }
        template <typename TField>
        void TransferField(std::string key, TField& value)
        {
            PushPath(key);

            TransferValue(value);

            PopPath(key);
        }
        template <typename TField>
        void TransferFieldNested(std::string key, TField& value)
        {
            PushPath(key);
            TransferValueNested(value);
            PopPath(key);
        }

        virtual void TransferValue(void* value, const Type& type);
        template <typename TTransfer, typename TSource>
        void TransferValueOf(TSource& value)
        {
            TransferValue(&value, GetTypeOf<TTransfer>());
        }
        template <typename TSource>
        void TransferValue(TSource& value)
        {
            TransferValueOf<TSource>(value);
        }
        template <typename TField>
        void TransferValueNested(TField& value)
        {
            value.Transfer(*this);
        }
    protected:
        virtual std::function<void(void*, const Type&)> GetTransferFuncFallback(const Type& type);
        virtual int GetTypeSizeFallback(const Type& type);
        virtual void TransferValueFallback(void* value, const Type& type);
    private:
        std::unordered_map<Type, std::function<void(void*, const Type&)>> transferFuncs;
        std::unordered_map<Type, int> typeSizes;
    };

    class TransferFunc
    {
    public:
        template <typename TTransferer, typename TValue>
        TransferFunc(TTransferer* transferer, std::function<void(TValue&)> func)
        {
            transferer->template SetTransferFunc<TValue>(func);
        }
    };

#define Internal_CustomTransferFunc(Type,FuncName,ID) TransferFunc CustomTransferFunc##FuncName##ID = {this,std::function([this](Type##& value){FuncName(value);})};
#define Internal_CustomTransferFunc2(Type,FuncName,ID) Internal_CustomTransferFunc(Type,FuncName,ID)
#define CustomTransferFunc(Type,FuncName) Internal_CustomTransferFunc2(Type,FuncName,__LINE__)
}
