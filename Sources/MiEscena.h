#pragma once

#include "cocos2d.h"

class MiEscena : public cocos2d::Layer{
public:
	static cocos2d::Scene *CreateScene();

	virtual bool init();

	// a selector callback
	void MenuCloseCallback(cocos2d::Ref *pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(MiEscena);
};

