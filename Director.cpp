#include "Director.h"
#include "GyuDon.h"
#include "DEFINE.h"


Director* Director::m_instance = 0;


Director::Director()
{
	m_framework = new GyuDon;
}


Director::~Director()
{
	SAFE_DELETE(m_framework);
}

void Director::create(){
	ASSERT(!m_instance && "Directorが既に生成されていますが生成しようとしました。");
	m_instance = new Director;
}

void Director::destroy(){
	ASSERT(m_instance && "Directorがありませんが削除しようとしました。");
	SAFE_DELETE(m_instance);
}

Director* Director::instance(){
	ASSERT(m_instance && "Directorを生成していませんが取得しようとしました。");
	return m_instance;
}



GyuDon* Director::framework(){
	return m_framework;
}