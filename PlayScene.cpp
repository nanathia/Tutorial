#include "PlayScene.h"
#include "PMXModel.h"
#include "Oniko.h"

PlayScene::PlayScene()
{
	OBJECT_CREATE(m_oniko, new Oniko());
}


PlayScene::~PlayScene()
{
	OBJECT_RELEASE(m_oniko);
}


void PlayScene::update(){
}


void PlayScene::draw(){
	m_oniko->draw();
}