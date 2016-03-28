#pragma once

// �V�[�����Ǘ����܂�

#include "Ref.h"

class SceneBase;

class SceneManager : public Ref
{
	SceneBase* m_currentScene = 0;
	SceneBase* m_nextScene = 0;

public:
	SceneManager();
	~SceneManager();

	// �ŏ��̃V�[��
	void initScene();
	// ���݃V�[��
	SceneBase* currentScene();
	// ���V�[��
	SceneBase* nextScene();
	// ���V�[�����Z�b�g
	void changeSceneRequest(SceneBase* next);
	// �V�[���X�V
	void update();
	void draw();
};

