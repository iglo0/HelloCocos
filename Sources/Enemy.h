#pragma once

#include "cocos2d.h"
#include "Game.h"
#include "GameActor.h"
// para poder disparar
#include "Bullet.h"
#include <vector>
// -------------------
// wtf por que no se produce una referencia ciclica ahora???
#include "Pool.h"
// -------------------

USING_NS_CC;

class Enemy : public GameActor{
public:
	// es menos engorroso as�. Puedo usar funcionMovimiento en vez de void(...:*...)(...)... etc
	typedef void(Enemy::*funcionControlEnemigo)(float);
	//typedef void(Enemy::*funcionControlEnemigo)(float);

	Enemy(Node *nodo, const char *pathSprite, const char *rutaSonidoMuerte, float scale, float rotation, float hp);
	~Enemy();
	void impacto(float) override;

	void update(float deltaT) override;


	// ---------------------------------------
	// TEST!
	// ---------------------------------------
	// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

	// todo: sobrecargarlo desde GameActor???
	void dispara();

	// funciones de control
	void funControl1(float segundos);


	// TODO: Par�metros de control
	funcionControlEnemigo funcionControlActual;
	float funcionControlTiempoDisparo;

	// a ve, par�metros de las balas que voy a disparar...
	// TODO: Acabo de sacar la clase Weapon y ya veo que me va a hacer falta de nuevo :D

	// TODO: �con qu� dispara un enemigo?
	// esto apuntar� a un pool en alg�n otro sitio
	std::vector<Bullet *> *poolMisBalas;


	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	// ---------------------------------------
	// ---------------------------------------

private:
	float tIniDisparo;
};
