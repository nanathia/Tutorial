#pragma once
#include <D3D11.h>
#include <D3DX10.h>
#include <D3DX11.h>

//void Bezier(D3DXVECTOR2* out, const D3DXVECTOR2& p0, const D3DXVECTOR2& p1, const D3DXVECTOR2& p2, const D3DXVECTOR2& p3, float t);

struct MyFunc{

	static float Lerp(float p1, float p2, float t);

	//ベジェ曲線からWeightを算出
	static float interpolateBezier(const D3DXVECTOR2& Interpolation1, const D3DXVECTOR2& Interpolation2, float frameInitilize);

	static float Clamp(float x, float minval, float maxval);

};