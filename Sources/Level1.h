#ifndef __LEVEL1_SCENE_H__
#define __LEVEL1_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

#include "Game.h"
#include "Bala.h"
#include "Jugador.h"
#include "Enemigo.h"
#include "Menus.h"
#include "Horda.h"

#include <vector>

USING_NS_CC;

class Level1 : public cocos2d::Layer
{
public:
	~Level1();

    static cocos2d::Scene* createScene();
    virtual bool init();
    
    // a selector callback
	void menuVuelveCallback(cocos2d::Ref* pSender);

	// keyboard listeners
	virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

    // implement the "static create()" method manually
    CREATE_FUNC(Level1);

	// el gameloop
	void update(float delta) override;

	// otras funciones

	//bool mueveBala(Sprite *bala, float cuanto);	// mueve una bala
	void mueveBalas(std::vector<Bala *> pool);	// procesa pool de balas
	void controlaProta();
	//void creaNaveProta();
	void creaEnemigos();
	//void mueveEnemigos(float cuanto);
	//void mueveEnemigo(Enemigo *enemigo, float cuanto);
	//void enemigoDispara(Sprite *enemigo);
	void creaPoolBalas(std::vector<Bala *> *pool, int cant, const char *pathSpriteBala, const char *pathSonidoDisparo, const char *pathSonidoImpacto, float scale, float speed);
	void creaPoolBalasFisica(std::vector<Bala *> *pool, int cant, const char *pathSpriteBala, const char *pathSonidoDisparo, const char *pathSonidoImpacto, float scale, float speed, int tipoColision, int colisionaCon);
	// gestiona un impacto desde el sprite colisionado. El sprite deberá tener defindo setUserData() y setTag() para que esto funcione
	void gestionaImpacto(Sprite *sprite);

	// --------------------------------------------------------------
	// Físicas (o sea, Colisiones)
	// --------------------------------------------------------------

	// evento colisión. Debe ser público.
	bool onContactBegin(PhysicsContact &contacto);

private:
	Game *gameInstance;	// cacheo la instancia de Game

	//Sprite *protaSprite,*balaSprite;
	bool mueveIzq, mueveDch, mueveArr, mueveAbj;
	bool dispara;
	bool sale;	// salir de la pantalla
	float protaSpeed = 500.0f, balaSpeed = 333.3f, balaEnemigaSpeed = -333.3f;// , enemigoSpeed = 100.0f;
	int nivelActual = 1;
	//float deltaT;
	//float tiempoTranscurrido;
	cocos2d::Label *lblMensajes;
	bool iniciaTemporizadorCambioEstado = false;	// TODO: todavía no sé muy bien cómo hacerlo, voy a intentar un temporizador genérico para esperas
	float tiempoIntro = 2.0f;
	float tiempoJugando = 20.0f;
	float tiempoFinNivel = 2.0f;
	float tiempoMuerte = 5.0f;
	float tiempoFinHorda = 2.0f;
	float tIniCambiaEstado;
	//const char *mensajeIntro = "Level 1 START!";
	std::string mensajeIntro;
	const char *mensajeFin = "GAME OVER";
	const char *mensajeMuerte = "HUEHUEHUEHUEHUE";
	const char *mensajeFinHorda = "Congrats y tal. Ahora vas a flipar!";

	Size visibleSize;
	Vec2 origin;

	Vector<Sprite *> enemigosDeprecated;
	std::vector<Enemigo *> enemigos;

	Horda *hordaActual; // TODO: la horda actual. De momento en pruebas

	// pruebas
	std::vector<Bala *> poolBalas;	// pool para mis disparos
	std::vector<Bala *> poolBalasGordas;	// pool para disparos del jefe
	std::vector<Bala *> poolBalasEnemigas;	// pool para disparos de los aliens normales

	Jugador *player;

	void precargaSonidosDelNivel();
	void subeNivel();
	Horda *hordaNivel(int nivel);

};

#endif // __LEVEL1_SCENE_H__
