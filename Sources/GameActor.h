#pragma once

#include "cocos2d.h"	// necesario para declarar Sprite

class Movimiento;

USING_NS_CC;

class GameActor{
public:
	GameActor();
	~GameActor();

	//typedef void(GameActor::*punteroAFuncionMovimiento)(Vec2, double);
	//v2: sin parametros, con variables miembro. Así no dependo tanto de la firma de la funcion
	typedef void(GameActor::*punteroAFuncionMovimiento)();
	typedef void(GameActor::*punteroAFuncionMovimientoV2)(Vec2);

	Sprite *setSprite(Node *nodo, const char *ruta, const char *name, int tipoColision, int colisionaCon, float initialScale = 1.0f);

	//virtual void update(float deltaT);	// imagino que las clases derivadas implementarán sus propias versiones
	// a ver que me entere... 
	// void(GameActor::*)() --> sin parámetros, funciona guay ?
	// TODO: void(GameActor::*)(Vec2) --> funciona para definir a la funcion pero no para pasar el parametro??? Se los tengo que pasar aparte??????

	// version a la que se le proporciona una funcion de movimiento
	//virtual void update(float deltaT, GameActor *instancia = nullptr, void(GameActor::*)(Vec2, double) = nullptr, Vec2 posIni = Vec2::ZERO, double amplitude = 600.0);
	//virtual void update(float deltaT, GameActor *instancia = nullptr, punteroAFuncionMovimiento = nullptr, Vec2 posIni = Vec2::ZERO, double amplitude = 600.0);
	virtual void update(float deltaT);
	//virtual void mueve();
	virtual void mueve(Vec2);	//
	virtual void impacto(float dmg);	//
	//virtual void dispara() = 0;

	// TEST!!!!
	Movimiento *movimiento;
	//punteroAFuncionMovimiento funcionMovimientoActual;

	Vec2 getPosition();
	void setPosition(Vec2);
	Sprite *getSprite();

	//bool mueveIzq, mueveDch, mueveArr, mueveAbj;	// hace visibles los controles de un GameActor para controlarlo desde fuera. Realmente solo los usa Player :/
	// permito redefinirlo según tipos
	virtual void activa(Vec2 pos);
	void desactiva();
	bool isActive();

	float gameActorSpeed;

protected:
	Sprite *sprite;
	float gameActorHP, gameActorHPInicial = 1.0;
	int gameActorPoints;
};