#include "PMXModel.h"
#include "DEFINE.h"
#include "GyuDon.h"
#include "Director.h"
#include "TGATexture.h"
#include "lib\MikuMikuFormats\Pmx.h"
#include <string>

#define LIMIT_BONE_COUNT 512

// DirectX系の解放マクロです。
#define SAFE_RELEASE(x) if (x){ x->Release(); x; }


PMXModel::PMXModel(const char* directory, const char* fileName)
{
	m_directoryName = directory;
	m_fileName = fileName;
	ASSERT(SUCCEEDED(initShader()));
	ASSERT(SUCCEEDED(initPolygon()));
	ASSERT(SUCCEEDED(initTexture()));
	ASSERT(SUCCEEDED(initBone()));
}


PMXModel::~PMXModel()
{
	delete[] m_renderedBones;
	m_renderedBones = 0;
	SAFE_DELETE_ARRAY(m_Bones);
	for (int i = 0; i < 3; i++){
		SAFE_RELEASE(m_pSamplerState[i]);
	}
	for (int i = 0; i < m_model.texture_count; i++){
		SAFE_RELEASE(m_pTexture[i]);
	}
	delete[] m_pTexture;
	SAFE_RELEASE(m_pVSConstantBuffer);
	SAFE_RELEASE(m_pPSConstantBuffer);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pIndexBuffer);
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pVertexLayout);
}


namespace{

	struct SimpleVertex
	{
		D3DXVECTOR4 Pos;
		unsigned vBoneIndices[4];
		D3DXVECTOR4 vBoneWeights; // 最大四点ウェイト
		unsigned vWeitFormula;
		D3DXVECTOR3 Normal;
		float Tex[2];
	};
	struct PMX_VS_CONSTANT_BUFFER
	{
		D3DXMATRIX mV;
		D3DXMATRIX mP;
		D3DXMATRIX test;
		D3DXMATRIX mW[LIMIT_BONE_COUNT];
	};
	struct PMX_PS_CONSTANT_BUFFER{
		D3DXVECTOR3 ambient;
		D3DXVECTOR4 diffuse;
		D3DXVECTOR3 specular;
		float shininess;

		D3DXVECTOR3 lightDirection; //direction towards the light
		D3DXVECTOR3 halfVector; //surface orientation for shiniest spots- the vector stretching from from the camera position to the target

		bool isEdge;
		D3DXVECTOR4 edgeColor;
		float edgeSize;

		float fSphereMode; //sphereMode stored as a float
	};

}

