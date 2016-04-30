#pragma once
#include <D3D11.h>
#include <D3DX10.h>

//void Bezier(D3DXVECTOR2* out, const D3DXVECTOR2& p0, const D3DXVECTOR2& p1, const D3DXVECTOR2& p2, const D3DXVECTOR2& p3, float t);

extern float Lerp(float p1, float p2, float t);

//ベジェ曲線からWeightを算出
extern float interpolateBezier(const D3DXVECTOR2& Interpolation1, const D3DXVECTOR2& Interpolation2, float frameInitilize);

extern float Clamp(float x, float minval, float maxval);