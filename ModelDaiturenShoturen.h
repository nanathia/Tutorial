#pragma once
#include "Ref.h"
#include <D3D11.h>
#include <D3DX10.h>
#include <D3DX11.h>
#include "OBJModelFile.h"

// 大通連＆小通連。

class ModelDaiturenShoturen :
	public Ref
{
	// 大通連刀身
	obj::OBJModelFile m_daiturenFile;

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

public:
	ModelDaiturenShoturen();
	~ModelDaiturenShoturen();

	void draw();
};