void PMXModel::draw(){
	ID3D11Device* device = Director::instance()->framework()->device();
	ID3D11DeviceContext* deviceContext = Director::instance()->framework()->deviceContext();
	const D3DXMATRIX& projMat = Director::instance()->framework()->projMat();
	const D3DXMATRIX& viewMat = Director::instance()->framework()->viewMat();
	// テスト用
	D3DXMATRIX mWorld;
	D3DXMatrixRotationY(&mWorld, timeGetTime() / 1000.0f);

	float val = timeGetTime() / 1000.0f;
	const D3DXVECTOR3& vEyePt = Director::instance()->framework()->vEyePt();

	deviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pPixelShader, NULL, 0);

	deviceContext->IASetInputLayout(m_pVertexLayout);
	deviceContext->PSSetSamplers(0, 3, m_pSamplerState);

	D3DXMATRIX testRota;
	D3DXMatrixRotationAxis(&testRota, &D3DXVECTOR3(1.f, 0.f, 0.f), timeGetTime()*M_PI/1000.f);

	// 差分生成。実際に動かしたい値をここで用意しておく。
	for (int i = 0; i < m_model.bone_count; i++){
		Bone* bone = &m_Bones[i];
		if (bone->id != 10){
			D3DXMatrixIdentity(&bone->boneMat);
		}
		else{
			bone->boneMat = testRota;
		}
	}

	// ボーン情報をレンダリングする。シェーダに渡す形にする。初期姿勢であれば、単位行列になるはずだ。それでテストが出来る。
	struct BoneToShader{
		static void Render(Bone* bone){
			if (bone->parent){
				bone->boneMat = bone->boneMat * bone->initMat * bone->parent->boneMat;
				bone->combMatAry[bone->id] = bone->offsetMat * bone->boneMat;
			}
			else{
				bone->boneMat = bone->initMat;
				bone->combMatAry[bone->id] = bone->offsetMat * bone->boneMat;
			}
			// 親は当然先に計算しなければ意味が無い。ローカルにならない。そして子に波及する。兄弟と子はこの場合順を問わない。
			if (bone->firstChild){
				Render(bone->firstChild);
			}
			if (bone->sibling){
				Render(bone->sibling);
			}
		}
	};
	BoneToShader::Render(&m_Bones[0]);



	// コンスタント配列に押し込む
	//for (int i = 0; i < m_model.bone_count; i++){
	//	Bone* bone = &m_Bones[i];
	//	bone->combMatAry[i] = bone->boneMat;
	//}

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
			{
				D3D11_MAPPED_SUBRESOURCE pData;
				PMX_VS_CONSTANT_BUFFER cb;
				// バーテクス
				if (SUCCEEDED(deviceContext->Map(m_pVSConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
				{
					pmx::PmxMaterial& material = m_model.materials[i];
					std::wstring name = m_model.textures[2];
					for (int j = 0; j < m_model.bone_count; j++){
						cb.mW[j] = m_renderedBones[j]*mWorld;
						D3DXMatrixTranspose(&cb.mW[j], &cb.mW[j]);
					}
					cb.mV = viewMat;
					D3DXMatrixTranspose(&cb.mV, &cb.mV);
					cb.mP = projMat;
					D3DXMatrixTranspose(&cb.mP, &cb.mP);
					//m_model.vertices[i].skinning
					cb.test = mWorld/**projMat*viewMat*/;
					D3DXMatrixTranspose(&cb.test, &cb.test);

					memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(PMX_VS_CONSTANT_BUFFER));
					deviceContext->Unmap(m_pVSConstantBuffer, 0);
				}
			}

			{
				D3D11_MAPPED_SUBRESOURCE pData;
				PMX_PS_CONSTANT_BUFFER cb;
				// ピクセル
				if (SUCCEEDED(deviceContext->Map(m_pPSConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
				{
					pmx::PmxMaterial& material = m_model.materials[i];
					std::wstring name = m_model.textures[2];
					//cb. = mWorld;
					//D3DXMatrixTranspose(&cb.mW, &cb.mW);
					//D3DMATRIX m = mWorld*viewMat*projMat;
					//cb.mWVP = m;
					//D3DXMatrixTranspose(&cb.mWVP, &cb.mWVP
					// ハーフベクトルのために視線ターゲットのワールド座標算出
					D3DXVECTOR3 eyeVec = Director::instance()->framework()->vLookatPt() - Director::instance()->framework()->vEyePt();
					D3DXVec3Normalize(&eyeVec, &eyeVec);

					cb.diffuse.x = m_model.materials[i].diffuse[0];
					cb.diffuse.y = m_model.materials[i].diffuse[1];
					cb.diffuse.z = m_model.materials[i].diffuse[2];
					cb.diffuse.w = 1.0f;
					cb.ambient.x = m_model.materials[i].ambient[0];
					cb.ambient.y = m_model.materials[i].ambient[1];
					cb.ambient.z = m_model.materials[i].ambient[2];
					cb.specular.x = m_model.materials[i].specular[0];
					cb.specular.y = m_model.materials[i].specular[1];
					cb.specular.z = m_model.materials[i].specular[2];
					cb.shininess = m_model.materials[i].specularlity;
					if (material.diffuse_texture_index == 3){
						int jjj = 0;
					}
					cb.edgeColor = m_model.materials[i].edge_color;
					cb.edgeSize = m_model.materials[i].edge_size;
					cb.fSphereMode = m_model.materials[i].sphere_op_mode;
					cb.lightDirection = Director::instance()->framework()->getDirectionLight();
					cb.isEdge = m_model.materials[i].flag;
					cb.lightDirection = D3DXVECTOR3(0, -0.5, 0.5);
					D3DXVec3Normalize(&cb.lightDirection, &cb.lightDirection);

					D3DXVECTOR3 harf;
					D3DXVec3Normalize(&harf, &(cb.lightDirection + eyeVec));
					cb.halfVector = harf;
					memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(PMX_PS_CONSTANT_BUFFER));
					deviceContext->Unmap(m_pVSConstantBuffer, 0);
				}
			}

			deviceContext->VSSetConstantBuffers(0, 1, &m_pVSConstantBuffer);
			deviceContext->PSSetConstantBuffers(0, 1, &m_pPSConstantBuffer);

			// 普通のテクスチャ
			if (m_model.materials[i].diffuse_texture_index != -1){
				deviceContext->PSSetShaderResources(2, 1, &m_pTexture[m_model.materials[i].diffuse_texture_index]);
			}
			// スフィアのテクスチャ
			if (m_model.materials[i].sphere_texture_index != -1){
				deviceContext->PSSetShaderResources(1, 1, &m_pTexture[m_model.materials[i].sphere_texture_index]);
			}
			if (m_model.materials[i].toon_texture_index != -1){
				deviceContext->PSSetShaderResources(0, 1, &m_pTexture[m_model.materials[i].toon_texture_index]);
			}
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

	if (FAILED(D3DX11CompileFromFile("pmxVertex.hlsl", NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
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
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "POSITION", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "POSITION", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//D3DXVECTOR3 Pos;
		//D3DXVECTOR4 vBoneIndices;
		//D3DXVECTOR4 vBoneWeights; // 最大四点ウェイト
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

HRESULT PMXModel::initPolygon(){
	ID3D11Device* device = Director::instance()->framework()->device();
	ID3D11DeviceContext* deviceContext = Director::instance()->framework()->deviceContext();

	// 2016-03-24 追加 pmxレンダリングテスト
	//const char *filename = "dat/pmx/tda_apemiku/Tda式初音ミク・アペンド_Ver1.00.pmx";
	//const char *filename = "dat/pmx/oniko/日本鬼子0.15.pmx";
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
			else if(m_model.vertices[i].skinning_type == pmx::PmxVertexSkinningType::BDEF4){
				HALT(f);
			}
			else if(m_model.vertices[i].skinning_type == pmx::PmxVertexSkinningType::QDEF){
				HALT(f);
			}
			else if(m_model.vertices[i].skinning_type == pmx::PmxVertexSkinningType::SDEF){
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

HRESULT PMXModel::initBone(){
	ASSERT(m_model.bone_count <= LIMIT_BONE_COUNT && "非対応ボーン数");

	// ボーンオブジェクトと、シェーダに渡すための配列が必要だ。
	m_Bones = new Bone[m_model.bone_count];
	m_renderedBones = new D3DXMATRIX[m_model.bone_count];

	// まず必要なのは何か？ローカルボーン座標だ。PMXには移動しか入っていない。
	for (int i = 0; i < m_model.bone_count; i++){
		pmx::PmxBone& data = m_model.bones[i];
		Bone* out = &m_Bones[i];
		D3DXMatrixTranslation(&(out->initMat), data.position[0], data.position[1], data.position[2]);
		out->combMatAry = m_renderedBones;
		out->parent = 0;
		out->firstChild = 0;
		out->id = i;
		out->sibling = 0;
	}

	// 色々やりやすくするために、親子関係をセットしておく必要がある。
	for (int i = 0; i < m_model.bone_count; i++){
		pmx::PmxBone& childData = m_model.bones[i];
		Bone* child = &m_Bones[i];
		if (childData.parent_index != -1){
			Bone* parent = &m_Bones[childData.parent_index];
			if (parent->firstChild){
				Bone* sibling = parent->firstChild;
				while (sibling->sibling){
					sibling = sibling->sibling;
				}
				sibling->sibling = child;
			}
			else{
				parent->firstChild = child;
			}
			child->parent = parent;
		}
	}

	// 次に各ボーンのローカルオフセットだ。実際にボーンを動かすにはこいつがいる。
	for (int i = 0; i < m_model.bone_count; i++){
		D3DXMatrixInverse(&m_Bones[i].offsetMat, 0, &m_Bones[i].initMat);
	}

	// それから上記を用いて、初期姿勢をローカルから親基点にする。これで差分行列が乗算できるようになる。
	for (int i = 0; i < m_model.bone_count; i++){
		Bone* bone = &m_Bones[i];
		if (bone->parent){
			bone->initMat *= bone->parent->offsetMat;
		}
	}

	// 初期化は完了。

	// テスト用。ボーン名の情報追加。
#ifdef TEST_SIZURU
	for (int i = 0; i < m_model.bone_count; i++){
		Bone* bone = &m_Bones[i];
		char temp[512];
		convert2Str(temp, m_model.bones[i].bone_name.c_str());
		bone->name = temp;
	}
#endif

	return S_OK;
}