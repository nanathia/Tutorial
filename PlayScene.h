#pragma once
#include "SceneBase.h"


class Oniko;

class PlayScene :
	public SceneBase
{
	Oniko* m_oniko = NULL;

public:
	PlayScene();
	~PlayScene();

	void update();
	void draw();
};

