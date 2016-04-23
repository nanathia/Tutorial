#include "Siba.h"

#include "INCLUDES.h"
#include "Director.h"
#include "GyuDon.h"


void Siba::Draw(){

	ID3D11Device* device = Director::instance()->framework()->device();
	ID3D11DeviceContext* deviceContext = Director::instance()->framework()->deviceContext();

	deviceContext->VSSetShader(m_kusa.m_pVertexShader, NULL, 0);
	deviceContext->PSSetShader(m_kusa.m_pPixelShader, NULL, 0);

	deviceContext->IASetInputLayout(m_kusa.m_pVertexLayout);
	deviceContext->PSSetSamplers(0, 1, &m_kusa.m_pSamplerState);
	deviceContext->PSSetShaderResources(0, 1, &m_kusa.m_pTexture);

	//ブレンディングをコンテキストに設定
	float blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };
	deviceContext->OMSetBlendState(m_kusa.m_pBlendState, blendFactor, 0xffffffff);

	D3DXMATRIX projMat, viewMat;
	projMat = Director::instance()->framework()->projMat();
	viewMat = Director::instance()->framework()->viewMat();

	{
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		deviceContext->IASetVertexBuffers(0, 1, &m_kusa.m_pVertexBuffer, &stride, &offset);
		deviceContext->IASetIndexBuffer(m_kusa.m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		deviceContext->VSSetConstantBuffers(0, 1, &m_kusa.m_pConstantBuffer);
		deviceContext->PSSetConstantBuffers(0, 1, &m_kusa.m_pConstantBuffer);

		auto renderTarget = Director::instance()->framework()->renderTargetView();
		deviceContext->OMSetRenderTargets(1, &renderTarget, 0);

		D3DXMATRIX wvp;
		D3DXMATRIX world;
		D3DXMATRIX trance, rota, scale;
		D3DXMatrixTranslation(&trance, 0, 0, 0);
		D3DXMatrixScaling(&scale, 1.f, 1.f, 1.f);
		D3DXMatrixRotationAxis(&rota, &D3DXVECTOR3(1.f, 1.f, 1.f), 0.f);
		wvp = world * projMat;
		D3DXMatrixTranspose(&world, &world);
		D3D11_MAPPED_SUBRESOURCE pData;
		CONSTANT cb;
		if (SUCCEEDED(deviceContext->Map(m_kusa.m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			cb.mWVP = world;

			memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(CONSTANT));
			deviceContext->Unmap(m_kusa.m_pConstantBuffer, 0);
		}

	}

}