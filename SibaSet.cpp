#include "Siba.h"
#include "Director.h"
#include "GyuDon.h"
#include "INCLUDES.h"


void Siba::Set(){
	ID3D11Device* device = Director::instance()->framework()->device();
	ID3D11DeviceContext* deviceContext = Director::instance()->framework()->deviceContext();

	ID3DBlob *pCompiledShader = NULL;
	ID3DBlob *pErrors = NULL;

	if (FAILED(D3DX11CompileFromFile("sibaShade.hlsl", NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, "hlsl読み込み失敗", NULL, MB_OK);
		HALT(h);
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(device->CreateVertexShader(pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(), NULL, &m_kusa.m_pVertexShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, "バーテックスシェーダー作成失敗", NULL, MB_OK);
		HALT(h);
	}

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	if (FAILED(device->CreateInputLayout(layout, numElements,
		pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_kusa.m_pVertexLayout)))
	{
		HALT(h);
	}

	if (FAILED(D3DX11CompileFromFile("sibaShade.hlsl", NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, "hlsl読み込み失敗", NULL, MB_OK);
		HALT(h);
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(device->CreatePixelShader(pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(), NULL, &m_kusa.m_pPixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, "ピクセルシェーダー作成失敗", NULL, MB_OK);
		HALT(h);
	}
	SAFE_RELEASE(pCompiledShader);

	{
		int csize = sizeof(CONSTANT);
		int amari = csize % 16;
		if (amari != 0){
			csize += 16 - amari;
		}
		D3D11_BUFFER_DESC cb;
		cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cb.ByteWidth = csize;
		cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cb.MiscFlags = 0;
		cb.StructureByteStride = 0;
		cb.Usage = D3D11_USAGE_DYNAMIC;
		HRESULT hr;
		if (FAILED(hr = device->CreateBuffer(&cb, NULL, &m_kusa.m_pConstantBuffer)))
		{
			HALT(h);
		}
	}


	{
		HRESULT hr;
		if (FAILED(hr = D3DX11CreateShaderResourceViewFromFile(device, "dat/tex/siba.png", NULL, NULL, &m_kusa.m_pTexture, NULL)))
		{
			HALT(h);
		}

		// sampler
		D3D11_SAMPLER_DESC samDesc;
		ZeroMemory(&samDesc, sizeof(samDesc));
		samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samDesc.AddressU = samDesc.AddressV = samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samDesc.MaxAnisotropy = 1;
		samDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samDesc.MaxLOD = D3D11_FLOAT32_MAX;

		device->CreateSamplerState(&samDesc, &m_kusa.m_pSamplerState);
	}


	{
		Vertex* vertices = new Vertex[4];
		vertices[0].Pos = D3DXVECTOR4(-1.f, 0.f, 1.f, 1.f);
		vertices[1].Pos = D3DXVECTOR4(1, 0.f, 1.f, 1.f);
		vertices[2].Pos = D3DXVECTOR4(-1.f, 0.f, -1.f, 1.f);
		vertices[3].Pos = D3DXVECTOR4(1.f, 0, -1.f, 1.f);
		vertices[0].UV = D3DXVECTOR2(0.f, 0.f);
		vertices[1].UV = D3DXVECTOR2(1.f, 0.f);
		vertices[2].UV = D3DXVECTOR2(0.f, 1.f);
		vertices[3].UV = D3DXVECTOR2(1.f, 1.f);
		D3DXVECTOR4 normal;
		D3DXVec4Cross(&normal, &vertices[0].Pos, &vertices[1].Pos, &vertices[2].Pos);
		vertices[0].Normal = *reinterpret_cast<D3DXVECTOR3*>(&normal);
		vertices[1].Normal = *reinterpret_cast<D3DXVECTOR3*>(&normal);
		vertices[2].Normal = *reinterpret_cast<D3DXVECTOR3*>(&normal);
		vertices[3].Normal = *reinterpret_cast<D3DXVECTOR3*>(&normal);

		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(Vertex) * 4;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = vertices;

		if (FAILED(device->CreateBuffer(&bd, &InitData, &m_kusa.m_pVertexBuffer)))
		{
			delete[] vertices;
			HALT(h);
		}
		delete[] vertices;

	}

	{
		unsigned* indecies = new unsigned[6];
		indecies[0] = 0;
		indecies[1] = 1;
		indecies[2] = 2;
		indecies[3] = 3;
		indecies[4] = 2;
		indecies[5] = 1;
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned) * 6;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = indecies;

		if (FAILED(device->CreateBuffer(&bd, &InitData, &m_kusa.m_pIndexBuffer)))
		{
			delete[] indecies;
			HALT(h);
		}
		delete[] indecies;

	}

	D3D11_BLEND_DESC BlendStateDesc;
	BlendStateDesc.AlphaToCoverageEnable = FALSE;
	BlendStateDesc.IndependentBlendEnable = FALSE;
	for (int i = 0; i < 8; i++){
		BlendStateDesc.RenderTarget[i].BlendEnable = TRUE;
		BlendStateDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		BlendStateDesc.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		BlendStateDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
		BlendStateDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
		BlendStateDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
		BlendStateDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		BlendStateDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}
	device->CreateBlendState(&BlendStateDesc, &m_kusa.m_pBlendState);

	BlerSet();
}

