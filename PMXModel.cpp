#include "PMXModel.h"
#include "DEFINE.h"
#include "GyuDon.h"
#include "Director.h"
#include "TGATexture.h"
#include "lib\MikuMikuFormats\Pmx.h"
#include <string>

// DirectX系の解放マクロです。
#define SAFE_RELEASE(x) if (x){ x->Release(); x; }


PMXModel::PMXModel(const char* directory, const char* fileName)
{
	m_vLight = D3DXVECTOR3(0, 0.5, -1);
	D3DXVec3Normalize(&m_vLight, &m_vLight);
	initShader();
	initPolygon();
	initTexture();
}


PMXModel::~PMXModel()
{
	SAFE_RELEASE(m_pSamplerState);
	for (int i = 0; i < m_model.texture_count; i++){
		SAFE_RELEASE(m_pTexture[i]);
	}
	delete[] m_pTexture;
	SAFE_RELEASE(m_pConstantBuffer);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pIndexBuffer);
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pVertexLayout);
}


namespace{

	struct SimpleVertex
	{
		D3DXVECTOR3 Pos;
		D3DXVECTOR3 Normal;
		float Tex[2];
	};
	struct SIMPLESHADER_CONSTANT_BUFFER
	{
		D3DXMATRIX mW;
		D3DXMATRIX mWVP;
		D3DXVECTOR4 vLightDir;
		D3DXVECTOR4 vColor;
		D3DXVECTOR4 vEye;
	};

}


void PMXModel::draw(){

	ID3D11Device* device = Director::instance()->framework()->device();
	ID3D11DeviceContext* deviceContext = Director::instance()->framework()->deviceContext();
	const D3DXMATRIX& projMat = Director::instance()->framework()->projMat();
	const D3DXMATRIX& viewMat = Director::instance()->framework()->viewMat();

	D3DXMATRIX mWorld;
	D3DXMatrixRotationY(&mWorld, timeGetTime() / 1000.0f);
	const D3DXVECTOR3& vEyePt = Director::instance()->framework()->vEyePt();

	deviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	D3D11_MAPPED_SUBRESOURCE pData;
	SIMPLESHADER_CONSTANT_BUFFER cb;
	if (SUCCEEDED(deviceContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		cb.mW = mWorld;
		D3DXMatrixTranspose(&cb.mW, &cb.mW);
		D3DMATRIX m = mWorld*viewMat*projMat;
		cb.mWVP = m;
		D3DXMatrixTranspose(&cb.mWVP, &cb.mWVP);
		D3DXVECTOR4 vColor(1, 0, 0, 1);
		cb.vColor = vColor;
		cb.vLightDir = (D3DXVECTOR4)m_vLight;
		cb.vEye = D3DXVECTOR4(vEyePt.x, vEyePt.y, vEyePt.z, 0.0f);

		memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(SIMPLESHADER_CONSTANT_BUFFER));
		deviceContext->Unmap(m_pConstantBuffer, 0);
	}
	deviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	deviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	deviceContext->IASetInputLayout(m_pVertexLayout);
	deviceContext->PSSetSamplers(0, 1, &m_pSamplerState);

	{
		// 2016-03-27 pmx 描画
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		UINT stride = sizeof(SimpleVertex);
		UINT offset = 0;
		deviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
		deviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		int index = 0;
		for (int i = 0; i < m_model.material_count; i++)
		{
			if (m_model.materials[i].diffuse_texture_index == -1){
				index += m_model.materials[i].index_count;
				continue;
			}
			deviceContext->PSSetShaderResources(0, 1, &m_pTexture[m_model.materials[i].diffuse_texture_index]);
			deviceContext->DrawIndexed(m_model.materials[i].index_count, index, 0);
			index += m_model.materials[i].index_count;
			//m_model.materials[i].
		}
	}
}

