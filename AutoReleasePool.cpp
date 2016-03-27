#include "AutoReleasePool.h"
#include "DEFINE.h"
#include "Ref.h"


AutoReleasePool::AutoReleasePool()
{
}


AutoReleasePool::~AutoReleasePool()
{
	auto it = m_objectList.begin();
	while (it != m_objectList.end())
	{
		ASSERT ((*it)->refCount() == 1 && "�I�u�W�F�N�g���J��������Ă��܂���");
		SAFE_DELETE(*it);
		it++;
	}
	m_objectList.clear();
}


// ������������ɓK�p���܂��B
void AutoReleasePool::autoRelease(Ref* object)
{
	ASSERT(object->refCount() == 0 && "�o�^����I�u�W�F�N�g�̎Q�ƃJ�E���g���s���ł��B");
	m_objectList.push_back(object);
	object->retain();
}

// �����J�������̃v���Z�X���s���܂��B
void AutoReleasePool::update()
{
	auto it = m_objectList.begin();
	while (it != m_objectList.end()){
		if ((*it)->refCount() == 1){
			SAFE_DELETE(*it);
			it = m_objectList.erase(it);
			continue;
		}
		it++;
	}
}