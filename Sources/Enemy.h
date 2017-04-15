#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

#include "Game.h"
#include "GameActor.h"
#include "Weapon.h"

#include <vector>
#include "Pool.h"

USING_NS_CC;

class Enemy : public GameActor{
public:
	// es menos engorroso así. Puedo usar funcionMovimiento en vez de void(...:*...)(...)... etc
	typedef void(Enemy::*funcionControlEnemigo)(float);
	//typedef void(Enemy::*funcionControlEnemigo)(float);

	Enemy(Node *nodo, const char *pathSprite, const char *rutaSonidoMuerte, float scale, float rotation, float hp);
	~Enemy();
	void impacto(float) override;

	////void update(float deltaT, GameActor *instancia = nullptr, void(GameActor::*)(Vec2, double) = nullptr, Vec2 posIni = Vec2::ZERO, double amplitude = 600.0) override;
	//void update(float deltaT, GameActor *instancia = nullptr, GameActor::punteroAFuncionMovimiento=nullptr, Vec2 posIni = Vec2::ZERO, double amplitude = 600.0) override;
	void update(float deltaT) override;


	// ---------------------------------------
	// ---------------------------------------
	// ---------------------------------------
	// TEST!
	// ---------------------------------------
	// ---------------------------------------
	// ---------------------------------------

	// funciones de control
	void funControl1(float segundos);


	// TODO: Parámetros de control
	funcionControlEnemigo funcionControlActual;
	float funcionControlTiempoDisparo;

	// ---------------------------------------
	// ---------------------------------------
	// ---------------------------------------
	Weapon *weapon;

private:
	float tIniDisparo;
	//bool enemigoDisparando;
};
