#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

#include "Bala.h"
#include <vector>

USING_NS_CC;

class Jugador {
public:
	Jugador();
	~Jugador();

	// Crea al jugador en el centro
	bool creaSprite(Node *nodo);
	// Crea al jugador en la posicion indicada
	bool creaSprite(Node *nodo, Vec2 posInicial);

	Sprite *creaSpriteFisicas(Node *nodo, int tipoColision, int colisionaCon);

	//void mueve(bool htal, bool vcal, float cant);
	void mueve(bool izq, bool dch, bool arr, bool abj);
	void dispara(std::vector<Bala *> &pool);
	//void setPlayerSpeed(float);
	// jugador recibe un impacto. Oh noes!
	void impacto();

	//Vec2 getPosition();

private:
	const char *pathSprite = "spaceshipspr.png";
	const char *pathSonidoMuerte = "sonidos/explosion.wav";
	Sprite *sprite;
	float spriteScale = 0.5f;	// escala del sprite (a piñón)
	int zOrder = 0;	// zOrder del jugador
	//Vec2 position;	// ya tengo la posicion de sprite
	float playerSpeed = 500.0f;

	//void setPosition(Vec2);
	// intenta cargar el sprite del jugador, devuelve false en caso de error
	bool cargaSprite();
};

