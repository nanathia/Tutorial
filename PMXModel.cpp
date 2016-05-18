
#include "PMXModel.h"
#include "DEFINE.h"
#include "GyuDon.h"
#include "Director.h"
#include "TGATexture.h"
#include "VMDMotion.h"

PMXModel::PMXModel(const char* directoryName, const char* pmxName)
{
	// test
	OBJECT_CREATE(m_testMotion, new VMDMotion("dat/vmd/d.vmd", 0.5f));

	m_Body.m_directoryName = directoryName;
	m_Body.m_fileName = pmxName;
	ASSERT(SUCCEEDED(m_Body.initShader()));
	ASSERT(SUCCEEDED(m_Body.initPolygon()));
	ASSERT(SUCCEEDED(m_Body.initTexture()));

	this->init();
}


PMXModel::~PMXModel()
{
	OBJECT_RELEASE(m_testMotion);
	delete[] m_Body.m_renderedBones;
	m_Body.m_renderedBones = 0;
	SAFE_DELETE_ARRAY(m_Body.m_Bones);
	for (int i = 0; i < 3; i++){
		SAFE_RELEASE(m_Body.m_pSamplerState[i]);
	}
	for (int i = 0; i < m_Body.m_model.texture_count; i++){
		SAFE_RELEASE(m_Body.m_pTexture[i]);
	}
	delete[] m_Body.m_pTexture;
	SAFE_RELEASE(m_Body.m_pVSConstantBuffer);
	SAFE_RELEASE(m_Body.m_pPSConstantBuffer);
	SAFE_RELEASE(m_Body.m_pVertexShader);
	SAFE_RELEASE(m_Body.m_pPixelShader);
	SAFE_RELEASE(m_Body.m_pIndexBuffer);
	SAFE_RELEASE(m_Body.m_pVertexBuffer);
	SAFE_RELEASE(m_Body.m_pVertexLayout);
}


