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
	ASSERT(!m_instance && "Director�����ɐ�������Ă��܂����������悤�Ƃ��܂����B");
	m_instance = new Director;
}

void Director::destroy(){
	ASSERT(m_instance && "Director������܂��񂪍폜���悤�Ƃ��܂����B");
	SAFE_DELETE(m_instance);
}

Director* Director::instance(){
	ASSERT(m_instance && "Director�𐶐����Ă��܂��񂪎擾���悤�Ƃ��܂����B");
	return m_instance;
}



GyuDon* Director::framework(){
	return m_framework;
}