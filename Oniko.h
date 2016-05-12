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
	// テストモーションですね。
	VMDMotion* m_testMotion = 0;

	// お体の部分ですね。
	struct Body{
		struct SimpleVertex
		{
			D3DXVECTOR4 Pos;
			unsigned vBoneIndices[4];
			D3DXVECTOR4 vBoneWeights; // 最大四点ウェイト
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
		// 生データ
		pmx::PmxModel m_model;

		// 頂点レイアウト
		ID3D11InputLayout* m_pVertexLayout = NULL;
		ID3D11VertexShader* m_pVertexShader = NULL;
		ID3D11PixelShader* m_pPixelShader = NULL;
		ID3D11Buffer* m_pVSConstantBuffer = NULL;
		ID3D11Buffer* m_pPSConstantBuffer = NULL;
		ID3D11Buffer* m_pVertexBuffer = NULL;
		ID3D11Buffer* m_pIndexBuffer = NULL;
		ID3D11SamplerState* m_pSamplerState[3];
		ID3D11ShaderResourceView** m_pTexture = NULL;
		// コンスタントサイズ
		int m_vertexConstantSize = 0;

		// シェーダに渡すボーン配列。シェーダに渡すために、一列でなければならない。
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
	
	// 大通連の刀身になりますね
	struct Daituuren_Toushin{
		struct SimpleVertex{
			D3DXVECTOR4 position;
			D3DXVECTOR2 uv;
			D3DXVECTOR3 normal;
		};
		struct CONSTANT_BUFFER_TOUSHIN{
			D3DXMATRIX g_mW;//ワールド行列
			D3DXMATRIX g_mWVP; //ワールドから射影までの変換行列
			D3DXVECTOR4 g_vLightDir;//ライトの方向ベクトル
			D3DXVECTOR4 g_Diffuse; //拡散反射(色）
			D3DXVECTOR4 g_vEye;	//カメラ（視点）
		};
		// 大通連刀身
		obj::OBJModelFile m_daiturenFile;

		// お体の中に親ボーン
		int m_parentBone_id = -1;
		// 初期姿勢
		D3DXMATRIX m_InitMat;
		// 仮想ボーン
		Body::Bone m_bone;

		// 頂点レイアウト
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

