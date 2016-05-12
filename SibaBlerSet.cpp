#include "Siba.h"
#include "INCLUDES.h"
#include "Director.h"
#include "GyuDon.h"

void Siba::BlerSet(){
	ID3D11Device* device = Director::instance()->framework()->device();
	ID3D11DeviceContext* deviceContext = Director::instance()->framework()->deviceContext();

	Bler& t = m_bler;

	ID3DBlob *pCompiledShader = NULL;
	ID3DBlob *pErrors = NULL;

	if (FAILED(D3DX11CompileFromFile("SibaBler.hlsl", NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, "hlsl読み込み失敗", NULL, MB_OK);
		HALT(h);
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(device->CreateVertexShader(pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(), NULL, &t.m_pVertexShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, "バーテックスシェーダー作成失敗", NULL, MB_OK);
		HALT(h);
	}

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	if (FAILED(device->CreateInputLayout(layout, numElements,
		pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &t.m_pVertexLayout)))
	{
		HALT(h);
	}

	if (FAILED(D3DX11CompileFromFile("SibaBler.hlsl", NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, "hlsl読み込み失敗", NULL, MB_OK);
		HALT(h);
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(device->CreatePixelShader(pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(), NULL, &t.m_pPixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, "ピクセルシェーダー作成失敗", NULL, MB_OK);
		HALT(h);
	}
	SAFE_RELEASE(pCompiledShader);

	{
		int csize = sizeof(BlerConstant);
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
		if (FAILED(hr = device->CreateBuffer(&cb, NULL, &t.m_pConstantBuffer)))
		{
			HALT(h);
		}
	}


	//{
	//	HRESULT hr;
	//	if (FAILED(hr = D3DX11CreateShaderResourceViewFromFile(device, "dat/tex/siba.png", NULL, NULL, &t.m_pTexture, NULL)))
	//	{
	//		HALT(h);
	//	}

	//	// sampler
	//	D3D11_SAMPLER_DESC samDesc;
	//	ZeroMemory(&samDesc, sizeof(samDesc));
	//	samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//	samDesc.AddressU = samDesc.AddressV = samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	//	samDesc.MaxAnisotropy = 1;
	//	samDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	//	samDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//	device->CreateSamplerState(&samDesc, &t.m_pSamplerState);
	//}


	{
		BlerVertex* vertices = new BlerVertex[4];
		vertices[0].Pos = D3DXVECTOR4(0.f, 0.f, 0.f, 1.f);
		vertices[1].Pos = D3DXVECTOR4(WINDOW_WIDTH, 0.f, 0.f, 1.f);
		vertices[2].Pos = D3DXVECTOR4(0.f, WINDOW_HEIGHT, 0.f, 1.f);
		vertices[3].Pos = D3DXVECTOR4(WINDOW_WIDTH, WINDOW_HEIGHT, 0.f, 1.f);

		D3DXMATRIX projMat;
		D3DXMatrixOrthoOffCenterLH(&projMat, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, -1, 1);
		for (int i = 0; i < 4; i++){
			D3DXVec3TransformCoord(reinterpret_cast<D3DXVECTOR3*>(&vertices[i].Pos), reinterpret_cast<D3DXVECTOR3*>(&vertices[i].Pos), &projMat);
		}

		vertices[0].Tex = D3DXVECTOR2(0.f, 0.f);
		vertices[1].Tex = D3DXVECTOR2(1.f, 0.f);
		vertices[2].Tex = D3DXVECTOR2(0.f, 1.f);
		vertices[3].Tex = D3DXVECTOR2(1.f, 1.f);

		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(BlerVertex) * 4;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = vertices;

		if (FAILED(device->CreateBuffer(&bd, &InitData, &t.m_pVertexBuffer)))
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

		if (FAILED(device->CreateBuffer(&bd, &InitData, &t.m_pIndexBuffer)))
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
	device->CreateBlendState(&BlendStateDesc, &t.m_pBlendState);


	// bler
	for (int i = 0; i < 2; i++)
	{
		D3D11_TEXTURE2D_DESC desc;
		memset(&desc, 0, sizeof(desc));
		desc.Width = WINDOW_WIDTH;
		desc.Height = WINDOW_HEIGHT;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		// RGBA(255,255,255,255)タイプ
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;


		ID3D11Texture2D* tex2D;   /// 2Ｄテクスチャ
		HRESULT hr = device->CreateTexture2D(&desc, 0, &tex2D);
		ASSERT(SUCCEEDED(hr));

		// フォント情報をテクスチャに書き込む部分
		//D3D11_MAPPED_SUBRESOURCE hMappedResource;
		//hr = deviceContext->Map(
		//	tex2D,
		//	0,
		//	D3D11_MAP_WRITE_DISCARD,
		//	0,
		//	&hMappedResource);
		//if (FAILED(hr)){
		//	HALT(h);
		//}

		// ShaderResourceViewの情報を作成する
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = desc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = desc.MipLevels;

		// ShaderResourceViewを作成する
		hr = device->CreateShaderResourceView(tex2D, &srvDesc, &t.m_pSurface[i]);
		ASSERT(SUCCEEDED(hr));
		// RenderTargetView Create.
		hr = device->CreateRenderTargetView(tex2D, NULL, &t.m_pRenderTarget[i]);
		ASSERT(SUCCEEDED(hr));
	}

	if (FAILED(D3DX11CompileFromFile("Simple.hlsl", NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, "hlsl読み込み失敗", NULL, MB_OK);
		HALT(h);
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(device->CreatePixelShader(pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(), NULL, &t.m_pSimplePixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, "ピクセルシェーダー作成失敗", NULL, MB_OK);
		HALT(h);
	}
	SAFE_RELEASE(pCompiledShader);

}