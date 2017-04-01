#pragma once 

#include "cocos2d.h"
#include "Game.h"
#include "Menus.h"

// ----------------------------
#include "Player.h"
#include "InputComponent.h"
// ----------------------------

USING_NS_CC;

class Level : public cocos2d::Layer
{
public:
	// funciones miembro públicas
	// --------------------------------------------------------------

	~Level();
	
    static cocos2d::Scene* createScene();
    virtual bool init();
    
    // a selector callback
	void menuVuelveCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(Level);

	// el gameloop
	void update(float delta) override;

	// --------------------------------------------------------------
	// Listeners
	// --------------------------------------------------------------
	// evento colisión. Debe ser público.
	bool onContactBegin(PhysicsContact &contacto);
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

private:
	// funciones miembro privadas
	// --------------------------------------------------------------

	// variables miembro privadas
	// --------------------------------------------------------------
	Game *gameInstance;	// cacheo la instancia de Game
	Size visibleSize;
	Vec2 origin;

	Player *player;
	InputComponent *inputComponent;

	// TODO: una vez creados, me parece que los enemigos se deberían gestionar solos. De momento y por ver algo...
	Enemy *enemy;
};
