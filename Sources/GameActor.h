#pragma once

#include "cocos2d.h"	// necesario para declarar Sprite

class Movimiento;

USING_NS_CC;

class GameActor{
public:
	GameActor();
	~GameActor();

	// --------------------------------
	// test
	// --------------------------------
	void loadSpriteSheet(Node *);
	// --------------------------------

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
	virtual void mueve();
	virtual void mueve(Vec2);	//
	virtual void impacto(float dmg);	//
	//virtual void dispara() = 0;

	// TEST!!!!
	Movimiento *movimiento;

	// posibles funciones de control para update configurable
	//void mueveSeno(Vec2 posIni, double amplitude);	// Naming! funcion de movimiento *tipo* seno
	//void mueveSpaceInvader(Vec2 whatever, double someNumber);

	void mueveSeno();			// Naming! funcion de movimiento *tipo* seno
	void mueveSpaceInvader();
	void mueveSeno2();			// TODO: implementar
	void mueveDirigido();		// TODO: implementar
	void mueveHaciaProta();		

	// con dos ojones
	// guardo aquí los parámetros que usará este gameaCtor para moverse, en cada update
	// así no tengo que andarle pasando nada en cada "tick", lo defino una vez y él ya sabe lo que hay que hacer
	punteroAFuncionMovimiento funcionMovimientoActual;
	Vec2 funcionMovimientoPosIni;
	Vec2 funcionMovimientoTarget;
	double funcionMovimientoAmplitude;
	double funcionMovimientoSpeed;

	// movimiento hacia
	Vec2 mueveFrom;
	Vec2 mueveTo;
	Vec2 mueveDelta;
	// movimiento tipo "space invader"
	// *movimiento lateral -> esquina -> bajar -> pal otro lado -> bajar -> repetir
	// cada nave se controla a sí misma, necesita:
	// - dirección inicial
	// - vel htal/vel vcal
	// - limites x
	//bool spaceInvaderMovement_goingRight;			// movido a SpaceInvaders pa donde tira
	//bool spaceInvaderMovement_goingDown;			// movido a SpaceInvaders pa donde tira
	float spaceInvaderMovement_speedX;				// cómo de rápido se mueve lateralmente
	float spaceInvaderMovement_speedY;				// cómo de rápido baja
	float spaceInvaderMovement_xMin;				// dónde hace tope
	float spaceInvaderMovement_xMax;				// dónde hace tope
	float spaceInvaderMovement_vcalMoveAmount;		// cuánto baja cuando baja
	float spaceInvaderMovement_vcalMoveCurrTarget;	// cómo sé cuándo deja de bajar
	// -------------------------------------------------

	Vec2 getPosition();
	void setPosition(Vec2);
	Sprite *getSprite();

	bool mueveIzq, mueveDch, mueveArr, mueveAbj;	// hace visibles los controles de un GameActor para controlarlo desde fuera. Realmente solo los usa Player :/
	void activa(Vec2 pos);
	void desactiva();
	bool isActive();

	float gameActorSpeed;

protected:
	Sprite *sprite;
	float gameActorHP, gameActorHPInicial = 1.0;
	int gameActorPoints;
};