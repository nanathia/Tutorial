#include "Oniko.h"
#include "INCLUDES.h"
#include "Director.h"
#include "GyuDon.h"

void Oniko::init(){

	m_Daituuren_Toushin.m_daiturenFile.Load("dat/obj/tuuren", "katana");

	ID3D11Device* device = Director::instance()->framework()->device();
	ID3D11DeviceContext* deviceContext = Director::instance()->framework()->deviceContext();

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

		//D3D11_BUFFER_DESC bd;
		//bd.Usage = D3D11_USAGE_DEFAULT;
		//bd.ByteWidth = sizeof(Daituuren_Toushin::SimpleVertex) * m_Daituuren_Toushin.m_daiturenFile.vertexSize();
		//bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		//bd.CPUAccessFlags = 0;
		//bd.MiscFlags = 0;

		//D3D11_SUBRESOURCE_DATA InitData;
		//InitData.pSysMem = vertices;

		//if (FAILED(device->CreateBuffer(&bd, &InitData, &m_Daituuren_Toushin.m_pVertexBuffer)))
		//{
		//	delete[] vertices;
		//	HALT(h);
		//}
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

	///////////////////////////////////////////////////////////////////////
	ASSERT(m_Body.m_model.bone_count <= 512 && "��Ή��{�[����");

	// �{�[���I�u�W�F�N�g�ƁA�V�F�[�_�ɓn�����߂̔z�񂪕K�v���B
	m_Body.m_Bones = new Body::Bone[m_Body.m_model.bone_count];
	m_Body.m_renderedBones = new D3DXMATRIX[m_Body.m_model.bone_count];

	// �܂��K�v�Ȃ͉̂����H���[�J���{�[�����W���BPMX�ɂ͈ړ����������Ă��Ȃ��B
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

	// �F�X���₷�����邽�߂ɁA�e�q�֌W���Z�b�g���Ă����K�v������B
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

	// ���Ɋe�{�[���̃��[�J���I�t�Z�b�g���B���ۂɃ{�[���𓮂����ɂ͂���������B
	for (int i = 0; i < m_Body.m_model.bone_count; i++){
		D3DXMatrixInverse(&m_Body.m_Bones[i].offsetMat, 0, &m_Body.m_Bones[i].initMat);
	}

	// ���ꂩ���L��p���āA�����p�������[�J������e��_�ɂ���B����ō����s�񂪏�Z�ł���悤�ɂȂ�B
	for (int i = 0; i < m_Body.m_model.bone_count; i++){
		Body::Bone* bone = &m_Body.m_Bones[i];
		if (bone->parent){
			bone->initMat *= bone->parent->offsetMat;
		}
	}

	// �������͊����B

	// �e�X�g�p�B�{�[�����̏��ǉ��B
#ifdef TEST_SIZURU
	for (int i = 0; i < m_Body.m_model.bone_count; i++){
		Body::Bone* bone = &m_Body.m_Bones[i];
		char temp[512];
		convert2Str(temp, m_Body.m_model.bones[i].bone_name.c_str());
		bone->name = temp;
	}
#endif




}