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
	Enemy(Node *nodo, const char *pathSprite, const char *rutaSonidoMuerte, float scale, float rotation, float hp);
	~Enemy();
	void impacto(float) override;

	Weapon *weapon;

private:

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

