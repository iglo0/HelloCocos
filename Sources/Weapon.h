#pragma once

#include "cocos2d.h"
#include "Game.h"
#include "Bala.h"	// necesario para std::vector<Bullet *>
//#include "Pool.h" // referencia circular
//class Pool;		// referencia incompleta (falta activa(...))
#include <vector>


USING_NS_CC;

class Weapon{
public:
	//Weapon();
	Weapon(std::vector<Bullet *> &pool);
	~Weapon();

	void fire(Vec2 posIni);
	//void createBulletPool(Node *nodo, int poolSize, const char *name, const char *pathSprite, const char *pathSonidoDisparo, const char *pathSonidoImpacto, float speed, float dmg, int tipoColision, int colisionoCon, float initialScale = 1.0f);

	// HACK: Temporalmente lo pongo p�blico. A ver c�mo lo gestiono y si me vale y tal...
	// lo defino como referencia para que apunte a otro creado por ah�, y de paso obligar a que tenga un valor
	std::vector<Bullet *> &bulletPool;	// obligatorio inicializarlo -> constructor cambiado

private:
	// delay between shots
	float shotDelay = 0.1f;
	float shotDamage = 1.0f;
};

