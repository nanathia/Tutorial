#pragma once

#include <string>
#include "OBJFileModelGroup.h"
#include "OBJFileModelMesh.h"
#include "MTLFileModelMaterial.h"
#include "MyVector.h"
#include <vector>
#include <D3D11.h>
#include <D3DX10.h>
#include <D3DX11.h>

namespace obj{

	class OBJModelFile
	{
		// ���_�f�[�^
		std::vector<Vec3> m_destxyz;
		std::vector<Vec3> m_destnomal;
		std::vector<Vec2> m_destuv;
		std::vector<unsigned> m_destIndexBuf;

	private:
		std::string m_modelName;
		std::string m_directoryName;
		// ���_����ǂ�
		void vertexRead(char*& data);
		// ���̍s�܂œǂݔ�΂�
		void goNextLine(char*& data);
		// ���̃X�y�[�X�܂œǂݔ�΂��B�X�y�[�X�����������ꍇ��false��Ԃ��B
		bool goNextSpace(char*& data);
		// ���̃X�y�[�X�܂œǂ݂Ƃ΂�������܂ł̕�������擾 �X�y�[�X�����������ꍇ��false��Ԃ��B
		bool goNextSpaceAndGetStr(char*& data, char*& str);
		// ���̃X���b�V��'/'�܂œǂݔ�΂��B�X�y�[�X�܂łɃX���b�V�����������false��Ԃ��B
		bool goNextSlash(char*& data);
		// ���̃X���b�V���܂œǂݔ�΂�����܂ł̕�������擾�@�X�y�[�X�܂łɃX���b�V�����Ȃ������ꍇ�����ŏI����false��Ԃ�
		bool goNextSlashAndGetStr(char*& data, char*& str);
		// ���̕������A�����Ȃ������ł��邩�ǂ����B
		bool isCharcterUnsignedNumber(char c) const;
		// �����Ȃ�������������擾���ǂݔ�΂��B�����Ȃ������ȊO�̕���������܂ł̒����̕�����Ƃ���B�Ō�ɂ��ǂ蒅����������Ԃ��B
		char getUnsignedNumberStr(char*& binary, char*& str);
		// face�f�[�^��p�ŁA���p�`�����ׂf�[�^��ǂݐi�߂�B���s��܂Ői�ނ��Ƃ͖������Ƃ��ۏ؂����B
		int getCountFaceVertexNumByDataLine(char*& binary);
		// ���O�ȂǂɎg���镶���ȊO������܂œǂݔ�΂��擾����B�Ō�ɗ���������Ԃ��B
		char goNextBorderAndGetNameStr(char*& data, char*& str);
		// ���O�Ȃǂɂ��̂܂܎g�p�ł��镶�����ǂ����𔻒肷��
		bool isCanUseNamingCharacter(char c);
		// �����Ȃ����������_�Ɏg���镶���ȊO������܂œǂݔ�΂��擾����
		char goNextBorderAndGetUnsignedFloatStr(char*& binary, char*& str);

		// .obj
		// �f�[�^�̒��̂��ꂼ��̗v�f�̐��𐔂���B
		void countDataXYZ_UV_Nomal_Face(char* data, int& xyz, int& uv, int& nomal, int& faceNum, int& vertBuf, int& indexBuf, int& groupNum, int& meshNum);
		// �ʖ��̒��_�����L�^����B���ꂩ��A�K�v�ȃf�[�^�C���f�N�X�́A�������ޗ̈���m�ۂ���B
		void countVertexForFace(char* binary, std::vector<std::vector<Vec3I>>& dataIndexList, std::vector<OBJFileModelGroup>& groupList, std::vector<OBJFileModelMesh>& meshList);
		// ���_�o�b�t�@�ƃC���f�N�X�o�b�t�@���擾����B
		void getVertexAndIndexBuffer(std::vector<Vec3>& srcXYZ, std::vector<Vec2>& srcUV, std::vector<Vec3>& srcNomal, std::vector<Vec3>& destXYZ, std::vector<Vec2>& destUV, std::vector<Vec3>& destNomal, std::vector<std::vector<Vec3I>>& dataIndexData, std::vector<unsigned>& destIndexBuf);
		// .obj�f�[�^�����[�h����B
		void loadObjData(char* binary, std::vector<Vec3>& xyz, std::vector<Vec3>& nomal, std::vector<Vec2>& uv, std::vector<std::vector<Vec3I>>& dataIndexList);

		// .mtl
		// �}�e���A���̐��𐔂���B
		void countMaterialByBinaryData(char* binary, int& materialNum);
		// �}�e���A�����Z�b�g����B
		void setMaterialByBinaryData(char* binary, std::vector<MTLFileModelMaterial>& materialList);

		// .obj�t�@�C���Ɋւ��鏈��
		void OBJDataLoadAndSetting();
		// .mtl�t�@�C���Ɋւ��鏈��
		void MTLDataLoadAndSetting();

		// �ŏI�����B
		void setMeshByMaterialData();

		// ���̃��f���́A�O�p�`�̐�
		int m_primitiveCount = -1;
		// ���̃��f���́A���_�̐��B
		int m_vertexCount = -1;

		// ���̃��f�����̃O���[�v�̃��X�g
		std::vector<OBJFileModelGroup> m_groupList;
		// ���̃��f�����̃��b�V���̃��X�g
		std::vector<OBJFileModelMesh> m_meshList;
		// ���̃��f�����̃}�e���A���̃��X�g
		std::vector<MTLFileModelMaterial> m_materialList;
		// ���̃��f�����̃e�N�X�`���̃��X�g
		std::vector<std::string> m_textureNameList;

	public:
		OBJModelFile();
		virtual ~OBJModelFile();

		void Load(const std::string& directory, const std::string& modelName);
		
		const std::vector<Vec3>& getPosition();
		const std::vector<unsigned>& getVertexIndex();
		const std::vector<Vec3>& getNormal();
		const std::vector<Vec2>& getUV();
		const std::vector<std::string> getTextureName();

		int vertexSize();
		int vertexIndexSize();
		int getTextureCount();

	};

}