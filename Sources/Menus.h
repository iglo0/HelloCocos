#pragma once

#include "cocos2d.h"

#include "Level1.h"
#include "Level.h"
#include "KeyboardTest.h"
#include "PolyspriteTest.h"
#include "Game.h"

class Menus : public cocos2d::Layer{
public:
	static cocos2d::Scene *CreateScene();

	virtual bool init();

	// a selector callback
	// OJO!
	// Según cuantos parámetros tenga, se llama con CC_CALLBACK_0, CC_CALLBACK_1, CC_CALLBACK_2... 
	void MenuCloseCallback(cocos2d::Ref *pSender);
	void MenuJugarCallback(cocos2d::Ref *pSender);
	void MenuJugar2Callback(cocos2d::Ref *pSender);
	void MenuKeyTestCallback(cocos2d::Ref *pSender);
	void MenuPolyspritesCallback(cocos2d::Ref *pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(Menus);
};

