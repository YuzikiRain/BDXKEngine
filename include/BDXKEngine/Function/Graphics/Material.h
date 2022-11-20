﻿#pragma once
#include <vector>
#include "Shader.h"
#include "Buffer.h"
#include "Texture.h"
#include "BDXKEngine/Base/Data/Mathematics/Matrix/Matrix4x4.h"


namespace BDXKEngine
{
    enum class RenderQueue
    {
        Background = 1000,
        Geometry = 2000,
        AlphaTest = 2450,
        Transparent = 3000,
        Overlay = 4000,
    };

    class Material : public Object
    {
    public:
        static ObjectPtr<Material> Create(const std::vector<ObjectPtr<Shader>>& shaders);

        std::vector<ObjectPtr<Shader>> GetShaders();
        RenderQueue GetRenderQueue() const;
        int GetPassCount() const;
        PassType GetPassType(int index) const;


        void SetShaders(const std::vector<ObjectPtr<Shader>>& shader);
        void SetRenderQueue(RenderQueue renderQueue);
        void SetFloat(int slotIndex, float value);
        void SetVector(int slotIndex, Vector4 value);
        void SetMatrix(int slotIndex, Matrix4x4 value);
        void SetTexture(int slotIndex, const ObjectPtr<Texture>& texture);
        /// 用当前材质的所有物填充渲染管线
        void SetPass(int index);
    private:
        struct Parameters : Transferable
        {
            static int GetSize()
            {
                return sizeof(Parameters) - sizeof(std::uintptr_t);
            }

            Vector4 float0_3;
            Vector4 float4_7;
            Vector4 vector0;
            Vector4 vector1;
            Vector4 vector2;
            Vector4 vector3;
            Vector4 vector4;
            Vector4 vector5;
            Matrix4x4 matrix0;
            Matrix4x4 matrix1;
            Matrix4x4 matrix2;
            Matrix4x4 matrix3;

            char* GetPtr()
            {
                return reinterpret_cast<char*>(this + std::uintptr_t{});
            }
            void Transfer(Transferer& transferer) override
            {
                TransferFieldInfo(float0_3);
                TransferFieldInfo(float4_7);
                TransferFieldInfo(vector0);
                TransferFieldInfo(vector1);
                TransferFieldInfo(vector2);
                TransferFieldInfo(vector3);
                TransferFieldInfo(vector4);
                TransferFieldInfo(vector5);
                TransferFieldInfo(matrix0);
                TransferFieldInfo(matrix1);
                TransferFieldInfo(matrix2);
                TransferFieldInfo(matrix3);
            }
        };

        // 渲染顺序
        RenderQueue renderQueue = RenderQueue::Geometry;
        Parameters parameters;
        std::vector<ObjectPtr<Shader>> shaders;
        ObjectPtr<Buffer> parametersBuffer;
        ObjectPtr<Texture> texture0;
        ObjectPtr<Texture> texture1;
        ObjectPtr<Texture> texture2;
        ObjectPtr<Texture> texture3;

        void Transfer(Transferer& transferer) override;
        void Awake() override;
    };
}