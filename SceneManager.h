#pragma once

// シーンを管理します

#include "Ref.h"

class SceneBase;

class SceneManager : public Ref
{
	SceneBase* m_currentScene = 0;
	SceneBase* m_nextScene = 0;

public:
	SceneManager();
	~SceneManager();

	// 最初のシーン
	void initScene();
	// 現在シーン
	SceneBase* currentScene();
	// 次シーン
	SceneBase* nextScene();
	// 次シーンをセット
	void changeSceneRequest(SceneBase* next);
	// シーン更新
	void update();
	void draw();
};

