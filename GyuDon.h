#pragma once


// フレームワーク根幹部分を集約しています。ゲーム更新・描画の呼び出し元となります。

#include <Windows.h>
#include <D3D11.h>
#include <D3DX10.h>
#include <D3DX11.h>
#include <D3Dcompiler.h>
#include "lib/MikuMikuFormats//Pmx.h"
#include "INCLUDES.h"
#include "Ref.h"
#ifdef TEST_SIZURU
#include "DebugDrawer.h"
#include "DebugMilliSecondLapper.h"
#endif

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")

#pragma warning(disable : 4305)

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define APP_NAME "PMX読み込みテスト"



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

public:
	const D3DXMATRIX& viewMat();
	const D3DXMATRIX& projMat();

	const D3DXVECTOR3& vEyePt();
	const D3DXVECTOR3& vLookatPt();
	const D3DXVECTOR3& vUpVec();

	const D3DXVECTOR3& getDirectionLight();
	void setDirectionLight(const D3DXVECTOR3& dir);

	ID3D11Device* device();
	ID3D11DeviceContext* deviceContext();

	ID3D11RenderTargetView* renderTargetView();
	ID3D11DepthStencilView* depthStencilView();

	float delta();

private:
	HWND m_hWnd = NULL;
	ID3D11Device* m_pDevice = NULL;
	ID3D11DeviceContext* m_pDeviceContext = NULL;
	IDXGISwapChain* m_pSwapChain = NULL;
	ID3D11RenderTargetView* m_pRenderTargetView = NULL;
	ID3D11DepthStencilView* m_pDepthStencilView = NULL;
	ID3D11Texture2D* m_pDepthStencil = NULL;

	D3DXMATRIX m_projMat;
	D3DXMATRIX m_viewMat;

	D3DXVECTOR3 m_vEyePt;
	D3DXVECTOR3 m_vLookatPt;
	D3DXVECTOR3 m_vUpVec;

	// 差分
	float m_delta = 0.f;
	DWORD m_preTime = 0.f;

	// ディレクションライト方向はプロジェクト全てで共通
	D3DXVECTOR3 m_DirectionLight = D3DXVECTOR3(0.f, 1.f, 0.f);

#ifdef TEST_SIZURU
	DebugDrawer* m_debugDraw = NULL;
	DebugMilliSecondLapper* m_milliSecondLapper = NULL;
public:
	DebugDrawer* debugDrawer();
	DebugMilliSecondLapper* milliSecondLapper();
private:
#endif
};