void PMXModel::init(){

	ID3D11Device* device = Director::instance()->framework()->device();
	ID3D11DeviceContext* deviceContext = Director::instance()->framework()->deviceContext();

	///////////////////////////////////////////////////////////////////////
	ASSERT(m_Body.m_model.bone_count <= 512 && "非対応ボーン数");

	// ボーンオブジェクトと、シェーダに渡すための配列が必要だ。
	m_Body.m_Bones = new PmxBone[m_Body.m_model.bone_count];
	m_Body.m_renderedBones = new D3DXMATRIX[m_Body.m_model.bone_count];

	// まず必要なのは何か？ローカルボーン座標だ。PMXには移動しか入っていない。
	for (int i = 0; i < m_Body.m_model.bone_count; i++){
		pmx::PmxBone& data = m_Body.m_model.bones[i];
		PmxBone* out = &m_Body.m_Bones[i];
		D3DXMatrixTranslation(&(out->initMat), data.position[0], data.position[1], data.position[2]);
		out->combMatAry = m_Body.m_renderedBones;
		out->parent = 0;
		out->firstChild = 0;
		out->id = i;
		out->sibling = 0;
	}

	// 色々やりやすくするために、親子関係をセットしておく必要がある。
	for (int i = 0; i < m_Body.m_model.bone_count; i++){
		pmx::PmxBone& childData = m_Body.m_model.bones[i];
		PmxBone* child = &m_Body.m_Bones[i];
		if (childData.parent_index != -1){
			PmxBone* parent = &m_Body.m_Bones[childData.parent_index];
			if (parent->firstChild){
				PmxBone* sibling = parent->firstChild;
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
	for (int i = 0; i < m_Body.m_model.bone_count; i++){
		D3DXMatrixInverse(&m_Body.m_Bones[i].offsetMat, 0, &m_Body.m_Bones[i].initMat);
	}

	// それから上記を用いて、初期姿勢をローカルから親基点にする。これで差分行列が乗算できるようになる。
	for (int i = 0; i < m_Body.m_model.bone_count; i++){
		PmxBone* bone = &m_Body.m_Bones[i];
		if (bone->parent){
			bone->initMat *= bone->parent->offsetMat;
		}
	}

	// 初期化は完了。

	// テスト用。ボーン名の情報追加。
#ifdef TEST_SIZURU
	for (int i = 0; i < m_Body.m_model.bone_count; i++){
		PmxBone* bone = &m_Body.m_Bones[i];
		char temp[512];
		convert2Str(temp, m_Body.m_model.bones[i].bone_name.c_str());
		bone->name = temp;
	}
#endif


	////////////////////////////////////////////////////////////////////////////

	//pmx::PmxBone test = m_body

	/*
	m_Daituuren_Toushin.m_daiturenFile.Load("dat/obj/tuuren", "katana");

	// お体の50番が中指
	m_Daituuren_Toushin.m_parentBone_id = 50;
	// 親ボーン
	pmx::PmxBone* parentBone = &m_Body.m_model.bones[m_Daituuren_Toushin.m_parentBone_id];
	// 初期姿勢はとりあ親と被す
	D3DXMatrixIdentity(&m_Daituuren_Toushin.m_InitMat);

	// vertex
	{
		Daituuren_Toushin::SimpleVertex* vertices = new Daituuren_Toushin::SimpleVertex[m_Daituuren_Toushin.m_daiturenFile.vertexSize()];
		for (int i = 0; i < m_Daituuren_Toushin.m_daiturenFile.vertexSize(); i++){
			vertices[i].position.x = m_Daituuren_Toushin.m_daiturenFile.getPosition()[i].x;
			vertices[i].position.y = m_Daituuren_Toushin.m_daiturenFile.getPosition()[i].y;
			vertices[i].position.z = m_Daituuren_Toushin.m_daiturenFile.getPosition()[i].z;
			vertices[i].position.w = 1.f;
			vertices[i].normal.x = m_Daituuren_Toushin.m_daiturenFile.getNormal()[i].x;
			vertices[i].normal.y = m_Daituuren_Toushin.m_daiturenFile.getNormal()[i].y;
			vertices[i].normal.z = m_Daituuren_Toushin.m_daiturenFile.getNormal()[i].z;
			vertices[i].uv.x = m_Daituuren_Toushin.m_daiturenFile.getUV()[i].x;
			vertices[i].uv.y = m_Daituuren_Toushin.m_daiturenFile.getUV()[i].y;
		}
		D3DXMATRIX InitTrance;
		// 付けたいとこまで持ってく
		D3DXMatrixTranslation(&InitTrance, parentBone->position[0] + 0.1f, parentBone->position[1] - 0.1f, parentBone->position[2]);
		// サイズ調整
		D3DXMATRIX InitScale;
		D3DXMatrixScaling(&InitScale, 1.8f, 1.8f, 1.8f);
		D3DXMATRIX InitRota;
		D3DXMatrixRotationAxis(&InitRota, &D3DXVECTOR3(1.f, 0.f, 0.f), 1.f * M_PI);
		D3DXMATRIX InitMat = InitScale*InitRota*InitTrance;
		for (int i = 0; i < m_Daituuren_Toushin.m_daiturenFile.vertexSize(); i++){
			D3DXVec3TransformCoord(reinterpret_cast<D3DXVECTOR3*>(&vertices[i].position), reinterpret_cast<D3DXVECTOR3*>(&vertices[i].position), &InitMat);
		}
		D3DXMatrixInverse(&m_Daituuren_Toushin.m_bone.offsetMat, 0, &InitMat);
		m_Daituuren_Toushin.m_bone.initMat = InitMat*m_Body.m_Bones[m_Daituuren_Toushin.m_parentBone_id].offsetMat;
		m_Daituuren_Toushin.m_bone.parent = &m_Body.m_Bones[m_Daituuren_Toushin.m_parentBone_id];

		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(Daituuren_Toushin::SimpleVertex) * m_Daituuren_Toushin.m_daiturenFile.vertexSize();
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = vertices;

		if (FAILED(device->CreateBuffer(&bd, &InitData, &m_Daituuren_Toushin.m_pVertexBuffer)))
		{
			delete[] vertices;
			HALT(h);
		}
		delete[] vertices;

	}


	// index
	{
		unsigned* indecies = new unsigned[m_Daituuren_Toushin.m_daiturenFile.vertexIndexSize()];
		for (int i = 0; i < m_Daituuren_Toushin.m_daiturenFile.vertexIndexSize(); i++){
			indecies[i] = m_Daituuren_Toushin.m_daiturenFile.getVertexIndex()[i];
		}
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned) * m_Daituuren_Toushin.m_daiturenFile.vertexIndexSize();
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = indecies;

		if (FAILED(device->CreateBuffer(&bd, &InitData, &m_Daituuren_Toushin.m_pIndexBuffer)))
		{
			delete[] indecies;
			HALT(h);
		}
		delete[] indecies;
	}
	*/
}

// 描画
void PMXModel::draw(){
	ID3D11Device* device = Director::instance()->framework()->device();
	ID3D11DeviceContext* deviceContext = Director::instance()->framework()->deviceContext();
	const D3DXMATRIX& projMat = Director::instance()->framework()->projMat();
	const D3DXMATRIX& viewMat = Director::instance()->framework()->viewMat();
	// テスト用
	D3DXMATRIX mWorld;
	//float val = timeGetTime() / 1000.f;
	float val = 0.f;
	D3DXMatrixRotationY(&mWorld, val);

	const D3DXVECTOR3& vEyePt = Director::instance()->framework()->vEyePt(); this;

#pragma region body

	deviceContext->VSSetShader(m_Body.m_pVertexShader, NULL, 0);
	deviceContext->PSSetShader(m_Body.m_pPixelShader, NULL, 0);

	deviceContext->IASetInputLayout(m_Body.m_pVertexLayout);
	deviceContext->PSSetSamplers(0, 3, m_Body.m_pSamplerState);

	D3DXMATRIX testRota;
	D3DXMatrixRotationAxis(&testRota, &D3DXVECTOR3(0.f, 1.f, 0.f), val*M_PI);

	// モーション更新
	m_testMotion->update();
	m_testMotion->BoneRendering(m_Body.m_Bones, m_Body.m_model.bones, m_Body.m_model.bone_count);

	{
		//pmx::PmxBone* parentBone = &m_Body.m_model.bones[m_Daituuren_Toushin.m_parentBone_id];

		// 2016-03-27 pmx 描画
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		UINT stride = sizeof(Body::SimpleVertex);
		UINT offset = 0;
		deviceContext->IASetVertexBuffers(0, 1, &m_Body.m_pVertexBuffer, &stride, &offset);
		deviceContext->IASetIndexBuffer(m_Body.m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		int index = 0;
		for (int i = 0; i < m_Body.m_model.material_count; i++)
		{
			{
				D3D11_MAPPED_SUBRESOURCE pData;
				Body::PMX_VS_CONSTANT_BUFFER cb;
				// バーテクス
				if (SUCCEEDED(deviceContext->Map(m_Body.m_pVSConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
				{
					pmx::PmxMaterial& material = m_Body.m_model.materials[i];
					std::wstring name = m_Body.m_model.textures[2];
					for (int j = 0; j < m_Body.m_model.bone_count; j++){
						cb.mW[j] = m_Body.m_renderedBones[j] * mWorld;
						D3DXMatrixTranspose(&cb.mW[j], &cb.mW[j]);
					}
					cb.mV = viewMat;
					D3DXMatrixTranspose(&cb.mV, &cb.mV);
					cb.mP = projMat;
					D3DXMatrixTranspose(&cb.mP, &cb.mP);
					//m_model.vertices[i].skinning
					cb.test = mWorld/**projMat*viewMat*/;
					D3DXMatrixTranspose(&cb.test, &cb.test);

					memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(Body::PMX_VS_CONSTANT_BUFFER));
					deviceContext->Unmap(m_Body.m_pVSConstantBuffer, 0);
				}
			}

			{
				D3D11_MAPPED_SUBRESOURCE pData;
				Body::PMX_PS_CONSTANT_BUFFER cb;
				// ピクセル
				if (SUCCEEDED(deviceContext->Map(m_Body.m_pPSConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
				{
					pmx::PmxMaterial& material = m_Body.m_model.materials[i];
					std::wstring name = m_Body.m_model.textures[2];
					//cb. = mWorld;
					//D3DXMatrixTranspose(&cb.mW, &cb.mW);
					//D3DMATRIX m = mWorld*viewMat*projMat;
					//cb.mWVP = m;
					//D3DXMatrixTranspose(&cb.mWVP, &cb.mWVP
					// ハーフベクトルのために視線ターゲットのワールド座標算出
					D3DXVECTOR3 eyeVec = Director::instance()->framework()->vLookatPt() - Director::instance()->framework()->vEyePt();
					D3DXVec3Normalize(&eyeVec, &eyeVec);

					cb.diffuse.x = m_Body.m_model.materials[i].diffuse[0];
					cb.diffuse.y = m_Body.m_model.materials[i].diffuse[1];
					cb.diffuse.z = m_Body.m_model.materials[i].diffuse[2];
					cb.diffuse.w = 1.0f;
					cb.ambient.x = m_Body.m_model.materials[i].ambient[0];
					cb.ambient.y = m_Body.m_model.materials[i].ambient[1];
					cb.ambient.z = m_Body.m_model.materials[i].ambient[2];
					cb.specular.x = m_Body.m_model.materials[i].specular[0];
					cb.specular.y = m_Body.m_model.materials[i].specular[1];
					cb.specular.z = m_Body.m_model.materials[i].specular[2];
					cb.shininess = m_Body.m_model.materials[i].specularlity;
					if (material.diffuse_texture_index == 3){
						int jjj = 0;
					}
					cb.edgeColor = m_Body.m_model.materials[i].edge_color;
					cb.edgeSize = m_Body.m_model.materials[i].edge_size;
					cb.fSphereMode = m_Body.m_model.materials[i].sphere_op_mode;
					//cb.lightDirection = Director::instance()->framework()->getDirectionLight();
					cb.isEdge = m_Body.m_model.materials[i].flag;
					cb.lightDirection = D3DXVECTOR3(0, -0.5, 0.5);
					D3DXVec3Normalize(&cb.lightDirection, &cb.lightDirection);

					D3DXVECTOR3 harf;
					D3DXVec3Normalize(&harf, &(cb.lightDirection + eyeVec));
					cb.halfVector = harf;
					memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(Body::PMX_PS_CONSTANT_BUFFER));
					deviceContext->Unmap(m_Body.m_pVSConstantBuffer, 0);
				}
			}

			deviceContext->VSSetConstantBuffers(0, 1, &m_Body.m_pVSConstantBuffer);
			deviceContext->PSSetConstantBuffers(0, 1, &m_Body.m_pPSConstantBuffer);

			// 普通のテクスチャ
			if (m_Body.m_model.materials[i].diffuse_texture_index != -1){
				deviceContext->PSSetShaderResources(2, 1, &m_Body.m_pTexture[m_Body.m_model.materials[i].diffuse_texture_index]);
			}
			// スフィアのテクスチャ
			if (m_Body.m_model.materials[i].sphere_texture_index != -1){
				deviceContext->PSSetShaderResources(1, 1, &m_Body.m_pTexture[m_Body.m_model.materials[i].sphere_texture_index]);
			}
			if (m_Body.m_model.materials[i].toon_texture_index != -1){
				deviceContext->PSSetShaderResources(0, 1, &m_Body.m_pTexture[m_Body.m_model.materials[i].toon_texture_index]);
			}
			deviceContext->DrawIndexed(m_Body.m_model.materials[i].index_count, index, 0);
			index += m_Body.m_model.materials[i].index_count;
			//m_model.materials[i].
		}
	}

#pragma endregion

	/*
	//////////////////////////////////////////////////////////////////////////
	{
		ID3D11Device* device = Director::instance()->framework()->device();
		ID3D11DeviceContext* deviceContext = Director::instance()->framework()->deviceContext();
		const D3DXMATRIX& projMat = Director::instance()->framework()->projMat();
		const D3DXMATRIX& viewMat = Director::instance()->framework()->viewMat();
		// 
		D3DXMATRIX Sabun;
		D3DXMATRIX rota;
		D3DXMATRIX scale;
		D3DXMATRIX trance;
		D3DXMatrixTranslation(&trance, 0.f, 0.f, 0.f);
		D3DXMatrixScaling(&scale, 1.f, 1.f, 1.f);
		D3DXMatrixRotationAxis(&rota, &D3DXVECTOR3(1.f, 0.f, 0.f), val);
		Sabun = scale * rota * trance;
		m_Daituuren_Toushin.m_bone.boneMat = Sabun * m_Daituuren_Toushin.m_bone.initMat * m_Daituuren_Toushin.m_bone.parent->boneMat;
		m_Daituuren_Toushin.m_bone.boneMat = m_Daituuren_Toushin.m_bone.parent->offsetMat * m_Daituuren_Toushin.m_bone.boneMat;

		const D3DXVECTOR3& vEyePt = Director::instance()->framework()->vEyePt();

		deviceContext->VSSetShader(m_Daituuren_Toushin.m_pVertexShader, NULL, 0);
		deviceContext->PSSetShader(m_Daituuren_Toushin.m_pPixelShader, NULL, 0);

		deviceContext->IASetInputLayout(m_Daituuren_Toushin.m_pVertexLayout);
		deviceContext->PSSetSamplers(0, 1, &m_Daituuren_Toushin.m_pSamplerState);
		// 普通のテクスチャ
		deviceContext->PSSetShaderResources(0, 1, &m_Daituuren_Toushin.m_pTexture);

		float blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };
		deviceContext->OMSetBlendState(0, blendFactor, 0xffffffff);

		// 刀身
		{
			// 2016-03-27 pmx 描画
			deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			UINT stride = sizeof(Daituuren_Toushin::SimpleVertex);
			UINT offset = 0;
			deviceContext->IASetVertexBuffers(0, 1, &m_Daituuren_Toushin.m_pVertexBuffer, &stride, &offset);
			deviceContext->IASetIndexBuffer(m_Daituuren_Toushin.m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

			D3D11_MAPPED_SUBRESOURCE pData;
			Daituuren_Toushin::CONSTANT_BUFFER_TOUSHIN cb;
			// バーテクス・ピクセル
			if (SUCCEEDED(deviceContext->Map(m_Daituuren_Toushin.m_pVSConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
			{
				cb.g_mW = m_Daituuren_Toushin.m_bone.boneMat * mWorld;
				D3DXMatrixTranspose(&cb.g_mW, &cb.g_mW);
				cb.g_mWVP = m_Daituuren_Toushin.m_bone.boneMat * mWorld * viewMat * projMat;
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

				memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(Daituuren_Toushin::CONSTANT_BUFFER_TOUSHIN));
				deviceContext->Unmap(m_Daituuren_Toushin.m_pVSConstantBuffer, 0);
			}
			deviceContext->VSSetConstantBuffers(0, 1, &m_Daituuren_Toushin.m_pVSConstantBuffer);
			deviceContext->PSSetConstantBuffers(0, 1, &m_Daituuren_Toushin.m_pVSConstantBuffer);

			deviceContext->DrawIndexed(m_Daituuren_Toushin.m_daiturenFile.getVertexIndex().size(), 0, 0);
		}
	}
	*/
	///////////////////////////////
}


HRESULT PMXModel::Body::initShader(){

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

HRESULT PMXModel::Body::initPolygon(){
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

HRESULT PMXModel::Body::initTexture(){
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

HRESULT PMXModel::Body::initBone(){

	return S_OK;
}

/*
PMXModel::Daituuren_Toushin::Daituuren_Toushin()
{
	CreateTexture();
	CreateToushinShader();
	CreateSayaShader();
}


PMXModel::Daituuren_Toushin::~Daituuren_Toushin()
{
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pSamplerState);

	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVSConstantBuffer);
	SAFE_RELEASE(m_pPSConstantBuffer);
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
}

void PMXModel::Daituuren_Toushin::CreateTexture(){
	ID3D11Device* device = Director::instance()->framework()->device();
	ID3D11DeviceContext* deviceContext = Director::instance()->framework()->deviceContext();
	{
		// tex
		char name[] = "dat/pmx/daituren_shoturen_3/tex.jpg";
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

void PMXModel::Daituuren_Toushin::CreateToushinShader(){

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
		int csize = sizeof(CONSTANT_BUFFER_TOUSHIN);
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
void PMXModel::Daituuren_Toushin::CreateSayaShader(){
}

*/