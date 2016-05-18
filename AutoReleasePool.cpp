#include "AutoReleasePool.h"
#include "DEFINE.h"
#include "Ref.h"


AutoReleasePool::AutoReleasePool()
{
}


AutoReleasePool::~AutoReleasePool()
{
	for (int i = 0; i < 1000; i++){
		// 1000回試行する。
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
	ASSERT(m_objectList.empty() && "オブジェクトが解放しきれていません");
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