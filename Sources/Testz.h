#pragma once

#include "cocos2d.h"
#include "Game.h"
#include <vector>

USING_NS_CC;

// AKA Pool de Balas?
class Ammo{
public:
	Ammo();
	~Ammo();

	std::vector<Sprite *> bulletPool;

private:

};

class Weapon{
public:
	Weapon();
	~Weapon();

	void fire(Vec2 posIni);

private:
	Ammo *ammo;
	float rateOfFire = 0.1f;
	float shotDamage = 1.0f;
};

