#include "Oniko.h"
#include "ModelDaiturenShoturen.h"

Oniko::Oniko()
{
	OBJECT_CREATE(m_human, new PMXModel("dat/pmx/oniko", "“ú–{‹Sq0.15.pmx"));
	OBJECT_CREATE(m_katana, new ModelDaiturenShoturen);
}


Oniko::~Oniko()
{
	OBJECT_RELEASE(m_human);
	OBJECT_RELEASE(m_katana);
}

void Oniko::draw(){
	m_human->draw();
	m_katana->draw();
}