#pragma once

#include "cocos2d.h"
#include "Game.h"

USING_NS_CC;

class GameActor{
public:
	GameActor();
	~GameActor();

	virtual Sprite *setSprite();
	virtual void update(float deltaT);
	Vec2 getPosition();
	void setPosition(Vec2);
	Sprite *getSprite();

	void mueve();
	bool mueveIzq, mueveDch, mueveArr, mueveAbj;
	void activa(Vec2 pos);
	void desactiva();
	bool isActive();

protected:
	Sprite *sprite;
	float gameActorSpeed;
	float gameActorHP, gameActorHPInicial = 1.0;
};


