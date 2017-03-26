#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "AudioEngine.h"
#include "Pool.h"

#include "Game.h"

#include <vector> // pool de balas

//#include <string>	

USING_NS_CC;

class Bala {
public:
	Bala(const char *pathSprite);	// bala sin fisica
	Bala(const char *name, const char *pathSprite, int tipoColision, int colisionoCon, float dmg=1.0f);	// bala con física
	~Bala();
	float getDanyoBala();

	enum tiposBala{ balaTipo1, balaTipo2};
	enum sonidosBala { disparo, impacto};

	// mueve la bala segun su velocidad 
	void update();

	bool setSpriteConFisica(const char *name, const char *ruta, int tipoColision, int colisionaCon);

	// carga sprite para este tipo de bala
	bool setSprite(const char *ruta);
	Sprite *getSprite();

	// incializa la variable y pregacarga el sonido
	void setSonido(sonidosBala tipo, const char *ruta);
	//const char *getSonido();

	void activar(Vec2 posInicial);
	void desActivar();

	void reproduceSonido(sonidosBala);

	// la velocidad de esta vala
	void setVelocidad(float);


	bool isActive();

private:
	void creaPool();


	Sprite *sprite;
	//std::string rutaSprite;
	//const char *rutaSprite;
	const char *rutaSonidoDisparo, *rutaSonidoImpacto;
	float velocidadAbs;
	//cocos2d::Size visibleSize;
	tiposBala tipoBala;
	float danyoBala;

	bool active = true;	// si esta bala está activa o está a la espera de ser utilizada

	// TODO: Y si la bala es solo la definición común de un tipo y cuando la quiero disparar solo tengo que sacar un sprite del pool y moverlo con los parámetros de la bala?
	//std::vector<Sprite *> pool;
	//Pool *pool2;
};
