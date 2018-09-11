#pragma once 

#include "cocos2d.h"	// necesario para declarar CREATE_FUNC y crear la escena en general

class InputText;

USING_NS_CC;
class EscenaVacia : public cocos2d::Layer {
public:
	// --------------------------------------------------------------
	// funciones miembro públicas
	// --------------------------------------------------------------

	~EscenaVacia();
	
    static cocos2d::Scene* createScene();
    virtual bool init();

	// keyboard input listeners
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

    // a selector callback
	void menuVuelveCallback(cocos2d::Ref* pSender);

	void vuelveAlMenu();

	// implement the "static create()" method manually
	CREATE_FUNC(EscenaVacia);

	// el gameloop
	void update(float delta) override;

private:
	InputText *inputText;

};
