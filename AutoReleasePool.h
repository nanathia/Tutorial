#pragma once

#include <list>

// ��������𓝊�����N���X�ł��B

class Ref;

class AutoReleasePool
{
	std::list<Ref*> m_objectList;

public:
	AutoReleasePool();
	~AutoReleasePool();

	// ������������ɓK�p���܂��B
	void autoRelease(Ref* object);
	// �����J�������̃v���Z�X���s���܂��B
	void update();
};

