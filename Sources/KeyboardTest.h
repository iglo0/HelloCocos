#ifndef _KEYBOARD_TEST_H_
#define _KEYBOARD_TEST_H_

//#include "cocos2d.h"

#include "cocos2d.h"

//#include "Menus.h"	// pantalla a la que vuelve

USING_NS_CC;


class KeyboardTest : public cocos2d::Layer{
public:
	KeyboardTest();
	~KeyboardTest();

	static cocos2d::Scene* createScene();
	virtual bool init();

	// quitar estos a pi�on y poner inputcomponent
	virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(KeyboardTest);

private:
	Label*            _label;
};

//class KeyboardTestScene : public TestScene{
//public:
//	virtual void runThisTest();
//};


#endif