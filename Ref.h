#pragma once

// �K���p������N���X�ł��B����������Ǘ����܂��B

class Ref
{
	int m_refCount = 0;

public:
	Ref();
	virtual ~Ref();
	
	void retain();
	void release();

	int refCount();
};

