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
		ASSERT ((*it)->refCount() == 1 && "オブジェクトが開放しきれていません");
		SAFE_DELETE(*it);
		it++;
	}
	m_objectList.clear();
}


// 自動解放処理に適用します。
void AutoReleasePool::autoRelease(Ref* object)
{
	ASSERT(object->refCount() == 0 && "登録するオブジェクトの参照カウントが不正です。");
	m_objectList.push_back(object);
	object->retain();
}

// 自動開放処理のプロセスを行います。
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