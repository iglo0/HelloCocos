#pragma once 

#include "cocos2d.h"
#include "Game.h"
#include "Menus.h"

// ----------------------------
#include "Player.h"
#include "Enemy.h"
#include "Pool.h"
#include "InputComponent.h"
#include "SpaceInvaders.h"
// ----------------------------
#include "Testz.h"
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
	void createGUI();

	// variables miembro privadas
	// --------------------------------------------------------------
	Game *gameInstance;	// cacheo la instancia de Game
	//Size visibleSize;
	//Vec2 origin;

	Player *player;
	InputComponent *inputComponent;

	// control de estados del juego
	bool iniciadoIntroNivel;
	bool iniciadoFinNivel;
	bool iniciadoMuerte;
	bool iniciadoGameOver;

	float tIniCambioEstado;

	// GUI! -> a Game
	//Label *lblMensajes;
	//Label *lblPuntos;
	//Label *lblHiScore;
	//Label *lblVidas;
};
