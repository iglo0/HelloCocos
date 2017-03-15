#pragma once

#include "cocos2d.h"

#include "Game.h"
#include "Pool.h"
#include "Bala.h"

USING_NS_CC;

class GameObject{
public:
	//GameObject();
	//~GameObject();

	Vec2 getPosition();
	Sprite *getSprite();
	Sprite *setSprite(Node *nodoPadre, const char *pathSprite, int zOrder);

protected:
	Sprite *sprite;

};

// ... : public GameObject... -> necesario para acceder a los miembros públicos de GameObject desde fuera (sin este public -> "error funcion xxxx es inaccesible")
class Nave : public GameObject{
public:

protected:
	Pool *balas;
};

class Enemigo2 : public Nave{
public:

protected:
};

class EnemigoXXXX {
public:
	EnemigoXXXX();
	~EnemigoXXXX();

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

	/*
	// Crea al jugador en el centro
	bool creaSprite(Node *nodo);
	// Crea al jugador en la posicion indicada
	bool creaSprite(Node *nodo, Vec2 posInicial);

	//void mueve(bool htal, bool vcal, float cant);
	void mueve(bool izq, bool dch, bool arr, bool abj);
	void dispara(std::vector<Bala *> &pool);
	//void setPlayerSpeed(float);

	//Vec2 getPosition();

private:
	int zOrder = 0;	// zOrder del jugador
	//Vec2 position;	// ya tengo la posicion de sprite
	float playerSpeed = 500.0f;

	//void setPosition(Vec2);
	// intenta cargar el sprite del jugador, devuelve false en caso de error
	bool cargaSprite();
	*/
};

