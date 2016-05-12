#include "Stage.h"
#include "INCLUDES.h"
#include "Director.h"
#include "GyuDon.h"

Stage::Stage()
{
}


Stage::~Stage()
{
}

Stage::Common::Common(){
	CreateTexture();
	CreateShader();
	CreateVertex();
}

Stage::Common::~Common(){
	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVSConstantBuffer);
	SAFE_RELEASE(m_pPSConstantBuffer);
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
	SAFE_RELEASE(m_pSamplerState);
	SAFE_RELEASE(m_pTexture);
}

void Stage::Common::Draw(){

	ID3D11Device* device = Director::instance()->framework()->device();
	ID3D11DeviceContext* deviceContext = Director::instance()->framework()->deviceContext();
	const D3DXMATRIX& projMat = Director::instance()->framework()->projMat();
	const D3DXMATRIX& viewMat = Director::instance()->framework()->viewMat();
	// 
	D3DXMATRIX mWorld;
	D3DXMATRIX rota;
	D3DXMATRIX scale;
	D3DXMATRIX trance;
	D3DXMatrixTranslation(&trance, 1.f, 5.f, 1.f);
	D3DXMatrixScaling(&scale, 3.f, 3.f, 3.f);
	D3DXMatrixRotationY(&rota, timeGetTime() / 1000.0f);
	mWorld = scale * rota * trance;

	float val = timeGetTime() / 1000.0f;
	const D3DXVECTOR3& vEyePt = Director::instance()->framework()->vEyePt();

	deviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pPixelShader, NULL, 0);

	deviceContext->IASetInputLayout(m_pVertexLayout);
	deviceContext->PSSetSamplers(0, 1, &m_pSamplerState);
	// 普通のテクスチャ
	deviceContext->PSSetShaderResources(0, 1, &m_pTexture);

	//ブレンディングをコンテキストに設定
	float blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };
	deviceContext->OMSetBlendState(NULL, blendFactor, 0xffffffff);

	// 刀身
	{
		// 2016-03-27 pmx 描画
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		UINT stride = sizeof(SimpleVertex);
		UINT offset = 0;
		deviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
		deviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		D3D11_MAPPED_SUBRESOURCE pData;
		CONSTANT cb;
		// バーテクス・ピクセル
		if (SUCCEEDED(deviceContext->Map(m_pVSConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			cb.g_mW = mWorld;
			D3DXMatrixTranspose(&cb.g_mW, &cb.g_mW);
			cb.g_mWVP = mWorld * viewMat * projMat;
			D3DXMatrixTranspose(&cb.g_mWVP, &cb.g_mWVP);
			cb.g_vEye.x = Director::instance()->framework()->vEyePt().x;
			cb.g_vEye.y = Director::instance()->framework()->vEyePt().y;
			cb.g_vEye.z = Director::instance()->framework()->vEyePt().z;
			cb.g_vEye.w = 1.f;
			cb.g_vLightDir.x = Director::instance()->framework()->getDirectionLight().x;
			cb.g_vLightDir.y = Director::instance()->framework()->getDirectionLight().y;
			cb.g_vLightDir.z = Director::instance()->framework()->getDirectionLight().z;
			cb.g_vLightDir.w = 0.f;
			cb.g_Diffuse = D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);

			memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(CONSTANT));
			deviceContext->Unmap(m_pVSConstantBuffer, 0);
		}
		deviceContext->VSSetConstantBuffers(0, 1, &m_pVSConstantBuffer);
		deviceContext->PSSetConstantBuffers(0, 1, &m_pVSConstantBuffer);

		deviceContext->DrawIndexed(m_file.getVertexIndex().size(), 0, 0);
	}

}

void Stage::Draw(){

	m_common.Draw();
}

void Stage::Common::CreateVertex(){


	m_file.Load("dat/obj/stage", "stage");
	ID3D11Device* device = Director::instance()->framework()->device();
	ID3D11DeviceContext* deviceContext = Director::instance()->framework()->deviceContext();

	// vertex
	{
		SimpleVertex* vertices = new SimpleVertex[m_file.vertexSize()];
		for (int i = 0; i < m_file.vertexSize(); i++){
			vertices[i].position.x = m_file.getPosition()[i].x;
			vertices[i].position.y = m_file.getPosition()[i].y;
			vertices[i].position.z = m_file.getPosition()[i].z;
			vertices[i].position.w = 1.f;
			vertices[i].normal.x = m_file.getNormal()[i].x;
			vertices[i].normal.y = m_file.getNormal()[i].y;
			vertices[i].normal.z = m_file.getNormal()[i].z;
			vertices[i].uv.x = m_file.getUV()[i].x;
			vertices[i].uv.y = m_file.getUV()[i].y;
		}

		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(SimpleVertex) * m_file.vertexSize();
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


	// index
	{
		unsigned* indecies = new unsigned[m_file.vertexIndexSize()];
		for (int i = 0; i < m_file.vertexIndexSize(); i++){
			indecies[i] = m_file.getVertexIndex()[i];
		}
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned) * m_file.vertexIndexSize();
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
}

void Stage::Common::CreateTexture(){

	ID3D11Device* device = Director::instance()->framework()->device();
	ID3D11DeviceContext* deviceContext = Director::instance()->framework()->deviceContext();

	{
		// tex
		char name[] = "dat/tex/dot.png";
		HRESULT hr;
		if (FAILED(hr = D3DX11CreateShaderResourceViewFromFile(device, name, NULL, NULL, &m_pTexture, NULL)))
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
}

void Stage::Common::CreateShader(){

	ID3DBlob *pCompiledShader = NULL;
	ID3DBlob *pErrors = NULL;

	ID3D11Device* device = Director::instance()->framework()->device();
	ID3D11DeviceContext* deviceContext = Director::instance()->framework()->deviceContext();

	if (FAILED(D3DX11CompileFromFile("dat/shader/SimpleFong.hlsl", NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
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
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	if (FAILED(device->CreateInputLayout(layout, numElements,
		pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout)))
	{
		HALT(h);
	}

	pCompiledShader->Release();
	if (FAILED(D3DX11CompileFromFile("dat/shader/SimpleFong.hlsl", NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
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
		if (FAILED(hr = device->CreateBuffer(&cb, NULL, &m_pVSConstantBuffer)))
		{
			HALT(h);
		}
	}

	//{
	//	int csize = sizeof(CONSTANT_BUFFER_TOUSHIN);
	//	int amari = csize % 16;
	//	if (amari != 0){
	//		csize += 16 - amari;
	//	}
	//	D3D11_BUFFER_DESC cb;
	//	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//	cb.ByteWidth = csize;
	//	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//	cb.MiscFlags = 0;
	//	cb.StructureByteStride = 0;
	//	cb.Usage = D3D11_USAGE_DYNAMIC;
	//	HRESULT hr;
	//	if (FAILED(hr = device->CreateBuffer(&cb, NULL, &m_pPSConstantBuffer)))
	//	{
	//		HALT(h);
	//	}
	//}
}