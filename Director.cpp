#include "Director.h"
#include "GyuDon.h"
#include "DEFINE.h"
#include "AutoReleasePool.h"
#include "SceneBase.h"

Director* Director::m_instance = 0;


Director::Director()
{
	// 最初にこれ
	m_autoRelease = new AutoReleasePool;
}


Director::~Director()
{
	// 最後にこれ
	SAFE_DELETE(m_autoRelease);
}

void Director::create(){
	ASSERT(!m_instance && "Directorが既に生成されていますが生成しようとしました。");
	m_instance = new Director;

	m_instance->m_framework = new GyuDon;
	m_instance->m_framework->autoRelease();
	m_instance->m_framework->retain();
}

void Director::destroy(){
	ASSERT(m_instance && "Directorがありませんが削除しようとしました。");

	m_instance->m_framework->release();

	SAFE_DELETE(m_instance);
}

Director* Director::instance(){
	ASSERT(m_instance && "Directorを生成していませんが取得しようとしました。");
	return m_instance;
}



GyuDon* Director::framework(){
	return m_framework;
}
// 解放管理
AutoReleasePool* Director::autoReleasePool(){
	return m_autoRelease;
}
// 現在シーン
SceneBase* Director::currentScene(){
	return m_currentScene;
}
// 次シーン
SceneBase* Director::nextScene(){
	return m_nextScene;
}