#include <D3D11.h>
#include <D3DX10.h>
#include <D3DX11.h>
#include <cmath>

//void Bezier(D3DXVECTOR2* out, const D3DXVECTOR2& p0, const D3DXVECTOR2& p1, const D3DXVECTOR2& p2, const D3DXVECTOR2& p3, float t){
//	//D3DXVECTOR3 p0(0.f, 0.f, 0.f);
//	//D3DXVECTOR3 p3(1.f, 1.f, 1.f);
//	float u = 1.f - t;
//	float u2 = u*u;
//	float u3 = u2*u;
//	float t2 = t*t;
//	float t3 = t2*t;
//	*out = p0*u3 + p1*(3 * u2*t) + p2*(3 * u*t2) + p3*t3;
//}

float Lerp(float p1, float p2, float t){
	return p1 + t * (p2 - p1);
}

//ベジェ曲線からWeightを算出
float interpolateBezier(const D3DXVECTOR2& Interpolation1, const D3DXVECTOR2& Interpolation2, float frameInitilize){
	//ベジェ曲線指示点
	float const x1 = Interpolation1[0] / 127.f;
	float const y1 = Interpolation1[1] / 127.f;
	float const x2 = Interpolation2[0] / 127.f;
	float const y2 = Interpolation2[1] / 127.f;

	//ニュートン法による3次方程式の解導出
	//[x0=0, x3=1, x=frameInitilize, s=t-1]
	//f(t) = 3*s*s*t*x1 + 3*s*t*t*x2 + t*t*t - frameInitilize = 0 となる。

	float limitDt = 0.001f; //閾値
	float t = 0.5f; //初期値(適当)
	float s = 1.0f - t;

	for (int i = 0; i<10; i++){
		float dt = 3 * s*s*t*x1 + 3 * s*t*t*x2 + t*t*t - frameInitilize;
		if (fabsf(dt) < limitDt) break; //許容誤差チェック
		//f(t)を微分し傾き算出
		float slope = 3 * s*s*x1 + 3 * 2 * s*t*x2 + 3 * t*t - frameInitilize;
		t = t - dt / slope;
		s = 1.0f - t;
	}
	return (3 * s*s*t*y1) + (3 * s*t*t*y2) + (t*t*t);
}

float Clamp(float x, float minval, float maxval)
{
	return min(max(minval, x), maxval);
}