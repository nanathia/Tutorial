#pragma once
#include "Ref.h"

// PMX�̊�{�I�ȃ��f���`��ł��B

#include <Windows.h>
#include <D3D11.h>
#include <D3DX10.h>
#include <D3DX11.h>
#include <D3Dcompiler.h>
#include "lib/MikuMikuFormats/Pmx.h"
#include "Ref.h"

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")

#include "lib/MikuMikuFormats/Pmx.h"

class PMXModel :
	public Ref
{
	// ���f�[�^
	pmx::PmxModel m_model;

	// ���C�g�x�N�g��
	D3DXVECTOR3 m_vLight;
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

public:
	PMXModel(const char* directory, const char* fileName);
	~PMXModel();

	void draw();

private:
	HRESULT initShader();
	HRESULT initPolygon();
	HRESULT initTexture();
};

