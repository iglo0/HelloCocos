#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

//#include "Bala.h"
//#include <vector>

USING_NS_CC;

class Enemigo {
public:
	Enemigo();
	~Enemigo();

	bool creaSprite(Node *nodo, const char *path, float scale, int zOrder);
	Sprite *getSprite();
	float getScale();
	void setPosition(Vec2 nuPos);
	void setPosition(float x, float y);

	float tiempoDisparo = 1.0f;
	float tIni;

private:
	const char *pathSprite;
	Sprite *sprite;
	float spriteScale;
	int zOrder;

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

