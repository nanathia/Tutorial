#pragma once
#include "Ref.h"
#include <D3D11.h>
#include <D3DX10.h>
#include <D3DX11.h>
#include "OBJModelFile.h"

// ��ʘA�����ʘA�B

class ModelDaiturenShoturen :
	public Ref
{
	// ��ʘA���g
	obj::OBJModelFile m_daiturenFile;

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

public:
	ModelDaiturenShoturen();
	~ModelDaiturenShoturen();

	void draw();
};

