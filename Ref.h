#pragma once

// �K���p������N���X�ł��B����������Ǘ����܂��B

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

