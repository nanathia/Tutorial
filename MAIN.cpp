
#include "TGATexture.h"
#include "Director.h"
#include "GyuDon.h"
#include "SceneManager.h"
#include <D3DX11.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, INT)
{
	Director::create();
	if (Director::instance()->framework() != NULL)
	{
		if (SUCCEEDED(Director::instance()->framework()->InitWindow(hInstance, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, APP_NAME, WndProc)))
		{
			if (SUCCEEDED(Director::instance()->framework()->InitD3D()))
			{
				Director::instance()->scene()->initScene();
				Director::instance()->framework()->Loop();
			}
		}
	}
	Director::destroy();
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return Director::instance()->framework()->MsgProc(hWnd, uMsg, wParam, lParam);
}