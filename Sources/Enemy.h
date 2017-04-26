#pragma once

#include "cocos2d.h"
#include "Game.h"
#include "GameActor.h"
// para poder disparar
#include "Bullet.h"
#include <vector>
// -------------------
// TODO: wtf por que no se produce una referencia ciclica???
#include "Pool.h"
// -------------------

USING_NS_CC;

class Enemy : public GameActor{
public:
	// es menos engorroso as�. Puedo usar funcionMovimiento en vez de void(...:*...)(...)... etc
	typedef void(Enemy::*funcionControlEnemigo)(float);

	enum tiposEnemigo{tipo1,tipo2,tipoBoss};

	Enemy(Node *nodo, const char *pathSprite, const char *rutaSonidoMuerte, float scale, float rotation, float hp);
	Enemy(Node *nodo, enum tiposEnemigo);
	
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
	void funControlFireAtInterval(float interval);
	void funControlFireRandom(float wat);


	// TODO: Par�metros de control
	funcionControlEnemigo funcionControlActual;
	float funcionControlTiempoDisparo;
	int funcionControlProbDisparoAleatoria;	// 1..x. Dispara en 1, cuanto m�s alta menos probabilidad por frame.
	// ---------------------------------------------------------------------------------------------------------

	// a ve, par�metros de las balas que voy a disparar...
	// TODO: Acabo de sacar la clase Weapon y ya veo que me va a hacer falta de nuevo :D

	// TODO: �con qu� dispara un enemigo?
	// esto apuntar� a un pool en alg�n otro sitio
	std::vector<Bullet *> *poolMisBalas;
	// TODO: �Y para un enemigo con m�s tipos de ataques?


	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	// ---------------------------------------
	// ---------------------------------------

private:
	float tIniDisparo;

	void createEnemy(Node *nodo, const char *pathSprite, const char *rutaSonidoMuerte, float scale, float rotation, float hp);
};
