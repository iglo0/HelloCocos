#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

#include "Bala.h"
#include "Pool.h"
#include <vector>
// Testz --------------------
#include "Escudo.h"


USING_NS_CC;

class Jugador {
public:
	Jugador(float hp);
	~Jugador();

	// Crea al jugador en el centro
	bool creaSprite(Node *nodo);
	// Crea al jugador en la posicion indicada
	bool creaSprite(Node *nodo, Vec2 posInicial);

	Sprite *creaSpriteFisicas(Node *nodo, int tipoColision, int colisionaCon);

	//void mueve(bool htal, bool vcal, float cant);
	void mueve(bool izq, bool dch, bool arr, bool abj);
	void dispara(std::vector<BalaOLD *> &pool);
	//void dispara();
	//void setPlayerSpeed(float);
	// jugador recibe un impacto. Oh noes! Devuelve si eso lo ha destruido o no
	bool impacto(float dmg);
	void resetea();
	Vec2 getPosition();
	Sprite *getSprite();
	void setPoolBalas(PoolDEPRECATED *pool);	// le asigna algo que disparar
	void setPoolBalas(std::vector<BalaOLD *> &pool);

	// funcion update a invocar desde fuera. Utiliza las variables mueveXXX, disparo y sale para saber qué le toca hacer
	void update(float delta);

	// variables miembro públicas
	// ----------------------------------------
	//Escudo *escudo;
	// variables para que el jugador sepa qué hacer cuando llegue su update
	bool mueveIzq, mueveDch, mueveArr, mueveAbj, disparo, sale;


private:
	//void setPosition(Vec2);
	// intenta cargar el sprite del jugador, devuelve false en caso de error
	bool cargaSprite();

	Game *gameInstance;
	const char *pathSprite = "spaceshipspr.png";
	const char *pathSonidoMuerte = "sonidos/explosion.wav";
	Sprite *sprite;
	float spriteScale = 0.5f;	// escala del sprite (a piñón)
	int zOrder = 0;	// zOrder del jugador
	float playerSpeed = 300.0f;
	float puntosDeGolpeActuales, puntosDeGolpeIniciales;
	float delayDisparo = 0.1f;	// TODO: delay entre disparos (esto podría estar definido en la bala mas bien, así distintos tipos de disparo tendrían distinto delay)
	float tIniDelay;	// empieza a contar desde el ultimo disparo
	//Pool *poolBalasActual;
	std::vector<BalaOLD *> poolBalasActual;
};

