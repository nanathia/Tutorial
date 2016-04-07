#pragma once
#include "Ref.h"
#include <D3D11.h>
#include <D3DX10.h>
#include <D3DX11.h>

class Ground :
	public Ref
{
	ID3D11SamplerState* m_pSamplerState;
	ID3D11ShaderResourceView* m_pTexture = NULL;

public:
	void update();
	void draw();

	Ground(const char* texFile = "white.png");
	~Ground();
};