HRESULT PMXModel::initShader(){

	ID3D11Device* device = Director::instance()->framework()->device();
	ID3D11DeviceContext* deviceContext = Director::instance()->framework()->deviceContext();

	ID3DBlob *pCompiledShader = NULL;
	ID3DBlob *pErrors = NULL;

	if (FAILED(D3DX11CompileFromFile("PhongShade.hlsl", NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, "hlsl読み込み失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(device->CreateVertexShader(pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(), NULL, &m_pVertexShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, "バーテックスシェーダー作成失敗", NULL, MB_OK);
		return E_FAIL;
	}

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	if (FAILED(device->CreateInputLayout(layout, numElements,
		pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout)))
	{
		return E_FAIL;
	}

	if (FAILED(D3DX11CompileFromFile("PhongShade.hlsl", NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, "hlsl読み込み失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(device->CreatePixelShader(pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(), NULL, &m_pPixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, "ピクセルシェーダー作成失敗", NULL, MB_OK);
		return E_FAIL;
	}

	SAFE_RELEASE(pCompiledShader);
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SIMPLESHADER_CONSTANT_BUFFER);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(device->CreateBuffer(&cb, NULL, &m_pConstantBuffer)))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT PMXModel::initPolygon(){
	ID3D11Device* device = Director::instance()->framework()->device();
	ID3D11DeviceContext* deviceContext = Director::instance()->framework()->deviceContext();

	// 2016-03-24 追加 pmxレンダリングテスト
	//const char *filename = "dat/pmx/tda_apemiku/Tda式初音ミク・アペンド_Ver1.00.pmx";
	const char *filename = "dat/pmx/oniko/日本鬼子0.15.pmx";
	std::ifstream stream = std::ifstream(filename, std::ios_base::binary);
	m_model.Read(&stream);

	stream.close();
	{
		SimpleVertex* vertices = new SimpleVertex[m_model.vertex_count];
		for (int i = 0; i < m_model.vertex_count; i++){
			vertices[i].Normal.x = m_model.vertices.get()[i].normal[0];
			vertices[i].Normal.y = m_model.vertices.get()[i].normal[1];
			vertices[i].Normal.z = m_model.vertices.get()[i].normal[2];
			vertices[i].Pos.x = m_model.vertices.get()[i].positon[0];
			vertices[i].Pos.y = m_model.vertices.get()[i].positon[1];
			vertices[i].Pos.z = m_model.vertices.get()[i].positon[2];
			vertices[i].Tex[0] = m_model.vertices[i].uv[0];
			vertices[i].Tex[1] = m_model.vertices[i].uv[1];
		}

		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(SimpleVertex) * m_model.vertex_count;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = vertices;

		if (FAILED(device->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
		{
			delete[] vertices;
			return E_FAIL;
		}
		delete[] vertices;

	}

	{
		unsigned* indecies = new unsigned[m_model.index_count];
		for (int i = 0; i < m_model.index_count; i++){
			indecies[i] = m_model.indices[i];
		}
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned) * m_model.index_count;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = indecies;

		if (FAILED(device->CreateBuffer(&bd, &InitData, &m_pIndexBuffer)))
		{
			delete[] indecies;
			return E_FAIL;
		}
		delete[] indecies;

	}
	return S_OK;
}

HRESULT PMXModel::initTexture(){
	ID3D11Device* device = Director::instance()->framework()->device();
	ID3D11DeviceContext* deviceContext = Director::instance()->framework()->deviceContext();
	{
		// tex
		m_pTexture = new ID3D11ShaderResourceView*[m_model.texture_count];
		for (int i = 0; i < m_model.texture_count; i++){
			char name[256];
			//if (m_model.textures[i].rfind(L".spa")){
			//	continue;
			//}
			convert2Str(name, (std::wstring(L"dat/pmx/oniko/") + m_model.textures[i]).c_str());
			if (m_model.textures[i].rfind(L".tga") != std::string::npos)
			{
				TGA tga(name);
				//int i = 0;
				// CPUで書き込みができるテクスチャを作成
				D3D11_TEXTURE2D_DESC desc;
				memset(&desc, 0, sizeof(desc));
				desc.Width = tga.GetWidth();
				desc.Height = tga.GetHeight();
				desc.MipLevels = 1;
				desc.ArraySize = 1;
				// RGBA(255,255,255,255)タイプ
				desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				desc.SampleDesc.Count = 1;
				// 動的（書き込みするための必須条件）
				desc.Usage = D3D11_USAGE_DYNAMIC;
				// シェーダリソースとして使う	
				desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
				// CPUからアクセスして書き込みOK
				desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

				ID3D11Texture2D* tex2D;   /// 2Ｄテクスチャ
				HRESULT hr = device->CreateTexture2D(&desc, 0, &tex2D);

				// フォント情報をテクスチャに書き込む部分
				D3D11_MAPPED_SUBRESOURCE hMappedResource;
				hr = deviceContext->Map(
					tex2D,
					0,
					D3D11_MAP_WRITE_DISCARD,
					0,
					&hMappedResource);
				if (FAILED(hr)){
					return E_FAIL;
				}

				// ここで書き込む
				BYTE* pBits = (BYTE*)hMappedResource.pData;
				//memset(pBits, 0, hMappedResource.RowPitch * TM.tmHeight);
				memcpy(
					(BYTE*)pBits,
					tga.GetImage(),
					tga.GetHeight()*tga.GetWidth()*sizeof(unsigned));
				deviceContext->Unmap(tex2D, 0);


				// ShaderResourceViewの情報を作成する
				D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
				ZeroMemory(&srvDesc, sizeof(srvDesc));
				srvDesc.Format = desc.Format;
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				srvDesc.Texture2D.MostDetailedMip = 0;
				srvDesc.Texture2D.MipLevels = desc.MipLevels;

				// ShaderResourceViewを作成する
				hr = device->CreateShaderResourceView(tex2D, &srvDesc, &m_pTexture[i]);

				int lgjlir = 0;


			}
			else{
				HRESULT hr;
				if (FAILED(hr = D3DX11CreateShaderResourceViewFromFile(device, name, NULL, NULL, &m_pTexture[i], NULL)))
				{
					return E_FAIL;
				}
			}
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