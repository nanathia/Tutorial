#include "Ref.h"


Ref::Ref()
{
	int i = 0;
}


Ref::~Ref()
{
	int i = 0;
}


void Ref::retain(){
	m_refCount++;
}

void Ref::release(){
	m_refCount--;
}

int Ref::refCount(){
	return m_refCount;
}

void Ref::autoRelease(){
	Director::instance()->autoReleasePool()->autoRelease(this);
}