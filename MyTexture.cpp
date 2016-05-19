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

// �����f�[�^�����������܂�
void MyTexture::ReleaseSelf(){
	SAFE_RELEASE(m_pImpl);
	SAFE_RELEASE(m_pRenderTargetView);
}

// ���ڃf�[�^����e�N�X�`������肽���ꍇ
void MyTexture::InitWithTexture2D(ID3D11Texture2D* m_pTexture2D){
	ReleaseSelf();
	// ������
	HALT(�������ł��A�K�v�ł���Ύ������܂��B);
}

// ���\�[�X�t�@�C������e�N�X�`�����쐬����ꍇ
void MyTexture::InitWithResourceData(const char* fileName){

	ReleaseSelf();

	ID3D11Device* device = Director::instance()->framework()->device();
	ID3D11DeviceContext* deviceContext = Director::instance()->framework()->deviceContext();

	std::string _fileName = fileName;
	if (_fileName.rfind(".tga") != std::string::npos){
		// DirectX��TGA�t�@�C���̓ǂݍ��݂ɑΉ����Ă��Ȃ��ׁA�蓮�œǂݍ��݂����܂��B
		TGA tga(fileName);
		//int i = 0;
		// CPU�ŏ������݂��ł���e�N�X�`�����쐬
		D3D11_TEXTURE2D_DESC desc;
		memset(&desc, 0, sizeof(desc));
		desc.Width = tga.GetWidth();
		desc.Height = tga.GetHeight();
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		// RGBA(255,255,255,255)�^�C�v
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		// ���I�i�������݂��邽�߂̕K�{�����j
		desc.Usage = D3D11_USAGE_DYNAMIC;
		// �V�F�[�_���\�[�X�Ƃ��Ďg��	
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		// CPU����A�N�Z�X���ď�������OK
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		ID3D11Texture2D* tex2D;   /// 2�c�e�N�X�`��
		HRESULT hr = device->CreateTexture2D(&desc, 0, &tex2D);

		// �o�C�i�������e�N�X�`���ɏ������ޕ���
		D3D11_MAPPED_SUBRESOURCE hMappedResource;
		hr = deviceContext->Map(
			tex2D,
			0,
			D3D11_MAP_WRITE_DISCARD,
			0,
			&hMappedResource);
		if (FAILED(hr)){
			HALT("�e�N�X�`�����}�b�v�o���܂���ł����B");
		}

		// �����ŏ�������
		BYTE* pBits = (BYTE*)hMappedResource.pData;
		//memset(pBits, 0, hMappedResource.RowPitch * TM.tmHeight);
		memcpy(
			(BYTE*)pBits,
			tga.GetImage(),
			tga.GetHeight()*tga.GetWidth()*sizeof(unsigned));
		deviceContext->Unmap(tex2D, 0);


		// ShaderResourceView�̏����쐬����
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = desc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = desc.MipLevels;

		// ShaderResourceView���쐬����
		hr = device->CreateShaderResourceView(tex2D, &srvDesc, &m_pImpl);
		ASSERT(SUCCEEDED(hr) && "TGA�e�N�X�`���̍쐬���o���܂���ł����B");

	}
	else{
		HRESULT hr;
		if (FAILED(hr = D3DX11CreateShaderResourceViewFromFile(device, fileName, NULL, NULL, &m_pImpl, NULL)))
		{
			HALT(���\�[�X����e�N�X�`�����쐬�ł��܂���ł����B);
		}
	}

	m_usageFlags = enUSAGE_CPU_ACCESS | enUSAGE_RESOURCE_TEXTURE;

	// �T�C�Y���𒊏o
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

// ��̃e�N�X�`���쐬�������ꍇ
void MyTexture::InitWithEmptyData(int width, int height){

	// �p��ł��邩�ǂ������e�X�g���܂�
	{
		// ���܂��ϐ������v�����܂���ł���(^^;)
		bool tempBool = false;
		int tempInt = width;
		while(true){
			// �P�܂Ŋ���؂邩�A�]�肪��������܂łQ�Ŋ��邱�Ƃřp��𔻕ʂ��܂�
			if (tempInt == 1){
				tempBool = true;
				break;
			}
			int nextInt = tempInt / 2;
			STRONG_ASSERT((tempInt % nextInt) == 0 && "�����_�����O�^�[�Q�b�g�����Ɏw�肳�ꂽ�T�C�Y���p��ł͂���܂���ł���");
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
	// RGBA(255,255,255,255)�^�C�v
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	ID3D11Texture2D* tex2D;   /// 2�c�e�N�X�`��
	ID3D11Device* device = Director::instance()->framework()->device();
	ID3D11DeviceContext* deviceContext = Director::instance()->framework()->deviceContext();
	HRESULT hr = device->CreateTexture2D(&desc, 0, &tex2D);
	ASSERT(SUCCEEDED(hr));

	// ShaderResourceView�̏����쐬����
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;

	// ShaderResourceView���쐬����
	hr = device->CreateShaderResourceView(tex2D, &srvDesc, &m_pImpl);
	ASSERT(SUCCEEDED(hr && "��̃e�N�X�`�����쐬�ł��܂���ł���"));

	// RenderTargetView Create.
	hr = device->CreateRenderTargetView(tex2D, NULL, &m_pRenderTargetView);
	ASSERT(SUCCEEDED(hr) && "��̃����_�����O�^�[�Q�b�g���쐬�ł��܂���ł���");

	// ���F�ŃN���A���Ă���
	float clearColor[4] = { 1.0, 0.0, 0.0, 0.0 };
	deviceContext->ClearRenderTargetView(m_pRenderTargetView, clearColor);

	// �T�C�Y���𒊏o
	m_width = m_originalWidth = width;
	m_height = m_originalHeight = height;

	m_usageFlags = enUSAGE_RESOURCE_TEXTURE | enUSAGE_RENDER_TARGET;
}


// ���̂��擾
ID3D11ShaderResourceView* MyTexture::GetImpl(){
	ASSERT(m_pImpl && "�e�N�X�`���̎��̂�����܂���");
	return m_pImpl;
}

// �����_�����O�^�[�Q�b�g���擾
ID3D11RenderTargetView* MyTexture::GetRenderTargetView(){
	ASSERT(m_pRenderTargetView && "�����_�����O�^�[�Q�b�g�ł͂���܂��񂪎擾���悤�Ƃ��܂���");
	return m_pRenderTargetView;
}

// �e�N�X�`���̃T�C�Y�i�p��j�ł��B
int MyTexture::GetFixedWidth() const{
	return m_width;
}
int MyTexture::GetFixedHeight() const{
	return m_height;
}

// �e�N�X�`���̃T�C�Y�i�g�p�͈́j�ł��B
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