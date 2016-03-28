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

// �ŏ��̃V�[��
void SceneManager::initScene(){
	m_currentScene = new PlayScene;
	m_currentScene->autoRelease();
	m_currentScene->retain();
}

// ���݃V�[��
SceneBase* SceneManager::currentScene(){
	return m_currentScene;
}
// ���V�[��
SceneBase* SceneManager::nextScene(){
	return m_nextScene;
}

// ���V�[�����Z�b�g
void SceneManager::changeSceneRequest(SceneBase* next){
	ASSERT(!m_nextScene && "���Ɏ��̃V�[�����L��܂��B");
	m_nextScene = next;
}
// �V�[���X�V
void SceneManager::update(){
	ASSERT(m_currentScene && "�V�[��������܂���");
	m_currentScene->update();
	if (m_nextScene){
		m_currentScene->release();
		m_currentScene = m_nextScene;
		m_nextScene = 0;
	}
}
// �V�[���`��
void SceneManager::draw(){
	m_currentScene->draw();
}