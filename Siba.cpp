#include "Siba.h"
#include "INCLUDES.h"


Siba::Siba()
{
	Set();
}


Siba::~Siba()
{
	SAFE_RELEASE(m_kusa.m_pVertexLayout);
	SAFE_RELEASE(m_kusa.m_pVertexShader);
	SAFE_RELEASE(m_kusa.m_pPixelShader);
	SAFE_RELEASE(m_kusa.m_pConstantBuffer);
	SAFE_RELEASE(m_kusa.m_pBlendState);
	SAFE_RELEASE(m_kusa.m_pVertexBuffer);
	SAFE_RELEASE(m_kusa.m_pIndexBuffer);
	SAFE_RELEASE(m_kusa.m_pSamplerState);
	SAFE_RELEASE(m_kusa.m_pTexture);
}

