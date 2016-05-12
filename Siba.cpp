#include "Siba.h"
#include "INCLUDES.h"
#include "Director.h"
#include "GyuDon.h"


Siba::Siba()
{
	Set();
}


Siba::~Siba()
{
	BlerEnd();
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
