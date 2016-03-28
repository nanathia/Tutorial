#include "PlayScene.h"
#include "PMXModel.h"

PlayScene::PlayScene()
{
	m_pmx = new PMXModel("", "");
	m_pmx->autoRelease();
	m_pmx->retain();
}


PlayScene::~PlayScene()
{
	m_pmx->release();
	m_pmx = 0;
}


void PlayScene::update(){
}


void PlayScene::draw(){
	m_pmx->draw();
}