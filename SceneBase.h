#pragma once
#include "Ref.h"

// �V�[���̃C���^�[�t�F�C�X�ł��B
class SceneBase : public Ref
{
public:
	SceneBase();
	virtual ~SceneBase();

	virtual void update() = 0;
	virtual void draw() = 0;
};

