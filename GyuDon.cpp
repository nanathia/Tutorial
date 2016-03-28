#include "GyuDon.h"
#include "TGATexture.h"
#include "DEFINE.h"
#include "Director.h"
#include "SceneBase.h"
#include "SceneManager.h"

// DirectXŒn‚Ì‰ð•úƒ}ƒNƒ‚Å‚·B
#define SAFE_RELEASE(x) if (x){ x->Release(); x; }


GyuDon::GyuDon()
{
}


GyuDon::~GyuDon()
{
	this->DestroyD3D();
}


HRESULT GyuDon::InitWindow(HINSTANCE hInstance, INT iX, INT iY, INT iWidth, INT iHeight, LPCSTR WindowName, WNDPROC WndProc)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.lpszClassName = WindowName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&wc);

	m_hWnd = CreateWindow(WindowName, WindowName, WS_OVERLAPPEDWINDOW, 0, 0, iWidth, iHeight, 0, 0, hInstance, 0);
	if (!m_hWnd)
	{
		return E_FAIL;
	}

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return S_OK;
}

LRESULT GyuDon::MsgProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_KEYDOWN:
		switch ((char)wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

void GyuDon::Loop()
{
	MSG msg = { 0 };
	DWORD pre = timeGetTime();
	ZeroMemory(&msg, sizeof(msg));

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			App();
			while (pre + 16 > timeGetTime()){
				Sleep(1);
			}
			pre = timeGetTime();
		}
	}
}

void GyuDon::App()
{
	Director::instance()->scene()->update();
	Director::instance()->autoReleasePool()->update();
	Render();
}

HRESULT GyuDon::InitD3D()
{
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = WINDOW_WIDTH;
	sd.BufferDesc.Height = WINDOW_HEIGHT;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = m_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL* pFeatureLevel = NULL;

	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &pFeatureLevels, 1, D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pDevice, pFeatureLevel, &m_pDeviceContext)))
	{
		return FALSE;
	}

	ID3D11Texture2D *pBackBuffer;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
	SAFE_RELEASE(pBackBuffer);

	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = WINDOW_WIDTH;
	descDepth.Height = WINDOW_HEIGHT;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	m_pDevice->CreateTexture2D(&descDepth, NULL, &m_pDepthStencil);
	m_pDevice->CreateDepthStencilView(m_pDepthStencil, NULL, &m_pDepthStencilView);

	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	D3D11_VIEWPORT vp;
	vp.Width = WINDOW_WIDTH;
	vp.Height = WINDOW_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pDeviceContext->RSSetViewports(1, &vp);
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc, sizeof(rdc));
	rdc.CullMode = D3D11_CULL_BACK;
	rdc.FillMode = D3D11_FILL_SOLID;
	ID3D11RasterizerState* pIr = NULL;
	m_pDevice->CreateRasterizerState(&rdc, &pIr);
	m_pDeviceContext->RSSetState(pIr);
	SAFE_RELEASE(pIr);

	return S_OK;
}

void GyuDon::DestroyD3D()
{
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pDepthStencil);
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pDevice);
}

HRESULT GyuDon::InitShader()
{

	return S_OK;
}

void GyuDon::Render()
{
	float ClearColor[4] = { 0, 0, 1, 1 };
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	m_vEyePt = D3DXVECTOR3(0.0f, 20.0f, -15.0f);
	m_vLookatPt = D3DXVECTOR3(0.0f, 15.0f, 0.0f);
	m_vUpVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&m_viewMat, &m_vEyePt, &m_vLookatPt, &m_vUpVec);

	D3DXMatrixPerspectiveFovLH(&m_projMat, D3DX_PI / 4, (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f, 10000.0f);

	Director::instance()->scene()->draw();

	m_pSwapChain->Present(0, 0);
}

const D3DXMATRIX& GyuDon::viewMat(){
	return m_viewMat;
}
const D3DXMATRIX& GyuDon::projMat(){
	return m_projMat;
}
ID3D11Device* GyuDon::device(){
	return m_pDevice;
}
ID3D11DeviceContext* GyuDon::deviceContext(){
	return m_pDeviceContext;
}
const D3DXVECTOR3& GyuDon::vEyePt(){
	return m_vEyePt;
}
const D3DXVECTOR3& GyuDon::vLookatPt(){
	return m_vLookatPt;
}
const D3DXVECTOR3& GyuDon::vUpVec(){
	return m_vUpVec;
}