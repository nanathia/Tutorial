#pragma once
#include "Ref.h"
#include <D3D11.h>
#include <D3DX10.h>
#include <D3DX11.h>

// ゲームエンジンのGUI部分を総括します
class EngineGUI :
	public Ref
{
	struct MainWindow{
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
		void draw();
		void init();
		void exit();
	};
	MainWindow m_mainWindow;

public:
	EngineGUI();
	~EngineGUI();

	void draw();
};

