#pragma once
#include "Ref.h"
#include <D3D11.h>
#include <D3DX10.h>
#include <D3DX11.h>

// テクスチャを管理するクラスです。

class MyTexture :
	public Ref
{
public:
	enum USAGE{ // 使用用途
		enUSAGE_RESOURCE_TEXTURE = 1, // 普通に使用可能。（リソース用途として使用可能）
		enUSAGE_RENDER_TARGET = 1 << 1, // 描画先の板として使用可能。
		enUSAGE_CPU_ACCESS = 1 << 2, // CPUからアクセス可能
	};

private:
	ID3D11ShaderResourceView* m_pImpl = NULL; // シェーダリソース用
	ID3D11RenderTargetView* m_pRenderTargetView = NULL; // レンダリングターゲット用

	// 使用用途ビットフラグです。
	unsigned m_usageFlags = 0;

	// テクスチャのサイズ（冪乗）です。
	int m_width = -1;
	int m_height = -1;
	// テクスチャのサイズ（使用範囲）です。
	int m_originalWidth = -1;
	int m_originalHeight = -1;

	// 内部データを初期化します
	void ReleaseSelf();

	// 冪乗に強制した数値を返します
	float convertDefault(float in) const;

public:

	MyTexture();
	virtual ~MyTexture();

	// 直接データからテクスチャを作りたい場合
	void InitWithTexture2D(ID3D11Texture2D* m_pTexture2D);
	// リソースファイルからテクスチャを作成する場合
	void InitWithResourceData(const char* fileName);
	// 空のレンダリング用テクスチャ作成したい場合
	void InitWithEmptyData(int width, int height);

	// 実体を取得
	ID3D11ShaderResourceView* GetImpl();
	// レンダリングターゲットを取得
	ID3D11RenderTargetView* GetRenderTargetView();

	// テクスチャのサイズ（冪乗）です。
	int GetFixedWidth() const;
	int GetFixedHeight() const;
	// テクスチャのサイズ（使用範囲）です。
	int GetOriginalWidth() const;
	int GetOriginalHeight() const;

};

