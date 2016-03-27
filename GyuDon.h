#pragma once


// フレームワーク根幹部分を集約しています。ゲーム更新・描画の呼び出し元となります。

#include <Windows.h>
#include <D3D11.h>
#include <D3DX10.h>
#include <D3DX11.h>
#include <D3Dcompiler.h>
#include "lib/MikuMikuFormats//Pmx.h"

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")

#pragma warning(disable : 4305)

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define APP_NAME "PMX読み込みテスト"



struct SimpleVertex
{
	D3DXVECTOR3 Pos;
	D3DXVECTOR3 Normal;
	float Tex[2];
};
struct SIMPLESHADER_CONSTANT_BUFFER
{
	D3DXMATRIX mW;
	D3DXMATRIX mWVP;
	D3DXVECTOR4 vLightDir;
	D3DXVECTOR4 vColor;
	D3DXVECTOR4 vEye;
};

class GyuDon
{


public:
	GyuDon();
	~GyuDon();
	// ウインドウを初期化します。
	HRESULT InitWindow(HINSTANCE, INT, INT, INT, INT, LPCSTR, WNDPROC WndProc);
	// OSからのメッセージのデリゲートです。
	LRESULT MsgProc(HWND, UINT, WPARAM, LPARAM);

	HRESULT InitD3D();
	HRESULT InitPolygon();
	HRESULT InitShader();
	void Loop();
	void App();
	void Render();
	void DestroyD3D();
	HWND m_hWnd;
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	IDXGISwapChain* m_pSwapChain;
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11DepthStencilView* m_pDepthStencilView;
	ID3D11Texture2D* m_pDepthStencil;
	D3DXVECTOR3 m_vLight;

	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11Buffer* m_pConstantBuffer;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	ID3D11SamplerState* m_pSamplerState;
	ID3D11ShaderResourceView** m_pTexture;


	ID3D11Buffer* m_pVertexBufferTest;

	pmx::PmxModel m_model;
};

