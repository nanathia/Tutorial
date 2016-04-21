#include "Oniko.h"
#include "DEFINE.h"
#include "GyuDon.h"
#include "Director.h"
#include "TGATexture.h"
#include "VMDMotion.h"
#include "INCLUDES.h"

int testCount = 0;

void Oniko::draw(){
	ID3D11Device* device = Director::instance()->framework()->device();
	ID3D11DeviceContext* deviceContext = Director::instance()->framework()->deviceContext();
	const D3DXMATRIX& projMat = Director::instance()->framework()->projMat();
	const D3DXMATRIX& viewMat = Director::instance()->framework()->viewMat();
	// テスト用
	D3DXMATRIX mWorld;
	//float val = timeGetTime() / 1000.f;
	float val = 0.f;
	D3DXMatrixRotationY(&mWorld, val);

	const D3DXVECTOR3& vEyePt = Director::instance()->framework()->vEyePt();

#pragma region body

	deviceContext->VSSetShader(m_Body.m_pVertexShader, NULL, 0);
	deviceContext->PSSetShader(m_Body.m_pPixelShader, NULL, 0);

	deviceContext->IASetInputLayout(m_Body.m_pVertexLayout);
	deviceContext->PSSetSamplers(0, 3, m_Body.m_pSamplerState);

	D3DXMATRIX testRota;
	D3DXMatrixRotationAxis(&testRota, &D3DXVECTOR3(0.f, 1.f, 0.f), val*M_PI);

	// モーション更新
	m_testMotion->update();

	// 差分生成。実際に動かしたい値をここで用意しておく。
	for (int i = 0; i < m_Body.m_model.bone_count; i++){
		Body::Bone* bone = &m_Body.m_Bones[i];
		char str[512];
		convert2Str(str, m_Body.m_model.bones[i].bone_name.c_str());
		if (!(m_Body.m_model.bones[i].bone_flag & pmx::PmxBone::FLAG_IK)){
			// Ikボーン以外
			m_testMotion->RenderFrameBone(str, &bone->boneMat);
		}
		else{
			pmx::PmxBone* pmxBone = &m_Body.m_model.bones[i];
			int i = 0;
		}
	}
	// 差分生成。実際に動かしたい値をここで用意しておく。
	//for (int i = 0; i < m_Body.m_model.bone_count; i++){
	//	Body::Bone* bone = &m_Body.m_Bones[i];
	//	char str[512];
	//	convert2Str(str, m_Body.m_model.bones[i].bone_name.c_str());
	//	if (m_Body.m_model.bones[i].bone_flag & pmx::PmxBone::FLAG_IK){
	//		// Ikボーン
	//		m_testMotion->RenderFrameIKBone(str, &bone->boneMat);
	//	}
	//}

	// ボーン情報をレンダリングする。シェーダに渡す形にする。初期姿勢であれば、単位行列になるはずだ。それでテストが出来る。
	struct BoneToShader{
		static void Render(Body::Bone* bone){
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
	BoneToShader::Render(&m_Body.m_Bones[0]);



	// コンスタント配列に押し込む
	//for (int i = 0; i < m_model.bone_count; i++){
	//	Bone* bone = &m_Bones[i];
	//	bone->combMatAry[i] = bone->boneMat;
	//}

	{
		pmx::PmxBone* parentBone = &m_Body.m_model.bones[m_Daituuren_Toushin.m_parentBone_id];

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
	///////////////////////////////

	testCount++;
}

