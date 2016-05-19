#include "MyTexture.h"
#include "Director.h"
#include "GyuDon.h"
#include "TGATexture.h"
#include "INCLUDES.h"

MyTexture::MyTexture()
{
}


MyTexture::~MyTexture()
{
	ReleaseSelf();
}

// 内部データを初期化します
void MyTexture::ReleaseSelf(){
	SAFE_RELEASE(m_pImpl);
	SAFE_RELEASE(m_pRenderTargetView);
}

// 直接データからテクスチャを作りたい場合
void MyTexture::InitWithTexture2D(ID3D11Texture2D* m_pTexture2D){
	ReleaseSelf();
	// 未実装
	HALT(未実装です、必要であれば実装します。);
}

// リソースファイルからテクスチャを作成する場合
void MyTexture::InitWithResourceData(const char* fileName){

	ReleaseSelf();

	ID3D11Device* device = Director::instance()->framework()->device();
	ID3D11DeviceContext* deviceContext = Director::instance()->framework()->deviceContext();

	std::string _fileName = fileName;
	if (_fileName.rfind(".tga") != std::string::npos){
		// DirectXがTGAファイルの読み込みに対応していない為、手動で読み込みをします。
		TGA tga(fileName);
		//int i = 0;
		// CPUで書き込みができるテクスチャを作成
		D3D11_TEXTURE2D_DESC desc;
		memset(&desc, 0, sizeof(desc));
		desc.Width = tga.GetWidth();
		desc.Height = tga.GetHeight();
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		// RGBA(255,255,255,255)タイプ
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		// 動的（書き込みするための必須条件）
		desc.Usage = D3D11_USAGE_DYNAMIC;
		// シェーダリソースとして使う	
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		// CPUからアクセスして書き込みOK
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		ID3D11Texture2D* tex2D;   /// 2Ｄテクスチャ
		HRESULT hr = device->CreateTexture2D(&desc, 0, &tex2D);

		// バイナリ情報をテクスチャに書き込む部分
		D3D11_MAPPED_SUBRESOURCE hMappedResource;
		hr = deviceContext->Map(
			tex2D,
			0,
			D3D11_MAP_WRITE_DISCARD,
			0,
			&hMappedResource);
		if (FAILED(hr)){
			HALT("テクスチャをマップ出来ませんでした。");
		}

		// ここで書き込む
		BYTE* pBits = (BYTE*)hMappedResource.pData;
		//memset(pBits, 0, hMappedResource.RowPitch * TM.tmHeight);
		memcpy(
			(BYTE*)pBits,
			tga.GetImage(),
			tga.GetHeight()*tga.GetWidth()*sizeof(unsigned));
		deviceContext->Unmap(tex2D, 0);


		// ShaderResourceViewの情報を作成する
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = desc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = desc.MipLevels;

		// ShaderResourceViewを作成する
		hr = device->CreateShaderResourceView(tex2D, &srvDesc, &m_pImpl);
		ASSERT(SUCCEEDED(hr) && "TGAテクスチャの作成が出来ませんでした。");

	}
	else{
		HRESULT hr;
		if (FAILED(hr = D3DX11CreateShaderResourceViewFromFile(device, fileName, NULL, NULL, &m_pImpl, NULL)))
		{
			HALT(リソースからテクスチャを作成できませんでした。);
		}
	}

	m_usageFlags = enUSAGE_CPU_ACCESS | enUSAGE_RESOURCE_TEXTURE;

	// サイズ情報を抽出
	D3D11_SHADER_RESOURCE_VIEW_DESC srdc;
	m_pImpl->GetDesc(&srdc);
	D3D11_TEXTURE2D_DESC txdc;
	ID3D11Texture2D* tex2D = NULL;
	m_pImpl->GetResource(reinterpret_cast<ID3D11Resource**>(&tex2D));
	tex2D->GetDesc(&txdc);
	m_originalWidth = txdc.Width;
	m_originalHeight = txdc.Height;
	m_width = convertDefault(txdc.Width);
	m_height = convertDefault(txdc.Height);
}

// 空のテクスチャ作成したい場合
void MyTexture::InitWithEmptyData(int width, int height){

	// 冪乗であるかどうかをテストします
	{
		// うまい変数名が思いつきませんでした(^^;)
		bool tempBool = false;
		int tempInt = width;
		while(true){
			// １まで割り切るか、余りが発生するまで２で割ることで冪乗を判別します
			if (tempInt == 1){
				tempBool = true;
				break;
			}
			int nextInt = tempInt / 2;
			STRONG_ASSERT((tempInt % nextInt) == 0 && "レンダリングターゲット生成に指定されたサイズが冪乗ではありませんでした");
			tempInt = nextInt;
		}
	}

	ReleaseSelf();
	D3D11_TEXTURE2D_DESC desc;
	memset(&desc, 0, sizeof(desc));
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	// RGBA(255,255,255,255)タイプ
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	ID3D11Texture2D* tex2D;   /// 2Ｄテクスチャ
	ID3D11Device* device = Director::instance()->framework()->device();
	ID3D11DeviceContext* deviceContext = Director::instance()->framework()->deviceContext();
	HRESULT hr = device->CreateTexture2D(&desc, 0, &tex2D);
	ASSERT(SUCCEEDED(hr));

	// ShaderResourceViewの情報を作成する
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;

	// ShaderResourceViewを作成する
	hr = device->CreateShaderResourceView(tex2D, &srvDesc, &m_pImpl);
	ASSERT(SUCCEEDED(hr && "空のテクスチャが作成できませんでした"));

	// RenderTargetView Create.
	hr = device->CreateRenderTargetView(tex2D, NULL, &m_pRenderTargetView);
	ASSERT(SUCCEEDED(hr) && "空のレンダリングターゲットが作成できませんでした");

	// 黒色でクリアしておく
	float clearColor[4] = { 1.0, 0.0, 0.0, 0.0 };
	deviceContext->ClearRenderTargetView(m_pRenderTargetView, clearColor);

	// サイズ情報を抽出
	m_width = m_originalWidth = width;
	m_height = m_originalHeight = height;

	m_usageFlags = enUSAGE_RESOURCE_TEXTURE | enUSAGE_RENDER_TARGET;
}


// 実体を取得
ID3D11ShaderResourceView* MyTexture::GetImpl(){
	ASSERT(m_pImpl && "テクスチャの実体がありません");
	return m_pImpl;
}

// レンダリングターゲットを取得
ID3D11RenderTargetView* MyTexture::GetRenderTargetView(){
	ASSERT(m_pRenderTargetView && "レンダリングターゲットではありませんが取得しようとしました");
	return m_pRenderTargetView;
}

// テクスチャのサイズ（冪乗）です。
int MyTexture::GetFixedWidth() const{
	return m_width;
}
int MyTexture::GetFixedHeight() const{
	return m_height;
}

// テクスチャのサイズ（使用範囲）です。
int MyTexture::GetOriginalWidth() const{
	return m_originalWidth;
}
int MyTexture::GetOriginalHeight() const{
	return m_originalHeight;
}


float MyTexture::convertDefault(float in) const{
	int ret = 1;
	int _in = float(in);
	while (ret < in){
		ret *= 2;
	}
	return (float)ret;
}