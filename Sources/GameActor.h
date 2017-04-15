#pragma once

#include "cocos2d.h"
#include "Game.h"

//class Enemy;

USING_NS_CC;

class GameActor{
public:
	GameActor();
	~GameActor();

	//typedef void(GameActor::*punteroAFuncionMovimiento)(Vec2, double);
	//v2: sin parametros, con variables miembro. Así no dependo tanto de la firma de la funcion
	typedef void(GameActor::*punteroAFuncionMovimiento)();

	Sprite *setSprite(Node *nodo, const char *ruta, const char *name, int tipoColision, int colisionaCon, float initialScale = 1.0f, bool createPolySprite = true);

	//virtual void update(float deltaT);	// imagino que las clases derivadas implementarán sus propias versiones
	// a ver que me entere... 
	// void(GameActor::*)() --> sin parámetros, funciona guay ?
	// TODO: void(GameActor::*)(Vec2) --> funciona para definir a la funcion pero no para pasar el parametro??? Se los tengo que pasar aparte??????

	// version a la que se le proporciona una funcion de movimiento
	// TODO: Por qué dice el compilador que update no está definido? Menos mal que solo es un warning
	//virtual void update(float deltaT, GameActor *instancia = nullptr, void(GameActor::*)(Vec2, double) = nullptr, Vec2 posIni = Vec2::ZERO, double amplitude = 600.0);
	//virtual void update(float deltaT, GameActor *instancia = nullptr, punteroAFuncionMovimiento = nullptr, Vec2 posIni = Vec2::ZERO, double amplitude = 600.0);
	virtual void update(float deltaT);
	virtual void mueve();				//
	virtual void impacto(float dmg);	//

	// TEST!!!!
	// posibles funciones de control para update configurable
	//void mueveSeno(Vec2 posIni, double amplitude);	// Naming! funcion de movimiento *tipo* seno
	//void mueveSpaceInvader(Vec2 whatever, double someNumber);

	void mueveSeno();	// Naming! funcion de movimiento *tipo* seno
	void mueveSpaceInvader();


	// con dos ojones
	// guardo aquí los parámetros que usará este gameaCtor para moverse, en cada update
	// así no tengo que andarle pasando nada en cada "tick", lo defino una vez y él ya sabe lo que hay que hacer
	punteroAFuncionMovimiento funcionMovimientoActual;
	Vec2 funcionMovimientoPosIni;
	double funcionMovimientoAmplitude;
	double funcionMovimientoSpeed;
	// -------------------------------------------------

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