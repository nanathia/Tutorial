#pragma once
#include <string>

namespace obj{

	class OBJFileModelGroup
	{
		// �O���[�v��
		std::string m_groupName = "";
		// ���_�o�b�t�@�J�n�n�_
		int m_vertexStart;
		// ���_�o�b�t�@�g�p�T�C�Y
		int m_vertexSize;
		// �C���f�N�X�o�b�t�@�J�n�n�_
		int m_indexStart;
		// �C���f�N�X�o�b�t�@�g�p�T�C�Y
		int m_indexSize;

	public:
		void setVertexStart(int index);
		void setVertexSize(int size);
		void setIndexStart(int index);
		void setIndexSize(int size);
		void setGroupName(const std::string& name);

		int getVertexStart() const;
		int getVertexSize() const;
		int getIndexStart() const;
		int getIndexSize() const;
		const std::string& getGroupName() const;

		OBJFileModelGroup();
		~OBJFileModelGroup();
	};

}