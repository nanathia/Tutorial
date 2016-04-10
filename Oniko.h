#pragma once
#include "Ref.h"
#include "PMXModel.h"
#include "NomalModel.h"

class ModelDaiturenShoturen;

class Oniko : public Ref
{
	//resources
	PMXModel* m_human = NULL;
	ModelDaiturenShoturen* m_katana = NULL;

public:
	Oniko();
	~Oniko();

	void draw();
};

