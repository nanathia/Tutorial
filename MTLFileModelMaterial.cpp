#include "MTLFileModelMaterial.h"


namespace obj{

	MTLFileModelMaterial::MTLFileModelMaterial()
	{
		ZeroMemory(&m_material, sizeof(D3DMATERIAL9));
	}


	MTLFileModelMaterial::~MTLFileModelMaterial()
	{
	}

	// �f�B�t���[�Y�F�̐ݒ�
	void MTLFileModelMaterial::setKd(float r, float g, float b){
		m_material.Diffuse.r = r;
		m_material.Diffuse.g = g;
		m_material.Diffuse.b = b;
	}
	// �X�y�L�����F�̐ݒ�
	void MTLFileModelMaterial::setKs(float r, float g, float b){
		m_material.Specular.r = r;
		m_material.Specular.g = g;
		m_material.Specular.b = b;
	}
	// �A���r�G���g�F�̐ݒ�
	void MTLFileModelMaterial::setKa(float r, float g, float b){
		m_material.Ambient.r = r;
		m_material.Ambient.g = g;
		m_material.Ambient.b = b;
	}

	// ���ߓx�i�f�B�t�B�[�Y�A�X�y�L�����̃A���t�@�l�H�j�̐ݒ�
	void MTLFileModelMaterial::setD(float tr){
		m_material.Diffuse.a = tr;
		m_material.Specular.a = tr;
		m_material.Ambient.a = tr;
	}
	// �X�y�L�����̋����̐ݒ�
	void MTLFileModelMaterial::setNs(int Ns){
		m_material.Power = (float)Ns;
	}

	// �e�N�X�`���̐ݒ�
	void MTLFileModelMaterial::setTextureName(const std::string& name){
		m_textureName = name;
	}

	// �e�N�X�`���̎擾
	const std::string& MTLFileModelMaterial::getTextureName(){
		return m_textureName;
	}

	// ���O�̐ݒ�Ǝ擾
	const std::string& MTLFileModelMaterial::getName() const{
		return m_materialName;
	}
	void MTLFileModelMaterial::setName(const std::string& name){
		m_materialName = name;
	}

	// �}�e���A�����擾�B
	const MTLFileModelMaterial::Data& MTLFileModelMaterial::getMaterial() const{
		return m_material;
	}

}