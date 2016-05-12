#pragma once


#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")

#include <D3DX11.h>

#include "Ref.h"
#include "PMXModel.h"
#include "NomalModel.h"
#include "OBJModelFile.h"

class ModelDaiturenShoturen;
class VMDMotion;

class Oniko : public Ref
{
	// �e�X�g���[�V�����ł��ˁB
	VMDMotion* m_testMotion = 0;

	// ���̂̕����ł��ˁB
	struct Body{
		struct SimpleVertex
		{
			D3DXVECTOR4 Pos;
			unsigned vBoneIndices[4];
			D3DXVECTOR4 vBoneWeights; // �ő�l�_�E�F�C�g
			unsigned vWeitFormula;
			D3DXVECTOR3 Normal;
			float Tex[2];
		};
		struct PMX_VS_CONSTANT_BUFFER
		{
			D3DXMATRIX mV;
			D3DXMATRIX mP;
			D3DXMATRIX test;
			D3DXMATRIX mW[512];
		};
		struct PMX_PS_CONSTANT_BUFFER{
			D3DXVECTOR3 ambient;
			D3DXVECTOR4 diffuse;
			D3DXVECTOR3 specular;
			float shininess;

			D3DXVECTOR3 lightDirection; //direction towards the light
			D3DXVECTOR3 halfVector; //surface orientation for shiniest spots- the vector stretching from from the camera position to the target

			bool isEdge;
			D3DXVECTOR4 edgeColor;
			float edgeSize;

			float fSphereMode; //sphereMode stored as a float
		};
		struct Bone{
#ifdef TEST_SIZURU
			std::string name;
#endif
			int id;
			Bone *parent = NULL;
			Bone *firstChild = NULL;
			Bone *sibling = NULL;

			D3DXMATRIX offsetMat;
			D3DXMATRIX initMat;
			D3DXMATRIX boneMat;
			D3DXMATRIX *combMatAry = NULL;
		};
		// ���f�[�^
		pmx::PmxModel m_model;

		// ���_���C�A�E�g
		ID3D11InputLayout* m_pVertexLayout = NULL;
		ID3D11VertexShader* m_pVertexShader = NULL;
		ID3D11PixelShader* m_pPixelShader = NULL;
		ID3D11Buffer* m_pVSConstantBuffer = NULL;
		ID3D11Buffer* m_pPSConstantBuffer = NULL;
		ID3D11Buffer* m_pVertexBuffer = NULL;
		ID3D11Buffer* m_pIndexBuffer = NULL;
		ID3D11SamplerState* m_pSamplerState[3];
		ID3D11ShaderResourceView** m_pTexture = NULL;
		// �R���X�^���g�T�C�Y
		int m_vertexConstantSize = 0;

		// �V�F�[�_�ɓn���{�[���z��B�V�F�[�_�ɓn�����߂ɁA���łȂ���΂Ȃ�Ȃ��B
		D3DXMATRIX* m_renderedBones = NULL;
		Bone* m_Bones = NULL;

		// file directory
		std::string m_directoryName;
		std::string m_fileName;

		HRESULT initShader();
		HRESULT initPolygon();
		HRESULT initTexture();
		HRESULT initBone();
	}m_Body;
	
	// ��ʘA�̓��g�ɂȂ�܂���
	struct Daituuren_Toushin{
		struct SimpleVertex{
			D3DXVECTOR4 position;
			D3DXVECTOR2 uv;
			D3DXVECTOR3 normal;
		};
		struct CONSTANT_BUFFER_TOUSHIN{
			D3DXMATRIX g_mW;//���[���h�s��
			D3DXMATRIX g_mWVP; //���[���h����ˉe�܂ł̕ϊ��s��
			D3DXVECTOR4 g_vLightDir;//���C�g�̕����x�N�g��
			D3DXVECTOR4 g_Diffuse; //�g�U����(�F�j
			D3DXVECTOR4 g_vEye;	//�J�����i���_�j
		};
		// ��ʘA���g
		obj::OBJModelFile m_daiturenFile;

		// ���̂̒��ɐe�{�[��
		int m_parentBone_id = -1;
		// �����p��
		D3DXMATRIX m_InitMat;
		// ���z�{�[��
		Body::Bone m_bone;

		// ���_���C�A�E�g
		ID3D11InputLayout* m_pVertexLayout = NULL;
		ID3D11VertexShader* m_pVertexShader = NULL;
		ID3D11PixelShader* m_pPixelShader = NULL;
		ID3D11Buffer* m_pVSConstantBuffer = NULL;
		ID3D11Buffer* m_pPSConstantBuffer = NULL;
		ID3D11Buffer* m_pVertexBuffer = NULL;
		ID3D11Buffer* m_pIndexBuffer = NULL;
		ID3D11SamplerState* m_pSamplerState = NULL;
		ID3D11ShaderResourceView* m_pTexture = NULL;

		void CreateDaituren();

		void CreateTexture();
		void CreateToushinShader();
		void CreateSayaShader();

		Daituuren_Toushin();
		~Daituuren_Toushin();
	}m_Daituuren_Toushin;


public:
	Oniko();
	~Oniko();

	void draw();

private:
	void init();

};

