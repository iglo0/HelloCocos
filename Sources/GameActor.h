#pragma once

#include "cocos2d.h"	// necesario para declarar Sprite

class Movimiento;
class AnimSprites;		// "Componente" para animaciones

USING_NS_CC;

class GameActor{
public:
	enum gameActorTypes {bullet, enemy, player, destructible};

	GameActor();
	~GameActor();

	//typedef void(GameActor::*punteroAFuncionMovimiento)(Vec2, double);
	//v2: sin parametros, con variables miembro. Así no dependo tanto de la firma de la funcion
	typedef void(GameActor::*punteroAFuncionMovimiento)();
	typedef void(GameActor::*punteroAFuncionMovimientoV2)(Vec2);

	Sprite *setSprite(Node *nodo, const char *ruta, const char *name, int tipoColision, int colisionaCon, float initialScale = 1.0f);
	void setSprite(Sprite *);

	static void createAnimationPool(Node *nodo, std::vector<GameActor *> &pool, int poolSize, const char *animSetName);


	virtual void update(float deltaT);
	virtual void mueve(Vec2);	//
	virtual void impacto(float dmg);	//
	Vec2 getPosition();
	void setPosition(Vec2);
	void setPosition(float, float);
	Sprite *getSprite();
	AnimSprites *getAnimSprites();
	virtual void activa(Vec2 pos);
	virtual void activa(float x, float y);
	void desactiva();
	bool isActive();
	void setTTL(float ttl);

	float gameActorSpeed_;
	Movimiento *movimiento_;
	//punteroAFuncionMovimiento funcionMovimientoActual;
	gameActorTypes type_;

protected:
	Sprite *sprite_;
	float gameActorHP_, gameActorHPInicial_ = 1.0;
	int gameActorPoints_;
	bool estaActivo_;
	AnimSprites *animSprites_;

	// TODO: implementar el TTL
	float ttl_, tIni_, tEnd_;
	//Vec2 currPos_;
};