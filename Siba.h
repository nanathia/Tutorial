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
	};
	struct Vertex{
		D3DXVECTOR4 Pos;
		D3DXVECTOR2 UV;
		D3DXVECTOR3 Normal;
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
	Kusa m_kusa;

	void Set();

public:


	void Draw();

	Siba();
	~Siba();
};

