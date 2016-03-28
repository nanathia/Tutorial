#pragma once

// このプロジェクトのどこからでもアクセス可能な場所です。

class GyuDon;
class SceneBase;
class AutoReleasePool;
class SceneManager;

class Director
{
	static Director* m_instance;

	Director();
	~Director();

public:

	static void create();
	static void destroy();
	static Director* instance();


// ここから実体です。
private:
	// フレームワーク
	GyuDon* m_framework = 0;
	// 解放管理
	AutoReleasePool* m_autoRelease = 0;
	// シーン管理
	SceneManager* m_scene = 0;

// インスタンス取得
public:
	// フレームワーク
	GyuDon* framework();
	// 解放管理
	AutoReleasePool* autoReleasePool();
	// シーン管理
	SceneManager* scene();

// その他制御
public:

};