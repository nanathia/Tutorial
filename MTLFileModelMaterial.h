#pragma once
#include <string>
#include <d3dx9.h>

namespace obj{

	class MTLFileModelMaterial
	{
		struct Data{
			D3DCOLORVALUE   Diffuse;        /* Diffuse color RGBA */
			D3DCOLORVALUE   Ambient;        /* Ambient color RGB */
			D3DCOLORVALUE   Specular;       /* Specular 'shininess' */
			D3DCOLORVALUE   Emissive;       /* Emissive color RGB */
			float           Power;          /* Sharpness if specular highlight */
		};
		Data m_material;
		std::string m_textureName = "";
		std::string m_materialName = "";

	public:
		// ディフューズ係数の設定
		void setKd(float r, float g, float b);
		// スペキュラ係数の設定
		void setKs(float r, float g, float b);
		// アンビエント係数の設定
		void setKa(float r, float g, float b);

		// 透過度（ディフィーズ、スペキュラのアルファ値？）の設定
		void setD(float tr);
		// スペキュラの強さの設定
		void setNs(int Ns);

		// テクスチャ名の設定
		void setTextureName(const std::string& name);
		// テクスチャの取得
		const std::string& getTextureName();

		// 名前の設定と取得
		const std::string& getName() const;
		void setName(const std::string& name);

		// マテリアルを取得。
		const Data& getMaterial() const;

		MTLFileModelMaterial();
		~MTLFileModelMaterial();

	};

}