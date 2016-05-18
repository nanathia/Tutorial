#include "AutoReleasePool.h"
#include "DEFINE.h"
#include "Ref.h"


AutoReleasePool::AutoReleasePool()
{
}


AutoReleasePool::~AutoReleasePool()
{
	for (int i = 0; i < 1000; i++){
		// 1000�񎎍s����B
		auto it = m_objectList.begin();
		while (it != m_objectList.end())
		{
			if ((*it)->refCount() == 1){
				SAFE_DELETE(*it);
				it = m_objectList.erase(it);
				continue;
			}
			it++;
		}
		if (m_objectList.empty()){
			break;
		}
	}
	ASSERT(m_objectList.empty() && "�I�u�W�F�N�g�����������Ă��܂���");
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