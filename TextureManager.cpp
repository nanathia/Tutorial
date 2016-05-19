#include "TextureManager.h"
#include "INCLUDES.h"
#include "MyTexture.h"


TextureManager::TextureManager()
{
}


TextureManager::~TextureManager()
{
}


// �}�b�v�ɒǉ�
void TextureManager::add(std::string name, MyTexture* tex){
	ASSERT(m_textureMap.find(name) == m_textureMap.end() && "���ɑ��݂���e�N�X�`�����ɏ㏑�����悤�Ƃ��Ă��܂��B");
	m_textureMap[name] = tex;
	tex->retain();
}

// �}�b�v����폜
void TextureManager::sub(std::string name){
	auto it = m_textureMap.find(name);
	ASSERT(it != m_textureMap.end() && "����w�肳�ꂽ�e�N�X�`�����̓}�b�v�ɑ��݂��܂���ł����B");
	it->second->release();
	it->second = NULL;
	m_textureMap.erase(it);
}

// �}�b�v��S���
void TextureManager::releaseAll(){
	for (auto it = m_textureMap.begin(); it != m_textureMap.end(); it++){
		it->second->release();
		it->second = NULL;
	}
	m_textureMap.clear();

}

// �v�f���擾
MyTexture* TextureManager::get(std::string& name){
	auto it = m_textureMap.find(name);
	if (it != m_textureMap.end()){
		return it->second;
	}
	else{
		return 0;
	}
}
