#include "PlayScene.h"
#include "PMXModel.h"
#include "Oniko.h"
#include "GyuDon.h"
#include "Siba.h"

PlayScene::PlayScene()
{
	OBJECT_CREATE(m_oniko, new Oniko());
	//OBJECT_CREATE(m_siba, new Siba());
}


PlayScene::~PlayScene()
{
	OBJECT_RELEASE(m_oniko);
	//OBJECT_RELEASE(m_siba);
}


void PlayScene::update(){

}


void PlayScene::draw(){
	auto f = Director::instance()->framework();
	auto renderTarget = f->renderTargetView();
	f->deviceContext()->OMSetRenderTargets(1, &renderTarget, f->depthStencilView());

	m_oniko->draw();
	//m_siba->Draw();
}