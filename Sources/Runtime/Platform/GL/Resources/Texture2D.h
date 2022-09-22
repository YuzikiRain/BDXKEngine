﻿#pragma once
#include"Texture.h"
#include"Base/Color.h"

namespace BDXKEngine {
	class GL2D;
	class Texture2D :public Texture
	{
		friend GL;
		friend GL2D;
	public:
		static ObjectPtr<Texture2D> Create(unsigned int width, unsigned int height);
		static ObjectPtr<Texture2D> Create(Color color);

		unsigned int GetWidth();
		unsigned int GetHeight();
	private:
		int width;
		int height;
		std::unique_ptr<Color[]> pixels;

		CComPtr<ID3D11SamplerState> samplerState;
		D3D11_TEXTURE2D_DESC renderTextureDesc;
		CComPtr<ID3D11Texture2D> renderTexture = nullptr;
		CComPtr<ID3D11ShaderResourceView> renderTextureSRV = nullptr;
		CComPtr<ID3D11RenderTargetView> renderTextureRTV = nullptr;
		CComPtr<ID3D11Texture2D> depthTexture = nullptr;
		CComPtr<ID3D11DepthStencilView> depthTextureDSV = nullptr;

		CComPtr<ID3D11ShaderResourceView> GetResourceView();
		CComPtr<ID3D11SamplerState> GetSamplerState();

		void Export(Exporter& exporter) override;
		void Import(Importer& importer) override;
		void Awake()override;
	};
}