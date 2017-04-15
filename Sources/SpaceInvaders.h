/*
// Horda de enemigos. Básicamente un array de n x m
Por ejemplo:
x	x	x	x	x
x	x	x 	x	x
x	x	x 	x	x
x	x	x 	x	x
*/

#pragma once

#include "cocos2d.h"

#include "Game.h"
#include "Enemy.h"

USING_NS_CC;

// creo que con este nombre queda más claro :D
class SpaceInvaders{
public:
	SpaceInvaders();
	~SpaceInvaders();

	enum tipoEnemigo{ tipo1, tipo2 };	// placeholder 

	void creaInvaders(Node *nodo, int dimX, int dimY, std::vector<Bullet *> &pool, float velMovHtal = 50.f, float velMovVcal = 10.f, int probDisparoAleat = RAND_MAX / 33, float dmg = 1.f);

private:
};

