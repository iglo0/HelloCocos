#pragma once

#include "cocos2d.h"

#include "Game.h"
#include "Enemigo.h"

USING_NS_CC;

// Horda de enemigos. B�sicamente un array de n x m 
/* 
Por ejemplo:
	x	x	x	x	x
	x	x	x	x	x
	x	x	x	x	x
		x	x	x	
	|
*/
class Horda{
public:

	Horda(Node *, Vec2 posInicio);

	enum tipoEnemigo{ tipo1, tipo2 };

	const char *enemigoT1PathSprite = "aliensprite2.png";// a pi��n
	const char *enemigoT2PathSprite = "Spaceship15.png";// a pi��n
	const char *enemigoPathSonidoMuerte = "sonidos/invaderkilled.wav";// a pi��n
	float escalaEnemigoT1 = 0.4f;// a pi��n
	float escalaEnemigoT2 = 0.5f;// a pi��n
	Vec2 dimensionesEnemigoT1;// calculadas
	Vec2 dimensionesEnemigoT2;// calculadas

	Vec2 posInicio;		// TODO: posici�n inicial de la horda (en bloque). La posici�n superior izquierda? inferior izquierda?
	Node *nodoPadre;	// el nodo al que a�adir la horda

	// array de n x m enemigos
	std::vector <std::vector<Enemigo *> > horda;
	
	void creaHorda(int dimX, int dimY);
	// crea una fila de n enemigos
	void creaFila(int cant, enum tipoEnemigo t, float coordY, float separacionX);
	// crea un enemigo de tipo t, fila posinicial.y-coordY, y x+desplX
	Enemigo *creaEnemigo(enum tipoEnemigo t, float coordY, float desplX);

};

