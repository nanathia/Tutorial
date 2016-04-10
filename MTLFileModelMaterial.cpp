#include "MTLFileModelMaterial.h"


namespace obj{

	MTLFileModelMaterial::MTLFileModelMaterial()
	{
		ZeroMemory(&m_material, sizeof(D3DMATERIAL9));
	}


	MTLFileModelMaterial::~MTLFileModelMaterial()
	{
	}

	// ディフューズ色の設定
	void MTLFileModelMaterial::setKd(float r, float g, float b){
		m_material.Diffuse.r = r;
		m_material.Diffuse.g = g;
		m_material.Diffuse.b = b;
	}
	// スペキュラ色の設定
	void MTLFileModelMaterial::setKs(float r, float g, float b){
		m_material.Specular.r = r;
		m_material.Specular.g = g;
		m_material.Specular.b = b;
	}
	// アンビエント色の設定
	void MTLFileModelMaterial::setKa(float r, float g, float b){
		m_material.Ambient.r = r;
		m_material.Ambient.g = g;
		m_material.Ambient.b = b;
	}

	// 透過度（ディフィーズ、スペキュラのアルファ値？）の設定
	void MTLFileModelMaterial::setD(float tr){
		m_material.Diffuse.a = tr;
		m_material.Specular.a = tr;
		m_material.Ambient.a = tr;
	}
	// スペキュラの強さの設定
	void MTLFileModelMaterial::setNs(int Ns){
		m_material.Power = (float)Ns;
	}

	// テクスチャの設定
	void MTLFileModelMaterial::setTextureName(const std::string& name){
		m_textureName = name;
	}

	// テクスチャの取得
	const std::string& MTLFileModelMaterial::getTextureName(){
		return m_textureName;
	}

	// 名前の設定と取得
	const std::string& MTLFileModelMaterial::getName() const{
		return m_materialName;
	}
	void MTLFileModelMaterial::setName(const std::string& name){
		m_materialName = name;
	}

	// マテリアルを取得。
	const MTLFileModelMaterial::Data& MTLFileModelMaterial::getMaterial() const{
		return m_material;
	}

}