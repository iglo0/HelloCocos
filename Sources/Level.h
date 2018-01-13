#pragma once 

#include "cocos2d.h"	// necesario para declarar CREATE_FUNC y crear la escena en general
USING_NS_CC;

class GameState;
class Player;
class InputComponent;
class Game;
class GameActor;
class SpaceInvaders;

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

	// usa el vector de niveles para sacar la siguiente oleada en caso de haberla
	void avanzaOleada();

	// TODO: test captura de pantalla
	void afterCaptured(bool succeed, const std::string& outputFile);
	bool capturing_ = false;
	// --------------------------------------------------------------

	// variables miembro privadas
	// --------------------------------------------------------------
	Game *gameInstance;	// cacheo la instancia de Game
	static Level *instance;	// TODO: cacheo esta instancia para... ¿que? ¿gui? ¿estados?
	
	Player *player;
	InputComponent *inputComponent;

	// control de estados del juego
	bool iniciadoIntroNivel;
	bool iniciadoFinNivel;
	bool iniciadoMuerte;
	bool iniciadoGameOver;

	float tIniCambioEstado;

	GameState *gameState;
	
	// variables de las oleadas
	// --------------------------------------------------------------
	std::unordered_map<int, SpaceInvaders *> levels_;
	SpaceInvaders *actualLevel_;
	int oleadaNum_;
	// --------------------------------------------------------------
};
