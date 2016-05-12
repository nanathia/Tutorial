#include "Siba.h"
#include "DEFINE.h"
#include "INCLUDES.h"

void Siba::BlerEnd(){
	Bler& t = m_bler;
	for (int i = 0; i < 2; i++){
		SAFE_RELEASE(t.m_pRenderTarget[i]);
		SAFE_RELEASE(t.m_pSurface[i]);
	}
	SAFE_RELEASE(t.m_pPixelShader);
	SAFE_RELEASE(t.m_pVertexShader);
	SAFE_RELEASE(t.m_pBlendState);
	SAFE_RELEASE(t.m_pConstantBuffer);
	SAFE_RELEASE(t.m_pIndexBuffer);
	SAFE_RELEASE(t.m_pVertexBuffer);
	SAFE_RELEASE(t.m_pSamplerState);
	SAFE_RELEASE(t.m_pVertexLayout);
	SAFE_RELEASE(t.m_pSimplePixelShader);
}
