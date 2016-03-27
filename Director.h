#pragma once

// このプロジェクトのどこからでもアクセス可能な場所です。

class GyuDon;
class SceneBase;
class AutoReleasePool;

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
	// 現在シーン
	SceneBase* m_currentScene = 0;
	// 次シーン
	SceneBase* m_nextScene = 0;

// インスタンス取得
public:
	// フレームワーク
	GyuDon* framework();
	// 解放管理
	AutoReleasePool* autoReleasePool();
	// 現在シーン
	SceneBase* currentScene();
	// 次シーン
	SceneBase* nextScene();

// その他制御
public:
	// 次シーンをセット
	void changeSceneRequest(SceneBase* next);
};