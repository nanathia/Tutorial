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
	deviceContext->OMSetBlendState(0, blendFactor, 0xffffffff);

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
		ID3D11RenderTargetView* pRenderTargets[2] = { renderTarget, m_kusa.m_pBlerRenderTarget[0] };
		deviceContext->OMSetRenderTargets(2, pRenderTargets, 0);

		D3DXMATRIX wvp;
		D3DXMATRIX world;
		D3DXMATRIX trance, rota, scale;
		D3DXMatrixTranslation(&trance, 0, 0, 0);
		D3DXMatrixScaling(&scale, 10.f, 10.f, 10.f);
		D3DXMatrixRotationAxis(&rota, &D3DXVECTOR3(1.f, 0.f, 0.f), float(timeGetTime())/1000.f);
		world = scale * rota * trance;
		wvp = world * viewMat * projMat;
		D3DXMatrixTranspose(&world, &world);
		D3DXMatrixTranspose(&wvp, &wvp);
		D3D11_MAPPED_SUBRESOURCE pData;
		CONSTANT cb;
		if (SUCCEEDED(deviceContext->Map(m_kusa.m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			cb.mWVP = wvp;
			cb.mW = world;
			cb.vLight = Director::instance()->framework()->getDirectionLight();
			cb.vEye = Director::instance()->framework()->vEyePt();

			memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(CONSTANT));
			deviceContext->Unmap(m_kusa.m_pConstantBuffer, 0);
		}

		deviceContext->DrawIndexed(6, 0, 0);
	}

}