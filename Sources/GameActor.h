#pragma once

#include "cocos2d.h"
#include "Game.h"

//class Enemy;

USING_NS_CC;

class GameActor{
public:
	GameActor();
	~GameActor();

	typedef void(GameActor::*funcionMovimiento)(Vec2, double);

	Sprite *setSprite(Node *nodo, const char *ruta, const char *name, int tipoColision, int colisionaCon, bool createPolySprite = true);

	//virtual void update(float deltaT);	// imagino que las clases derivadas implementar�n sus propias versiones
	// a ver que me entere... 
	// void(GameActor::*)() --> sin par�metros, funciona guay ?
	// TODO: void(GameActor::*)(Vec2) --> funciona para definir a la funcion pero no para pasar el parametro??? Se los tengo que pasar aparte??????

	// version a la que se le proporciona una funcion de movimiento
	// TODO: Por qu� dice el compilador que update no est� definido? Menos mal que solo es un warning
	//virtual void update(float deltaT, GameActor *instancia = nullptr, void(GameActor::*)(Vec2, double) = nullptr, Vec2 posIni = Vec2::ZERO, double amplitude = 600.0);
	virtual void update(float deltaT, GameActor *instancia = nullptr, funcionMovimiento=nullptr, Vec2 posIni = Vec2::ZERO, double amplitude = 600.0);
	virtual void mueve();				//
	virtual void impacto(float dmg);	//

	// TEST!!!!
	// posibles funciones de control para update configurable
	void mueveSeno(Vec2 posIni, double amplitude);	// Naming! funcion de movimiento *tipo* seno



	Vec2 getPosition();
	void setPosition(Vec2);
	Sprite *getSprite();

	bool mueveIzq, mueveDch, mueveArr, mueveAbj;	// hace visibles los controles de un GameActor para controlarlo desde fuera
	void activa(Vec2 pos);
	void desactiva();
	bool isActive();

protected:
	Sprite *sprite;
	float gameActorSpeed;
	float gameActorHP, gameActorHPInicial = 1.0;
};


