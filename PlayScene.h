#pragma once
#include "SceneBase.h"


class Oniko;
class Siba;

class PlayScene :
	public SceneBase
{
	Oniko* m_oniko = NULL;
	Siba* m_siba = NULL;

public:
	PlayScene();
	~PlayScene();

	void update();
	void draw();
};

