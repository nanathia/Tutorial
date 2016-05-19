#pragma once
#include "Ref.h"
#include <D3D11.h>
#include <D3DX10.h>
#include <D3DX11.h>

// �e�N�X�`�����Ǘ�����N���X�ł��B

class MyTexture :
	public Ref
{
public:
	enum USAGE{ // �g�p�p�r
		enUSAGE_RESOURCE_TEXTURE = 1, // ���ʂɎg�p�\�B�i���\�[�X�p�r�Ƃ��Ďg�p�\�j
		enUSAGE_RENDER_TARGET = 1 << 1, // �`���̔Ƃ��Ďg�p�\�B
		enUSAGE_CPU_ACCESS = 1 << 2, // CPU����A�N�Z�X�\
	};

private:
	ID3D11ShaderResourceView* m_pImpl = NULL; // �V�F�[�_���\�[�X�p
	ID3D11RenderTargetView* m_pRenderTargetView = NULL; // �����_�����O�^�[�Q�b�g�p

	// �g�p�p�r�r�b�g�t���O�ł��B
	unsigned m_usageFlags = 0;

	// �e�N�X�`���̃T�C�Y�i�p��j�ł��B
	int m_width = -1;
	int m_height = -1;
	// �e�N�X�`���̃T�C�Y�i�g�p�͈́j�ł��B
	int m_originalWidth = -1;
	int m_originalHeight = -1;

	// �����f�[�^�����������܂�
	void ReleaseSelf();

	// �p��ɋ����������l��Ԃ��܂�
	float convertDefault(float in) const;

public:

	MyTexture();
	virtual ~MyTexture();

	// ���ڃf�[�^����e�N�X�`������肽���ꍇ
	void InitWithTexture2D(ID3D11Texture2D* m_pTexture2D);
	// ���\�[�X�t�@�C������e�N�X�`�����쐬����ꍇ
	void InitWithResourceData(const char* fileName);
	// ��̃����_�����O�p�e�N�X�`���쐬�������ꍇ
	void InitWithEmptyData(int width, int height);

	// ���̂��擾
	ID3D11ShaderResourceView* GetImpl();
	// �����_�����O�^�[�Q�b�g���擾
	ID3D11RenderTargetView* GetRenderTargetView();

	// �e�N�X�`���̃T�C�Y�i�p��j�ł��B
	int GetFixedWidth() const;
	int GetFixedHeight() const;
	// �e�N�X�`���̃T�C�Y�i�g�p�͈́j�ł��B
	int GetOriginalWidth() const;
	int GetOriginalHeight() const;

};

