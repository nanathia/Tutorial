#include "PlayScene.h"
#include "PMXModel.h"
#include "Oniko.h"
#include "GyuDon.h"

PlayScene::PlayScene()
{
	//OBJECT_CREATE(m_oniko, new Oniko());
}


PlayScene::~PlayScene()
{
	//OBJECT_RELEASE(m_oniko);
}


void PlayScene::update(){
}


void PlayScene::draw(){
	auto f = Director::instance()->framework();
	auto renderTarget = f->renderTargetView();
	f->deviceContext()->OMSetRenderTargets(1, &renderTarget, f->depthStencilView());
	//m_oniko->draw();
}