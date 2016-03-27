#pragma once


// フレームワーク根幹部分を集約しています。ゲーム更新・描画の呼び出し元となります。

#include <Windows.h>
#include <D3D11.h>
#include <D3DX10.h>
#include <D3DX11.h>
#include <D3Dcompiler.h>
#include "lib/MikuMikuFormats//Pmx.h"
#include "Ref.h"

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

class GyuDon : public Ref
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

	HWND m_hWnd = NULL;
	ID3D11Device* m_pDevice = NULL;
	ID3D11DeviceContext* m_pDeviceContext = NULL;
	IDXGISwapChain* m_pSwapChain = NULL;
	ID3D11RenderTargetView* m_pRenderTargetView = NULL;
	ID3D11DepthStencilView* m_pDepthStencilView = NULL;
	ID3D11Texture2D* m_pDepthStencil = NULL;
	D3DXVECTOR3 m_vLight;

	ID3D11InputLayout* m_pVertexLayout = NULL;
	ID3D11VertexShader* m_pVertexShader = NULL;
	ID3D11PixelShader* m_pPixelShader = NULL;
	ID3D11Buffer* m_pConstantBuffer = NULL;
	ID3D11Buffer* m_pVertexBuffer = NULL;
	ID3D11Buffer* m_pIndexBuffer = NULL;
	ID3D11SamplerState* m_pSamplerState = NULL;
	ID3D11ShaderResourceView** m_pTexture = NULL;


	ID3D11Buffer* m_pVertexBufferTest = NULL;

	pmx::PmxModel m_model;
};

