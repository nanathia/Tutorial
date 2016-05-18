#pragma once
#include "SceneBase.h"


class PMXModel;
class Siba;
class OBJModel;

class PlayScene :
	public SceneBase
{
	PMXModel* m_oniko = NULL;
	OBJModel* m_skyDome = NULL;
	Siba* m_siba = NULL;

public:
	PlayScene();
	virtual ~PlayScene();

	void update();
	void draw();
};

