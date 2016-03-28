#include "SceneManager.h"
#include "DEFINE.h"
#include "SceneBase.h"
#include "PlayScene.h"

SceneManager::SceneManager()
{
}


SceneManager::~SceneManager()
{
	if (m_currentScene){
		m_currentScene->release();
		m_currentScene = 0;
	}
	if (m_nextScene){
		m_nextScene->release();
		m_currentScene = 0;
	}
}

// 最初のシーン
void SceneManager::initScene(){
	m_currentScene = new PlayScene;
	m_currentScene->autoRelease();
	m_currentScene->retain();
}

// 現在シーン
SceneBase* SceneManager::currentScene(){
	return m_currentScene;
}
// 次シーン
SceneBase* SceneManager::nextScene(){
	return m_nextScene;
}

// 次シーンをセット
void SceneManager::changeSceneRequest(SceneBase* next){
	ASSERT(!m_nextScene && "既に次のシーンが有ります。");
	m_nextScene = next;
}
// シーン更新
void SceneManager::update(){
	ASSERT(m_currentScene && "シーンがありません");
	m_currentScene->update();
	if (m_nextScene){
		m_currentScene->release();
		m_currentScene = m_nextScene;
		m_nextScene = 0;
	}
}
// シーン描画
void SceneManager::draw(){
	m_currentScene->draw();
}