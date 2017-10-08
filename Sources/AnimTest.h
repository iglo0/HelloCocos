#pragma once 

#include "cocos2d.h"	// necesario para declarar CREATE_FUNC y crear la escena en general

USING_NS_CC;

class AnimSprites;
class Bullet;

class AnimTest : public cocos2d::Layer
{
public:
	// funciones miembro públicas
	// --------------------------------------------------------------

	~AnimTest();
	
	#pragma region fluff
    static cocos2d::Scene* createScene();
    virtual bool init();

	// keyboard input listeners
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

    // a selector callback
	void menuVuelveCallback(cocos2d::Ref* pSender);
	void vuelveAlMenu();

	// implement the "static create()" method manually
	CREATE_FUNC(AnimTest);
	#pragma endregion
	
	// el gameloop
	void update(float delta) override;

	void mueveNodo();

private:
	#pragma region xml

	void xmlLoadTest(const char *);
	// unused: prueba de conceto
	void xmlSaveTest(const char *);



	#pragma endregion

	void miInit();
	//void mueveNodo();

	bool mueveArr_, mueveAbj_, mueveIzq_, mueveDch_;

	//AnimSprites *animaciones_;
	std::vector<Bullet *> balas_;
};
