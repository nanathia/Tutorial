#pragma once

// 必ず継承するクラスです。自動解放を管理します。

#include "AutoReleasePool.h"
#include "Director.h"

class Ref
{
	int m_refCount = 0;

public:
	Ref();
	virtual ~Ref();
	
	void retain();
	void release();
	void autoRelease();

	int refCount();
};

