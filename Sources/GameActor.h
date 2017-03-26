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

protected:
	Sprite *sprite;
	float movementSpeed;
	float hp, hpInicial = 1.0;
};


