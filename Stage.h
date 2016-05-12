#pragma once
#include "Ref.h"
#include <D3D11.h>
#include <D3DX10.h>
#include <D3DX11.h>
#include "OBJModelFile.h"

class Stage :
	public Ref
{
	struct Common{
		struct SimpleVertex{
			D3DXVECTOR4 position;
			D3DXVECTOR2 uv;
			D3DXVECTOR3 normal;
		};
		struct CONSTANT{
			D3DXMATRIX g_mW;//ワールド行列
			D3DXMATRIX g_mWVP; //ワールドから射影までの変換行列
			D3DXVECTOR4 g_vLightDir;//ライトの方向ベクトル
			D3DXVECTOR4 g_Diffuse; //拡散反射(色）
			D3DXVECTOR4 g_vEye;	//カメラ（視点）
		};
		// 大通連刀身
		obj::OBJModelFile m_file;

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

		void CreateTexture();
		void CreateShader();
		void CreateVertex();

		void Draw();

		Common();
		~Common();
	};
	Common m_common;
public:
	Stage();
	~Stage();

	void Draw();
};

