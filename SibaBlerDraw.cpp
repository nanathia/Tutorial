#include "Siba.h"
#include "Director.h"
#include "GyuDon.h"


void Siba::BlerDraw(){
	ID3D11Device* device = Director::instance()->framework()->device();
	ID3D11DeviceContext* deviceContext = Director::instance()->framework()->deviceContext();
	ID3D11RenderTargetView* renderTarget2 = Director::instance()->framework()->renderTargetView();

	Bler& t = m_bler;

	deviceContext->VSSetShader(t.m_pVertexShader, NULL, 0);
	deviceContext->PSSetShader(t.m_pPixelShader, NULL, 0);

	deviceContext->IASetInputLayout(t.m_pVertexLayout);
	deviceContext->PSSetSamplers(0, 1, &t.m_pSamplerState);
	//deviceContext->PSSetShaderResources(0, 1, &t.m_pTexture);

	//ブレンディングをコンテキストに設定
	float blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };
	deviceContext->OMSetBlendState(t.m_pBlendState, blendFactor, 0xffffffff);

	D3DXMATRIX projMat, viewMat;
	projMat = Director::instance()->framework()->projMat();
	viewMat = Director::instance()->framework()->viewMat();

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	UINT stride = sizeof(BlerVertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &t.m_pVertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(t.m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->VSSetConstantBuffers(0, 1, &t.m_pConstantBuffer);
	deviceContext->PSSetConstantBuffers(0, 1, &t.m_pConstantBuffer);

	auto renderTarget = Director::instance()->framework()->renderTargetView();
	deviceContext->OMSetRenderTargets(1, &renderTarget, 0);

	deviceContext->PSSetShaderResources(0, 1, &t.m_pSurface[0]);

	D3D11_MAPPED_SUBRESOURCE pData;
	BlerConstant cb;
	if (SUCCEEDED(deviceContext->Map(t.m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		const float u1 = 1.0f / WINDOW_WIDTH;
		for (int i = 0; i < sizeof(cb.offsets)/sizeof(float); i++){
			cb.offsets[i] = u1 * i + u1;
		}

		memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(BlerConstant));
		deviceContext->Unmap(t.m_pConstantBuffer, 0);
	}
	deviceContext->DrawIndexed(6, 0, 0);

	//deviceContext->PSSetShaderResources(0, 1, &t.m_pSurface[1]);
	//deviceContext->OMSetRenderTargets(1, &renderTarget2, 0);

	//deviceContext->PSSetShader(t.m_pSimplePixelShader, 0, 0);
	//deviceContext->PSSetShaderResources(0, 1, &t.m_pSurface[1]);

	//deviceContext->DrawIndexed(6, 0, 0);

}