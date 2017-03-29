#pragma once

#include "cocos2d.h"
#include "Game.h"
#include <vector>

#include "Bala.h"
#include "Pool.h"

USING_NS_CC;

class Weapon{
public:
	Weapon();
	~Weapon();

	void fire(Vec2 posIni);
	void createBulletPool(Node *nodo, int poolSize, const char *name, const char *pathSprite, const char *pathSonidoDisparo, const char *pathSonidoImpacto, float speed, float dmg, int tipoColision, int colisionoCon);

	// HACK: Temporalmente lo pongo público. A ver cómo lo gestiono y si me vale y tal...
	std::vector<Bullet *> bulletPool;

private:
	// delay between shots
	float shotDelay = 0.1f;
	float shotDamage = 1.0f;


};

