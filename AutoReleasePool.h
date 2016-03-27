#pragma once

#include <list>

// 自動解放を統括するクラスです。

class Ref;

class AutoReleasePool
{
	std::list<Ref*> m_objectList;

public:
	AutoReleasePool();
	~AutoReleasePool();

	// 自動解放処理に適用します。
	void autoRelease(Ref* object);
	// 自動開放処理のプロセスを行います。
	void update();
};

