#include "Oniko.h"
#include "DEFINE.h"
#include "GyuDon.h"
#include "Director.h"
#include "TGATexture.h"
#include "lib\MikuMikuFormats\Pmx.h"
#include "VMDMotion.h"
#include <string>

#define LIMIT_BONE_COUNT 512
	
// DirectX系の解放マクロです。
#define SAFE_RELEASE(x) if (x){ x->Release(); x; }


Oniko::Oniko()
{
	// test
	OBJECT_CREATE(m_testMotion, new VMDMotion("dat/vmd/d.vmd", 0.25f));

	m_Body.m_directoryName = "dat/pmx/oniko";
	m_Body.m_fileName = "日本鬼子0.15.pmx";
	ASSERT(SUCCEEDED(m_Body.initShader()));
	ASSERT(SUCCEEDED(m_Body.initPolygon()));
	ASSERT(SUCCEEDED(m_Body.initTexture()));

	this->init();
}


Oniko::~Oniko()
{
	OBJECT_RELEASE(m_testMotion);
	delete[] m_Body.m_renderedBones;
	m_Body.m_renderedBones = 0;
	SAFE_DELETE_ARRAY(m_Body.m_Bones);
	for (int i = 0; i < 3; i++){
		SAFE_RELEASE(m_Body.m_pSamplerState[i]);
	}
	for (int i = 0; i < m_Body.m_model.texture_count; i++){
		SAFE_RELEASE(m_Body.m_pTexture[i]);
	}
	delete[] m_Body.m_pTexture;
	SAFE_RELEASE(m_Body.m_pVSConstantBuffer);
	SAFE_RELEASE(m_Body.m_pPSConstantBuffer);
	SAFE_RELEASE(m_Body.m_pVertexShader);
	SAFE_RELEASE(m_Body.m_pPixelShader);
	SAFE_RELEASE(m_Body.m_pIndexBuffer);
	SAFE_RELEASE(m_Body.m_pVertexBuffer);
	SAFE_RELEASE(m_Body.m_pVertexLayout);
}
