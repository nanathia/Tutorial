#pragma once
#include "Ref.h"

// シーンのインターフェイスです。
class SceneBase : public Ref
{
public:
	SceneBase();
	virtual ~SceneBase();

	virtual void update() = 0;
	virtual void draw() = 0;
};

