#pragma once
#include "Ref.h"
#include <map>

// 同一テクスチャを複数箇所から使用するためのクラスです。

class MyTexture;

class TextureManager :
	public Ref
{
	std::map<std::string, MyTexture*> m_textureMap;

public:
	TextureManager();
	~TextureManager();

	// マップに追加
	void add(std::string name, MyTexture* tex);
	// マップから削除
	void sub(std::string name);
	// マップを全解放
	void releaseAll();
	// 要素を取得
	MyTexture* get(std::string& name);

};

