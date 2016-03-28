#pragma once

// ���̃v���W�F�N�g�̂ǂ�����ł��A�N�Z�X�\�ȏꏊ�ł��B

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


// ����������̂ł��B
private:
	// �t���[�����[�N
	GyuDon* m_framework = 0;
	// ����Ǘ�
	AutoReleasePool* m_autoRelease = 0;
	// �V�[���Ǘ�
	SceneManager* m_scene = 0;

// �C���X�^���X�擾
public:
	// �t���[�����[�N
	GyuDon* framework();
	// ����Ǘ�
	AutoReleasePool* autoReleasePool();
	// �V�[���Ǘ�
	SceneManager* scene();

// ���̑�����
public:

};