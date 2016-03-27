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
	ASSERT(!m_instance && "Director‚ªŠù‚É¶¬‚³‚ê‚Ä‚¢‚Ü‚·‚ª¶¬‚µ‚æ‚¤‚Æ‚µ‚Ü‚µ‚½B");
	m_instance = new Director;
}

void Director::destroy(){
	ASSERT(m_instance && "Director‚ª‚ ‚è‚Ü‚¹‚ñ‚ªíœ‚µ‚æ‚¤‚Æ‚µ‚Ü‚µ‚½B");
	SAFE_DELETE(m_instance);
}

Director* Director::instance(){
	ASSERT(m_instance && "Director‚ğ¶¬‚µ‚Ä‚¢‚Ü‚¹‚ñ‚ªæ“¾‚µ‚æ‚¤‚Æ‚µ‚Ü‚µ‚½B");
	return m_instance;
}



GyuDon* Director::framework(){
	return m_framework;
}