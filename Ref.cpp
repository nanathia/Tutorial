#include "Ref.h"


Ref::Ref()
{
}


Ref::~Ref()
{
}


void Ref::retain(){
}

void Ref::release(){
	m_refCount--;
}

int Ref::refCount(){
	return m_refCount;
}