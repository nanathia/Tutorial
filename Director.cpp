#include "Director.h"
#include "GyuDon.h"
#include "DEFINE.h"
#include "AutoReleasePool.h"
#include "SceneBase.h"

Director* Director::m_instance = 0;


Director::Director()
{
	// �ŏ��ɂ���
	m_autoRelease = new AutoReleasePool;
}


Director::~Director()
{
	// �Ō�ɂ���
	SAFE_DELETE(m_autoRelease);
}

void Director::create(){
	ASSERT(!m_instance && "Director�����ɐ�������Ă��܂����������悤�Ƃ��܂����B");
	m_instance = new Director;

	m_instance->m_framework = new GyuDon;
	m_instance->m_framework->autoRelease();
	m_instance->m_framework->retain();
}

void Director::destroy(){
	ASSERT(m_instance && "Director������܂��񂪍폜���悤�Ƃ��܂����B");

	m_instance->m_framework->release();

	SAFE_DELETE(m_instance);
}

Director* Director::instance(){
	ASSERT(m_instance && "Director�𐶐����Ă��܂��񂪎擾���悤�Ƃ��܂����B");
	return m_instance;
}



GyuDon* Director::framework(){
	return m_framework;
}
// ����Ǘ�
AutoReleasePool* Director::autoReleasePool(){
	return m_autoRelease;
}
// ���݃V�[��
SceneBase* Director::currentScene(){
	return m_currentScene;
}
// ���V�[��
SceneBase* Director::nextScene(){
	return m_nextScene;
}