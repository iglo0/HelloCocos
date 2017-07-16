#pragma once 

#include "cocos2d.h"	// necesario para declarar CREATE_FUNC y crear la escena en general

class GameState;

USING_NS_CC;

//#include "Game.h"
//#include "Menus.h"
// ----------------------------
//#include "Player.h"
//#include "Enemy.h"
//#include "Pool.h"
//#include "InputComponent.h"
//#include "SpaceInvaders.h"
// ----------------------------
//#include "Testz.h"
// ----------------------------

class Player;
class InputComponent;
class Game;
class GameActor;

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

	// HACK: Para cambiar de estado mayormente
	static void setGameState(GameState *);
	static void vuelveAlMenu();
private:
	// funciones miembro privadas
	// --------------------------------------------------------------
	void createGUI();
	void initLevel();

	void creaCasitas(int numba, float margen);
	void creaCasita(Vec2 esquinaInfIzq);
	GameActor *creaDestructible(Vec2 pos, int type=0);

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

	// HACK: --------------- PRUEBAS ----------------------------
	GameState *gameState;
	static Level *instance;
	// ----------------------------------------------------

};
