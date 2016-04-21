#include "DebugDrawer.h"
#include "Director.h"
#include "GyuDon.h"
#include "INCLUDES.h"
#include <sstream>

namespace{
	struct CONSTANT{
		D3DXMATRIX mWVP; //ワールドから射影までの変換行列
		D3DXVECTOR4 Diffuse = D3DXVECTOR4(1, 0, 0, 1); //拡散反射(色）	
	};
	struct Vertex{
		D3DXVECTOR4 Pos;
		D3DXVECTOR2 UV;
	};
}

DebugDrawer::DebugDrawer()
{

}


DebugDrawer::~DebugDrawer()
{
	SAFE_RELEASE(m_hpBlendState);
	SAFE_RELEASE(m_pSamplerState);
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pConstantBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pVertexLayout);

	ReleaseBackBox();
}

void DebugDrawer::AddDebugString(const std::string& str, int order){
	m_drawStrings.insert(std::pair<int, std::string>(order, str));
}

void DebugDrawer::Set(int w, int h){

	SetBackBox();

	m_fontHeight = h;
	m_fontWidth = w;

	{
		SAFE_RELEASE(m_hpBlendState);
		SAFE_RELEASE(m_pSamplerState);
		SAFE_RELEASE(m_pTexture);
		SAFE_RELEASE(m_pVertexShader);
		SAFE_RELEASE(m_pPixelShader);
		SAFE_RELEASE(m_pConstantBuffer);
		SAFE_RELEASE(m_pIndexBuffer);
		SAFE_RELEASE(m_pVertexBuffer);
		SAFE_RELEASE(m_pVertexLayout);
	}

	ID3D11Device* device = Director::instance()->framework()->device();
	ID3D11DeviceContext* deviceContext = Director::instance()->framework()->deviceContext();

	ID3DBlob *pCompiledShader = NULL;
	ID3DBlob *pErrors = NULL;

	if (FAILED(D3DX11CompileFromFile("SimpleUIShader.hlsl", NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, "hlsl読み込み失敗", NULL, MB_OK);
		HALT(h);
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(device->CreateVertexShader(pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(), NULL, &m_pVertexShader)))
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
		pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout)))
	{
		HALT(h);
	}

	if (FAILED(D3DX11CompileFromFile("SimpleUIShader.hlsl", NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, "hlsl読み込み失敗", NULL, MB_OK);
		HALT(h);
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(device->CreatePixelShader(pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(), NULL, &m_pPixelShader)))
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
		if (FAILED(hr = device->CreateBuffer(&cb, NULL, &m_pConstantBuffer)))
		{
			HALT(h);
		}
	}


	{
		HRESULT hr;
		std::ostringstream oss;
		oss << "dat/tex/font-menlo-" << w << "x" << h << ".png";
		if (FAILED(hr = D3DX11CreateShaderResourceViewFromFile(device, oss.str().c_str(), NULL, NULL, &m_pTexture, NULL)))
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

		device->CreateSamplerState(&samDesc, &m_pSamplerState);
	}


	{
		Vertex* vertices = new Vertex[4*26*7];
		float _w = 1.f / 26.f;
		float _h = 1.f / 7.f;
		for (int x = 0; x < 26; x++){
			for (int y = 0; y < 7; y++){
				int ofs = (y * 26 + x) * 4;
				vertices[ofs + 0].Pos = D3DXVECTOR4(0.f, 0.f, 0.f, 1.f);
				vertices[ofs + 1].Pos = D3DXVECTOR4(w, 0.f, 0.f, 1.f);
				vertices[ofs + 2].Pos = D3DXVECTOR4(0.f, h, 0.f, 1.f);
				vertices[ofs + 3].Pos = D3DXVECTOR4(w, h, 0.f, 1.f);
				vertices[ofs + 0].UV = D3DXVECTOR2(x*_w, y*_h);
				vertices[ofs + 1].UV = D3DXVECTOR2(x*_w + _w, y*_h);
				vertices[ofs + 2].UV = D3DXVECTOR2(x*_w, y*_h + _h);
				vertices[ofs + 3].UV = D3DXVECTOR2(x*_w + _w, y*_h + _h);
			}
		}

		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(Vertex) * 4 * 26 * 7;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = vertices;

		if (FAILED(device->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
		{
			delete[] vertices;
			HALT(h);
		}
		delete[] vertices;

	}

	{
		unsigned* indecies = new unsigned[6*26*7];
		for (int y = 0; y < 7; y++){
			for (int x = 0; x < 26; x++){
				int ofs = (y * 26 + x) * 6;
				int ofsV = (y * 26 + x) * 4;
				indecies[ofs + 0] = ofsV + 0;
				indecies[ofs + 1] = ofsV + 1;
				indecies[ofs + 2] = ofsV + 2;
				indecies[ofs + 3] = ofsV + 3;
				indecies[ofs + 4] = ofsV + 2;
				indecies[ofs + 5] = ofsV + 1;
			}
		}
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned) * 6 * 26 * 7;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = indecies;

		if (FAILED(device->CreateBuffer(&bd, &InitData, &m_pIndexBuffer)))
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
	device->CreateBlendState(&BlendStateDesc, &m_hpBlendState);
}

void DebugDrawer::draw(){

	DrawBackBox();

	ID3D11Device* device = Director::instance()->framework()->device();
	ID3D11DeviceContext* deviceContext = Director::instance()->framework()->deviceContext();

	deviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pPixelShader, NULL, 0);

	deviceContext->IASetInputLayout(m_pVertexLayout);
	deviceContext->PSSetSamplers(0, 1, &m_pSamplerState);
	deviceContext->PSSetShaderResources(0, 1, &m_pTexture);

	//ブレンディングをコンテキストに設定
	float blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };
	deviceContext->OMSetBlendState(m_hpBlendState, blendFactor, 0xffffffff);

	D3DXMATRIX projMat;
	//D3DXMatrixOrthoLH(&projMat, WINDOW_WIDTH, WINDOW_HEIGHT, -1.f, 1.f);
	D3DXMatrixOrthoOffCenterLH(&projMat, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, -1, 1);
	//D3DXMatrix
	// 
	{
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		deviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
		deviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		//D3D11_MAPPED_SUBRESOURCE pData;
		//CONSTANT cb;
		//// バーテクス・ピクセル
		//if (SUCCEEDED(deviceContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		//{
		//	cb.Diffuse = D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);
		//	cb.mWVP = projMat;
		//	//D3DXMatrixTranspose(&cb.mWVP, &cb.mWVP);

		//	memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(CONSTANT));
		//	deviceContext->Unmap(m_pConstantBuffer, 0);
		//}
		//deviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
		//deviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
		deviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
		deviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);

		auto renderTarget = Director::instance()->framework()->renderTargetView();
		deviceContext->OMSetRenderTargets(1, &renderTarget, 0);

		//for (int x = 0; x < 26; x++){
		//	for (int y = 0; y < 7; y++){
		//		D3DXMATRIX world;
		//		D3DXMatrixTranslation(&world, 14 * x/2, 28 * y/2, 0);
		//		world *= projMat;
		//		D3DXMatrixTranspose(&world, &world);
		//		D3D11_MAPPED_SUBRESOURCE pData;
		//		CONSTANT cb;
		//		if (SUCCEEDED(deviceContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		//		{
		//			cb.Diffuse = D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);
		//			cb.mWVP = world;

		//			memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(CONSTANT));
		//			deviceContext->Unmap(m_pConstantBuffer, 0);
		//		}
		//		deviceContext->DrawIndexed(6, (y*26+x) * 6, 0);
		//	}
		//}

		int currentY = 0;
		int currentX = 0;
		for (auto it = m_drawStrings.begin(); it != m_drawStrings.end(); it++){
			currentX = 0;
			for (int i = 0; i < it->second.size(); i++){
				char c = it->second[i];
				int x = 0;
				int y = 0;
				if (c >= ' ' && c <= '/'){
					y = 0;
					x = c - ' ';
				}
				else if (c >= '0' && c <= '9'){
					y = 1;
					x = c - '0';
				}
				else if (c >= ':' && c <= '@'){
					y = 2;
					x = c - ':';
				}
				else if (c >= 'A' && c <= 'Z'){
					y = 3;
					x = c - 'A';
				}
				else if (c >= '[' && c <= '`'){
					y = 4;
					x = c - '[';
				}
				else if (c >= 'a' && c <= 'z'){
					y = 5;
					x = c - 'a';
				}
				else if (c >= '{' && c <= '~'){
					y = 6;
					x = c - '{';
				}
				else{
					continue;
				}

				D3DXMATRIX world;
				D3DXMatrixTranslation(&world, (float)currentX, (float)currentY, 0);
				world *= projMat;
				D3DXMatrixTranspose(&world, &world);
				D3D11_MAPPED_SUBRESOURCE pData;
				CONSTANT cb;
				if (SUCCEEDED(deviceContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
				{
					cb.Diffuse = D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);
					cb.mWVP = world;

					memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(CONSTANT));
					deviceContext->Unmap(m_pConstantBuffer, 0);
				}
				deviceContext->DrawIndexed(6, (y*26+x) * 6, 0);

				currentX += m_fontWidth;
			}
			currentY += m_fontHeight;
		}
		m_drawStrings.clear();

	}

}