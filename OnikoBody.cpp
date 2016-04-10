#include "Oniko.h"
#include "GyuDon.h"
#include "Director.h"
#include "TGATexture.h"


HRESULT Oniko::Body::initShader(){

	ID3D11Device* device = Director::instance()->framework()->device();
	ID3D11DeviceContext* deviceContext = Director::instance()->framework()->deviceContext();

	ID3DBlob *pCompiledShader = NULL;
	ID3DBlob *pErrors = NULL;

	if (FAILED(D3DX11CompileFromFile("pmxVertex.hlsl", NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, "hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(device->CreateVertexShader(pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(), NULL, &m_pVertexShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, "�o�[�e�b�N�X�V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "POSITION", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "POSITION", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//D3DXVECTOR3 Pos;
		//D3DXVECTOR4 vBoneIndices;
		//D3DXVECTOR4 vBoneWeights; // �ő�l�_�E�F�C�g
		//float vWeitFormula;
		//D3DXVECTOR3 Normal;
		//float Tex[2];
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	if (FAILED(device->CreateInputLayout(layout, numElements,
		pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout)))
	{
		return E_FAIL;
	}

	if (FAILED(D3DX11CompileFromFile("pmxPixel.hlsl", NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, "hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(device->CreatePixelShader(pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(), NULL, &m_pPixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, "�s�N�Z���V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	{
		int csize = sizeof(PMX_VS_CONSTANT_BUFFER);
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
			return E_FAIL;
		}
	}

	{
		int csize = sizeof(PMX_PS_CONSTANT_BUFFER);
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
		if (FAILED(hr = device->CreateBuffer(&cb, NULL, &m_pPSConstantBuffer)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT Oniko::Body::initPolygon(){
	ID3D11Device* device = Director::instance()->framework()->device();
	ID3D11DeviceContext* deviceContext = Director::instance()->framework()->deviceContext();

	// 2016-03-24 �ǉ� pmx�����_�����O�e�X�g
	//const char *filename = "dat/pmx/tda_apemiku/Tda�������~�N�E�A�y���h_Ver1.00.pmx";
	//const char *filename = "dat/pmx/oniko/���{�S�q0.15.pmx";
	std::string str = (m_directoryName + "/" + m_fileName).c_str();
	const char *filename_ = str.c_str();
	std::ifstream stream = std::ifstream(filename_, std::ios_base::binary);
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
			vertices[i].Pos.w = 1.f;
			//m_model.materials[0].
			//vertices[i].vBoneIndices.x = m_model.bones[i].
			vertices[i].Tex[0] = m_model.vertices[i].uv[0];
			vertices[i].Tex[1] = m_model.vertices[i].uv[1];
			vertices[i].vWeitFormula = (unsigned)m_model.vertices[i].skinning_type;
			if (m_model.vertices[i].skinning_type == pmx::PmxVertexSkinningType::BDEF1){
				pmx::PmxVertexSkinning* skining = m_model.vertices[i].skinning.get();
				pmx::PmxVertexSkinningBDEF1* bdef1 = reinterpret_cast<pmx::PmxVertexSkinningBDEF1*>(skining);
				unsigned* out = vertices[i].vBoneIndices;
				out[0] = (unsigned)bdef1->bone_index;
				out[1] = 0;
				out[2] = 0;
				out[3] = 0;
			}
			else if (m_model.vertices[i].skinning_type == pmx::PmxVertexSkinningType::BDEF2){
				pmx::PmxVertexSkinning* skining = m_model.vertices[i].skinning.get();
				pmx::PmxVertexSkinningBDEF2* bdef2 = reinterpret_cast<pmx::PmxVertexSkinningBDEF2*>(skining);
				unsigned* out = vertices[i].vBoneIndices;
				out[0] = (unsigned)bdef2->bone_index1;
				out[1] = (unsigned)bdef2->bone_index2;
				//if (out[0] != 87 && out[0] != 125 && out[0] != 124){
				//	int i = 0;
				//}
				out[2] = 0;
				out[3] = 0;
				vertices[i].vBoneWeights[0] = bdef2->bone_weight;
				vertices[i].vBoneWeights[1] = 1.0f - bdef2->bone_weight;
				int i = 0;
			}
			else if (m_model.vertices[i].skinning_type == pmx::PmxVertexSkinningType::BDEF4){
				HALT(f);
			}
			else if (m_model.vertices[i].skinning_type == pmx::PmxVertexSkinningType::QDEF){
				HALT(f);
			}
			else if (m_model.vertices[i].skinning_type == pmx::PmxVertexSkinningType::SDEF){
				HALT(f);
			}
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

HRESULT Oniko::Body::initTexture(){
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
				// CPU�ŏ������݂��ł���e�N�X�`�����쐬
				D3D11_TEXTURE2D_DESC desc;
				memset(&desc, 0, sizeof(desc));
				desc.Width = tga.GetWidth();
				desc.Height = tga.GetHeight();
				desc.MipLevels = 1;
				desc.ArraySize = 1;
				// RGBA(255,255,255,255)�^�C�v
				desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				desc.SampleDesc.Count = 1;
				// ���I�i�������݂��邽�߂̕K�{�����j
				desc.Usage = D3D11_USAGE_DYNAMIC;
				// �V�F�[�_���\�[�X�Ƃ��Ďg��	
				desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
				// CPU����A�N�Z�X���ď�������OK
				desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

				ID3D11Texture2D* tex2D;   /// 2�c�e�N�X�`��
				HRESULT hr = device->CreateTexture2D(&desc, 0, &tex2D);

				// �t�H���g�����e�N�X�`���ɏ������ޕ���
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

				// �����ŏ�������
				BYTE* pBits = (BYTE*)hMappedResource.pData;
				//memset(pBits, 0, hMappedResource.RowPitch * TM.tmHeight);
				memcpy(
					(BYTE*)pBits,
					tga.GetImage(),
					tga.GetHeight()*tga.GetWidth()*sizeof(unsigned));
				deviceContext->Unmap(tex2D, 0);


				// ShaderResourceView�̏����쐬����
				D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
				ZeroMemory(&srvDesc, sizeof(srvDesc));
				srvDesc.Format = desc.Format;
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				srvDesc.Texture2D.MostDetailedMip = 0;
				srvDesc.Texture2D.MipLevels = desc.MipLevels;

				// ShaderResourceView���쐬����
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
		for (int i = 0; i < 3; i++){
			D3D11_SAMPLER_DESC samDesc;
			ZeroMemory(&samDesc, sizeof(samDesc));
			samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			samDesc.AddressU = samDesc.AddressV = samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			samDesc.MaxAnisotropy = 1;
			samDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
			samDesc.MaxLOD = D3D11_FLOAT32_MAX;

			device->CreateSamplerState(&samDesc, &m_pSamplerState[i]);
		}
	}

	return S_OK;
}

HRESULT Oniko::Body::initBone(){

	return S_OK;
}