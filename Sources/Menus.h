#pragma once

#include "cocos2d.h"
USING_NS_CC; // using namespace cocos2d


//#include "Level.h"
//#include "KeyboardTest.h"
//#include "PolyspriteTest.h"
//#include "Game.h"

class Menus : public cocos2d::Layer{
public:
	static cocos2d::Scene *createScene();

	virtual bool init();

	// a selector callback
	// OJO!
	// Seg�n cuantos par�metros tenga, se llama con CC_CALLBACK_0, CC_CALLBACK_1, CC_CALLBACK_2... 
	void MenuCloseCallback(cocos2d::Ref *pSender);
	//void MenuJugarCallback(cocos2d::Ref *pSender);
	void MenuJugar2Callback(cocos2d::Ref *pSender);
	void MenuKeyTestCallback(cocos2d::Ref *pSender);
	void MenuPolyspritesCallback(cocos2d::Ref *pSender);
	void MenuEscenaVaciaCallback(cocos2d::Ref *pSender);
	void MenuAnimTestCallback(cocos2d::Ref *pSender);
	void MenuHiScoresCallback(cocos2d::Ref *pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(Menus);
};

