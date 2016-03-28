#pragma once
#include "SceneBase.h"

class PMXModel;

class PlayScene :
	public SceneBase
{
	PMXModel* m_pmx;

public:
	PlayScene();
	~PlayScene();

	void update();
	void draw();
};

