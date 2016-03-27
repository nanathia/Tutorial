#pragma once

// ���̃v���W�F�N�g�̂ǂ�����ł��A�N�Z�X�\�ȏꏊ�ł��B

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


// ����������̂ł��B
private:
	// �t���[�����[�N
	GyuDon* m_framework = 0;
	// ����Ǘ�
	AutoReleasePool* m_autoRelease = 0;
	// ���݃V�[��
	SceneBase* m_currentScene = 0;
	// ���V�[��
	SceneBase* m_nextScene = 0;

// �C���X�^���X�擾
public:
	// �t���[�����[�N
	GyuDon* framework();
	// ����Ǘ�
	AutoReleasePool* autoReleasePool();
	// ���݃V�[��
	SceneBase* currentScene();
	// ���V�[��
	SceneBase* nextScene();

// ���̑�����
public:
	// ���V�[�����Z�b�g
	void changeSceneRequest(SceneBase* next);
};