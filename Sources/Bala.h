#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

#include <vector> // pool de balas

//#include <string>	

USING_NS_CC;

class Bala {
public:
	Bala(const char *pathSprite);
	~Bala();

	typedef enum tiposBala{ balaTipo1, balaTipo2};
	typedef enum sonidosBala { disparo, impacto};

	// wip
	void init(const char *pathSprite, const char *pathSonidoDisparo, const char *pathSonidoImpacto, enum tiposBala tb);	// inicializa todo el sistema para este tipo de Bala

	// mueve la bala segun su velocidad 
	void mueve();

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
	Sprite *sprite;
	//std::string rutaSprite;
	const char *rutaSprite;
	const char *rutaSonidoDisparo, *rutaSonidoImpacto;
	float velocidadAbs;
	cocos2d::Size visibleSize;
	tiposBala tipoBala;

	bool active = true;	// si esta bala está activa o está a la espera de ser utilizada
};

