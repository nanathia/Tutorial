#include "Oniko.h"
#include "INCLUDES.h"
#include "Director.h"
#include "GyuDon.h"

void Oniko::init(){

	ID3D11Device* device = Director::instance()->framework()->device();
	ID3D11DeviceContext* deviceContext = Director::instance()->framework()->deviceContext();

	///////////////////////////////////////////////////////////////////////
	ASSERT(m_Body.m_model.bone_count <= 512 && "非対応ボーン数");

	// ボーンオブジェクトと、シェーダに渡すための配列が必要だ。
	m_Body.m_Bones = new Body::Bone[m_Body.m_model.bone_count];
	m_Body.m_renderedBones = new D3DXMATRIX[m_Body.m_model.bone_count];

	// まず必要なのは何か？ローカルボーン座標だ。PMXには移動しか入っていない。
	for (int i = 0; i < m_Body.m_model.bone_count; i++){
		pmx::PmxBone& data = m_Body.m_model.bones[i];
		Body::Bone* out = &m_Body.m_Bones[i];
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
		Body::Bone* child = &m_Body.m_Bones[i];
		if (childData.parent_index != -1){
			Body::Bone* parent = &m_Body.m_Bones[childData.parent_index];
			if (parent->firstChild){
				Body::Bone* sibling = parent->firstChild;
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
		Body::Bone* bone = &m_Body.m_Bones[i];
		if (bone->parent){
			bone->initMat *= bone->parent->offsetMat;
		}
	}

	// 初期化は完了。

	// テスト用。ボーン名の情報追加。
#ifdef TEST_SIZURU
	for (int i = 0; i < m_Body.m_model.bone_count; i++){
		Body::Bone* bone = &m_Body.m_Bones[i];
		char temp[512];
		convert2Str(temp, m_Body.m_model.bones[i].bone_name.c_str());
		bone->name = temp;
	}
#endif


////////////////////////////////////////////////////////////////////////////

	//pmx::PmxBone test = m_body

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
		D3DXMatrixTranslation(&InitTrance, parentBone->position[0]+0.1f, parentBone->position[1]-0.1f, parentBone->position[2]);
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
}