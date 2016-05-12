#pragma once
#include <D3D11.h>
#include <D3DX10.h>
#include <D3DX11.h>

#include "Ref.h"


// é≈ê∂

class Siba :
	public Ref
{
	struct CONSTANT{
		D3DXMATRIX mWVP;
		D3DXMATRIX mW;
		D3DXVECTOR3 vLight;
		D3DXVECTOR3 vEye;
	};
	struct Vertex{
		D3DXVECTOR4 Pos;
		D3DXVECTOR2 UV;
		D3DXVECTOR3 Normal;
	};

	struct BlerConstant{
		float offsets[15];
	};
	struct BlerVertex{
		D3DXVECTOR4 Pos;
		D3DXVECTOR2 Tex;
	};

	struct Kusa{
		ID3D11InputLayout* m_pVertexLayout = NULL;
		ID3D11VertexShader* m_pVertexShader = NULL;
		ID3D11PixelShader* m_pPixelShader = NULL;
		ID3D11Buffer* m_pConstantBuffer = NULL;
		ID3D11Buffer* m_pVertexBuffer = NULL;
		ID3D11Buffer* m_pIndexBuffer = NULL;
		ID3D11SamplerState* m_pSamplerState = NULL;
		ID3D11ShaderResourceView* m_pTexture = NULL;
		ID3D11BlendState* m_pBlendState = NULL;
	};

	struct Bler{
		ID3D11RenderTargetView* m_pRenderTarget[2];
		ID3D11ShaderResourceView* m_pSurface[2];
		ID3D11VertexShader* m_pVertexShader = NULL;
		ID3D11PixelShader* m_pPixelShader = NULL;
		ID3D11InputLayout* m_pVertexLayout = NULL;
		ID3D11Buffer* m_pConstantBuffer = NULL;
		ID3D11Buffer* m_pVertexBuffer = NULL;
		ID3D11Buffer* m_pIndexBuffer = NULL;
		ID3D11BlendState* m_pBlendState = NULL;
		ID3D11SamplerState* m_pSamplerState = NULL;
		ID3D11PixelShader* m_pSimplePixelShader = NULL;
	};

	Kusa m_kusa;
	Bler m_bler;

	void Set();

	void BlerDraw();
	void BlerSet();
	void BlerEnd();

public:


	void Draw();

	Siba();
	~Siba();
};

