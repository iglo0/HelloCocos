#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

#include "Game.h"
#include "GameActor.h"
#include "Weapon.h"

//#include "Bala.h"
//#include <vector>

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
	
	

class EnemigoOOOLD {
public:
	EnemigoOOOLD();
	~EnemigoOOOLD();

	Sprite *creaSprite(Node *nodo, const char *pathSprite, const char *sonidoMuerte, float scale, int zOrder, float hp=1.f);
	Sprite *getSprite();
	float getScale();
	void setPosition(Vec2 nuPos);
	void setPosition(float x, float y);
	Vec2 getPosition();
	void mueveRelativo(Vec2); // suma Vec2 a la posicion actual
	void impacto(float dmg);
	void desActivar();
	void activar(Vec2);	// reactiva el enemigo en la posicion
	bool estaActivo();	// como está el enemigo?

	float tiempoDisparo = 1.0f;
	float tIni;

private:
	const char *pathSprite;
	const char *pathSonidoMuerte;
	Sprite *sprite;
	float spriteScale;
	int zOrder;
	float puntosDeGolpeIniciales;
	float puntosDeGolpeActuales;

};

