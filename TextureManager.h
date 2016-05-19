#pragma once
#include "Ref.h"
#include <map>

// ����e�N�X�`���𕡐��ӏ�����g�p���邽�߂̃N���X�ł��B

class MyTexture;

class TextureManager :
	public Ref
{
	std::map<std::string, MyTexture*> m_textureMap;

public:
	TextureManager();
	~TextureManager();

	// �}�b�v�ɒǉ�
	void add(std::string name, MyTexture* tex);
	// �}�b�v����폜
	void sub(std::string name);
	// �}�b�v��S���
	void releaseAll();
	// �v�f���擾
	MyTexture* get(std::string& name);

};

