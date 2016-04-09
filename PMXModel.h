#pragma once
#include "Ref.h"

// PMXの基本的なモデル描画です。

#include <Windows.h>
#include <D3D11.h>
#include <D3DX10.h>
#include <D3DX11.h>
#include <D3Dcompiler.h>
#include "lib/MikuMikuFormats/Pmx.h"
#include "INCLUDES.h"

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")

#include "lib/MikuMikuFormats/Pmx.h"


class PMXModel :
	public Ref
{
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

	// ライトベクトル
	D3DXVECTOR3 m_vLight;
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

public:
	PMXModel(const char* directory, const char* fileName);
	~PMXModel();

	void draw();

private:
	HRESULT initShader();
	HRESULT initPolygon();
	HRESULT initTexture();
	HRESULT initBone();
};

