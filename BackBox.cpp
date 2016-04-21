#include "DebugDrawer.h"
#include "INCLUDES.h"
#include "Director.h"
#include "GyuDon.h"
#include <D3D11.h>
#include <D3DX10.h>
#include <D3DX11.h>

namespace{
	struct CONSTANT{
		D3DXMATRIX mWVP;
	};
	struct Vertex{
		D3DXVECTOR4 Pos;
	};
}

void DebugDrawer::SetBackBox(){

	{
		SAFE_RELEASE(m_backBox.m_pVertexShader);
		SAFE_RELEASE(m_backBox.m_pPixelShader);
		SAFE_RELEASE(m_backBox.m_pConstantBuffer);
		SAFE_RELEASE(m_backBox.m_pIndexBuffer);
		SAFE_RELEASE(m_backBox.m_pVertexBuffer);
		SAFE_RELEASE(m_backBox.m_pVertexLayout);
	}

	ID3D11Device* device = Director::instance()->framework()->device();
	ID3D11DeviceContext* deviceContext = Director::instance()->framework()->deviceContext();

	ID3DBlob *pCompiledShader = NULL;
	ID3DBlob *pErrors = NULL;

	if (FAILED(D3DX11CompileFromFile("blackBoxHarf.hlsl", NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, "hlsl読み込み失敗", NULL, MB_OK);
		HALT(h);
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(device->CreateVertexShader(pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(), NULL, &m_backBox.m_pVertexShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, "バーテックスシェーダー作成失敗", NULL, MB_OK);
		HALT(h);
	}

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	if (FAILED(device->CreateInputLayout(layout, numElements,
		pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_backBox.m_pVertexLayout)))
	{
		HALT(h);
	}

	if (FAILED(D3DX11CompileFromFile("blackBoxHarf.hlsl", NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, "hlsl読み込み失敗", NULL, MB_OK);
		HALT(h);
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(device->CreatePixelShader(pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(), NULL, &m_backBox.m_pPixelShader)))
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
		if (FAILED(hr = device->CreateBuffer(&cb, NULL, &m_backBox.m_pConstantBuffer)))
		{
			HALT(h);
		}
	}


	//{
	//	HRESULT hr;
	//	std::ostringstream oss;
	//	oss << "dat/tex/font-menlo-" << w << "x" << h << ".png";
	//	if (FAILED(hr = D3DX11CreateShaderResourceViewFromFile(device, oss.str().c_str(), NULL, NULL, &m_pTexture, NULL)))
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

	//	device->CreateSamplerState(&samDesc, &m_pSamplerState);
	//}


	{
		Vertex* vertices = new Vertex[4];
		vertices[0].Pos = D3DXVECTOR4(0.f, 0.f, 0.f, 1.f);
		vertices[1].Pos = D3DXVECTOR4(1, 0.f, 0.f, 1.f);
		vertices[2].Pos = D3DXVECTOR4(0.f, 1, 0.f, 1.f);
		vertices[3].Pos = D3DXVECTOR4(1, 1, 0.f, 1.f);

		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(Vertex) * 4;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = vertices;

		if (FAILED(device->CreateBuffer(&bd, &InitData, &m_backBox.m_pVertexBuffer)))
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

		if (FAILED(device->CreateBuffer(&bd, &InitData, &m_backBox.m_pIndexBuffer)))
		{
			delete[] indecies;
			HALT(h);
		}
		delete[] indecies;

	}

	//D3D11_BLEND_DESC BlendStateDesc;
	//BlendStateDesc.AlphaToCoverageEnable = FALSE;
	//BlendStateDesc.IndependentBlendEnable = FALSE;
	//for (int i = 0; i < 8; i++){
	//	BlendStateDesc.RenderTarget[i].BlendEnable = TRUE;
	//	BlendStateDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	//	BlendStateDesc.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	//	BlendStateDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
	//	BlendStateDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
	//	BlendStateDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
	//	BlendStateDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	//	BlendStateDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	//}
	//device->CreateBlendState(&BlendStateDesc, &m_hpBlendState);
}
void DebugDrawer::ReleaseBackBox(){
	SAFE_RELEASE(m_backBox.m_pVertexShader);
	SAFE_RELEASE(m_backBox.m_pPixelShader);
	SAFE_RELEASE(m_backBox.m_pConstantBuffer);
	SAFE_RELEASE(m_backBox.m_pIndexBuffer);
	SAFE_RELEASE(m_backBox.m_pVertexBuffer);
	SAFE_RELEASE(m_backBox.m_pVertexLayout);
}
void DebugDrawer::DrawBackBox(){

	int maxWidth = 0;
	int maxHeight = 0;
	for (auto it = m_drawStrings.begin(); it != m_drawStrings.end(); it++){
		if (it->second.size()*m_fontWidth > maxWidth){
			maxWidth = it->second.size()*m_fontWidth;
		}
	}
	maxHeight = m_drawStrings.size() * m_fontHeight;

	int sizeX = maxWidth;
	int sizeY = maxHeight;

	ID3D11Device* device = Director::instance()->framework()->device();
	ID3D11DeviceContext* deviceContext = Director::instance()->framework()->deviceContext();

	deviceContext->IASetInputLayout(m_backBox.m_pVertexLayout);
	deviceContext->VSSetShader(m_backBox.m_pVertexShader, 0, 0);
	deviceContext->PSSetShader(m_backBox.m_pPixelShader, NULL, 0);

	float blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };
	deviceContext->OMSetBlendState(m_hpBlendState, blendFactor, 0xffffffff);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &m_backBox.m_pVertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_backBox.m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &m_backBox.m_pConstantBuffer);
	deviceContext->PSSetConstantBuffers(0, 1, &m_backBox.m_pConstantBuffer);

	auto renderTarget = Director::instance()->framework()->renderTargetView();
	deviceContext->OMSetRenderTargets(1, &renderTarget, 0);


	D3DXMATRIX projMat;
	D3DXMatrixOrthoOffCenterLH(&projMat, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, -1, 1);
	D3DXMATRIX worldMat;
	D3DXMatrixScaling(&worldMat, (float)sizeX, (float)sizeY, 1.f);
	D3DXMATRIX mat = worldMat*projMat;
	D3DXMatrixTranspose(&mat, &mat);

	D3D11_MAPPED_SUBRESOURCE pData;
	CONSTANT cb;
	if (SUCCEEDED(deviceContext->Map(m_backBox.m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		cb.mWVP = mat;

		memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(CONSTANT));
		deviceContext->Unmap(m_backBox.m_pConstantBuffer, 0);
	}
	deviceContext->DrawIndexed(6, 0, 0);

}