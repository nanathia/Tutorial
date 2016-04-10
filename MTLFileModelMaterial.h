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
		// �f�B�t���[�Y�W���̐ݒ�
		void setKd(float r, float g, float b);
		// �X�y�L�����W���̐ݒ�
		void setKs(float r, float g, float b);
		// �A���r�G���g�W���̐ݒ�
		void setKa(float r, float g, float b);

		// ���ߓx�i�f�B�t�B�[�Y�A�X�y�L�����̃A���t�@�l�H�j�̐ݒ�
		void setD(float tr);
		// �X�y�L�����̋����̐ݒ�
		void setNs(int Ns);

		// �e�N�X�`�����̐ݒ�
		void setTextureName(const std::string& name);
		// �e�N�X�`���̎擾
		const std::string& getTextureName();

		// ���O�̐ݒ�Ǝ擾
		const std::string& getName() const;
		void setName(const std::string& name);

		// �}�e���A�����擾�B
		const Data& getMaterial() const;

		MTLFileModelMaterial();
		~MTLFileModelMaterial();

	};

}