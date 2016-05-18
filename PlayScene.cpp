#include "PlayScene.h"
#include "PMXModel.h"
#include "GyuDon.h"
#include "Siba.h"
#include "OBJModel.h"

PlayScene::PlayScene()
{
	//OBJECT_CREATE(m_oniko, new PMXModel("dat/pmx/oniko", "“ú–{‹Sq0.15.pmx"));
	//OBJECT_CREATE(m_skyDome, new OBJModel("dat/obj/stage", "stage"));
}


PlayScene::~PlayScene()
{
	//OBJECT_RELEASE(m_oniko);
	//OBJECT_RELEASE(m_skyDome);
}


void PlayScene::update(){

}


void PlayScene::draw(){
	auto f = Director::instance()->framework();
	auto renderTarget = f->renderTargetView();
	f->deviceContext()->OMSetRenderTargets(1, &renderTarget, f->depthStencilView());

	//m_oniko->draw();
	//m_skyDome->draw();
}