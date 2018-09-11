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
	// funciones miembro p�blicas
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
	// evento colisi�n. Debe ser p�blico.
	bool onContactBegin(PhysicsContact &contacto);
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

	// HACK: Para cambiar de estado mayormente
	static void setGameState(GameState *);
	void vuelveAlMenu();
	//static void pantallaHiScore();	// salta a la pantalla puntuaciones
	//void gameOver();	// acciones post-gameOver
	static void apuntaHiScore();	// 
	static void siguienteNivel();	// para llamar a "avanzaOleada" desde fuera

private:
	// funciones miembro privadas
	// --------------------------------------------------------------
	void createGUI();
	void initLevel();

	void creaCasitas(int numba, float margen);
	void creaCasita(Vec2 esquinaInfIzq);
	GameActor *creaDestructible(Vec2 pos, int type=0);

	// TODO: test captura de pantalla
	void afterCaptured(bool succeed, const std::string& outputFile);
	bool capturing_ = false;
	// --------------------------------------------------------------

	// usa el vector de niveles para sacar la siguiente oleada en caso de haberla
	void avanzaOleada();

	// variables miembro privadas
	// --------------------------------------------------------------
	Game *gameInstance;	// cacheo la instancia de Game
	static Level *instance;	// TODO: cacheo esta instancia para... �que? �gui? �estados?
	
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
	std::unordered_map<int, SpaceInvaders *>::iterator levelsIterator_;
	SpaceInvaders *actualLevel_;
	int oleadaNum_;
	// --------------------------------------------------------------

	// �apa para que la lectura de teclas se comporte distinto y lea las teclas del juego o el nombre del jugador
	bool apuntandoRecords_ = false;
};
