#ifndef __LEVEL1_SCENE_H__
#define __LEVEL1_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

#include "Game.h"
#include "Bala.h"
#include "Jugador.h"
#include "Enemigo.h"
#include "Menus.h"
#include "SpaceInvaders.h"
// testz
#include "Pool.h"
//#include "Escudo.h"
// ------------------------

#include <vector>

USING_NS_CC;

class Level1 : public cocos2d::Layer
{
public:
	// funciones miembro públicas
	// --------------------------------------------------------------

	~Level1();
	
    static cocos2d::Scene* createScene();
    virtual bool init();
    
    // a selector callback
	void menuVuelveCallback(cocos2d::Ref* pSender);

	// TODO: keyboard listeners --> a InputComponent. Eventualmente...
	virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

    // implement the "static create()" method manually
    CREATE_FUNC(Level1);

	// el gameloop
	void update(float delta) override;

	// otras funciones
	void mueveBalas(std::vector<BalaOLD *> pool);	// procesa pool de balas
	//void controlaProta();
	void creaEnemigos();
	//void creaPoolBalas(std::vector<Bala *> *pool, int cant, const char *pathSpriteBala, const char *pathSonidoDisparo, const char *pathSonidoImpacto, float scale, float speed);
	void creaPoolBalasFisica(std::vector<BalaOLD *> *pool, int cant, const char *pathSpriteBala, const char *pathSonidoDisparo, const char *pathSonidoImpacto, float scale, float speed, int tipoColision, int colisionaCon);
	void desactivaPool(std::vector<BalaOLD *> &pool);

	// --------------------------------------------------------------
	// Físicas (o sea, Colisiones)
	// --------------------------------------------------------------
	// evento colisión. Debe ser público.
	bool onContactBegin(PhysicsContact &contacto);
	// gestiona un impacto desde el sprite colisionado. El sprite deberá tener defindo setUserData() y setTag() para que esto funcione
	void gestionaImpacto(Sprite *sprite, float dmg);

	// variables miembro públicas
	// --------------------------------------------------------------

private:
	// funciones miembro privadas
	// --------------------------------------------------------------
	void precargaSonidosDelNivel();
	void subeNivel();
	HordaDEPRECATED *hordaNivel(int nivel);
	float calculaDanyoImpacto(Sprite *, Sprite *);	// calcula el daño que se hacen dos sprites al chocar (caso más común: balas vs cosas)

	// variables miembro privadas
	// --------------------------------------------------------------
	Game *gameInstance;	// cacheo la instancia de Game

	//bool mueveIzq, mueveDch, mueveArr, mueveAbj;
	//bool dispara;
	bool sale;	// salir de la pantalla
	float protaSpeed = 500.0f, balaSpeed = 333.3f, balaEnemigaSpeed = -333.3f;// , enemigoSpeed = 100.0f;
	int nivelActual = 1;
	cocos2d::Label *lblMensajes;
	bool iniciaTemporizadorCambioEstado = false;	// TODO: todavía no sé muy bien cómo hacerlo, voy a intentar un temporizador genérico para esperas
	float tiempoIntro = 2.0f;
	float tiempoJugando = 20.0f;
	float tiempoFinNivel = 2.0f;
	float tiempoMuerte = 5.0f;
	float tiempoFinHorda = 2.0f;
	float tIniCambiaEstado;
	std::string mensajeIntro;	// TODO: lo creo como string porque lo voy a estar modificando. ¿Por qué no char *?
	const char *mensajeFin = "GAME OVER";
	const char *mensajeMuerte = "HUEHUEHUEHUEHUE";
	const char *mensajeFinHorda = "Congrats y tal. Ahora vas a flipar!";

	Size visibleSize;
	Vec2 origin;

	Vector<Sprite *> enemigosDeprecated;
	std::vector<EnemigoOOOLD *> enemigos;

	HordaDEPRECATED *hordaActual; // TODO: la horda actual. De momento en pruebas

	// pruebas
	std::vector<BalaOLD *> poolBalas;	
	//Pool *poolBalas;	// pool para mis disparos

	std::vector<BalaOLD *> poolBalasGordas;	// pool para disparos del jefe
	std::vector<BalaOLD *> poolBalasEnemigas;	// pool para disparos de los aliens normales

	Jugador *player;


};

#endif // __LEVEL1_SCENE_H__
