#pragma once
#include "Ref.h"
#include <D3D11.h>
#include <D3DX10.h>
#include <D3DX11.h>
#include <map>
#include <list>

class DebugDrawer :
	public Ref
{
	ID3D11InputLayout* m_pVertexLayout = NULL;
	ID3D11VertexShader* m_pVertexShader = NULL;
	ID3D11PixelShader* m_pPixelShader = NULL;
	ID3D11Buffer* m_pVertexBuffer = NULL;
	ID3D11Buffer* m_pIndexBuffer = NULL;
	ID3D11SamplerState* m_pSamplerState = NULL;
	ID3D11ShaderResourceView* m_pTexture = NULL;
	ID3D11Buffer* m_pConstantBuffer = NULL;

	//ブレンディングステート生成
	ID3D11BlendState* m_hpBlendState = NULL;

	std::multimap<int, std::string> m_drawStrings;

	int m_fontWidth = -1;
	int m_fontHeight = -1;

public:
	DebugDrawer();
	~DebugDrawer();

	void Set(int fontWidth, int fontHeight);
	void AddDebugString(const std::string& str, int order = 0);

	void draw();
};