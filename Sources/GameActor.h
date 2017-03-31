#pragma once

#include "cocos2d.h"
#include "Game.h"

USING_NS_CC;

class GameActor{
public:
	GameActor();
	~GameActor();

	Sprite *setSprite(Node *nodo, const char *ruta, const char *name, int tipoColision, int colisionaCon, bool isPolySprite = true);

	virtual void update(float deltaT);	// imagino que las clases derivadas implementarán sus propias versiones
	virtual void mueve();				//
	virtual void impacto(float dmg);

	Vec2 getPosition();
	void setPosition(Vec2);
	Sprite *getSprite();

	bool mueveIzq, mueveDch, mueveArr, mueveAbj;
	void activa(Vec2 pos);
	void desactiva();
	bool isActive();

protected:
	Sprite *sprite;
	float gameActorSpeed;
	float gameActorHP, gameActorHPInicial = 1.0;
};


