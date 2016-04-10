#pragma once
#include "DEFINE.h"
#include "MTLFileModelMaterial.h"

namespace obj{

	// .obj�t�@�C���̕`��P�ʂł��B

	class OBJFileModelMesh
	{
		// �}�e���A����
		std::string m_materialName = "";
		// ���_�o�b�t�@�J�n�n�_
		int m_vertexStart;
		// ���_�o�b�t�@�g�p�T�C�Y
		int m_vertexSize;
		// �C���f�N�X�o�b�t�@�J�n�n�_
		int m_indexStart;
		// �C���f�N�X�o�b�t�@�g�p�T�C�Y
		int m_indexSize;
		// �}�e���A���̃|�C���^
		MTLFileModelMaterial* m_material = 0;

	public:

		void setVertexStart(int index);
		void setVertexSize(int size);
		void setIndexStart(int index);
		void setIndexSize(int size);
		void setMaterialName(const std::string& name);

		int getVertexStart() const;
		int getVertexSize() const;
		int getIndexStart() const;
		int getIndexSize() const;
		const std::string& getMaterialName() const;

		void setMaterial(MTLFileModelMaterial* material);
		MTLFileModelMaterial* getMaterial();

		OBJFileModelMesh();
		~OBJFileModelMesh();

	};

}