#include "TextureManager.h"
#include "INCLUDES.h"
#include "MyTexture.h"


TextureManager::TextureManager()
{
}


TextureManager::~TextureManager()
{
}


// マップに追加
void TextureManager::add(std::string name, MyTexture* tex){
	ASSERT(m_textureMap.find(name) == m_textureMap.end() && "既に存在するテクスチャ名に上書きしようとしています。");
	m_textureMap[name] = tex;
	tex->retain();
}

// マップから削除
void TextureManager::sub(std::string name){
	auto it = m_textureMap.find(name);
	ASSERT(it != m_textureMap.end() && "解放指定されたテクスチャ名はマップに存在しませんでした。");
	it->second->release();
	it->second = NULL;
	m_textureMap.erase(it);
}

// マップを全解放
void TextureManager::releaseAll(){
	for (auto it = m_textureMap.begin(); it != m_textureMap.end(); it++){
		it->second->release();
		it->second = NULL;
	}
	m_textureMap.clear();

}

// 要素を取得
MyTexture* TextureManager::get(std::string& name){
	auto it = m_textureMap.find(name);
	if (it != m_textureMap.end()){
		return it->second;
	}
	else{
		return 0;
	}
}
