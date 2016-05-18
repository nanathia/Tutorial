#pragma once
#include "Ref.h"
#include <D3D11.h>
#include <D3DX10.h>
#include <D3DX11.h>


class UINode;
// 2DUIを描画するためのセット項目を全て管理するクラスです。

class UIDrawSetter :
	public Ref
{
	ID3D11InputLayout* m_pVertexLayout = NULL;
	ID3D11VertexShader* m_pVertexShader = NULL;
	ID3D11PixelShader* m_pPixelShader = NULL;
	ID3D11Buffer* m_pConstantBuffer = NULL;
	ID3D11Buffer* m_pVertexBuffer = NULL;
	ID3D11Buffer* m_pIndexBuffer = NULL;
	ID3D11BlendState* m_hpBlendState = NULL;
	struct CONSTANT{
		D3DXMATRIX mWVP; //ワールドから射影までの変換行列
		D3DXVECTOR4 Diffuse = D3DXVECTOR4(1, 0, 0, 1); //拡散反射(色）	
	};
	struct Vertex{
		D3DXVECTOR4 Pos;
		D3DXVECTOR2 UV;
	};

	void init();
	void exit();

public:
	UIDrawSetter();
	~UIDrawSetter();

	void draw(UINode* grandParentNode);

};

