#pragma once

template <typename T, typename R>
void OBJECT_CREATE(T*& out, R* object){
	out = object;
	out->autoRelease();
	out->retain();
}

template <typename T>
void OBJECT_RELEASE(T*& object){
	object->release();
	object = 0;
}