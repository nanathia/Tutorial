#pragma once

// 必ず継承するクラスです。自動解放を管理します。

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

