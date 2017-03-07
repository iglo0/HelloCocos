#pragma once

#include "cocos2d.h"

#include "Game.h"
#include "Enemigo.h"

USING_NS_CC;

// Horda de enemigos. Básicamente un array de n x m 
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

	const char *enemigoT1PathSprite = "aliensprite2.png";// a piñón
	const char *enemigoT2PathSprite = "Spaceship15.png";// a piñón
	const char *enemigoPathSonidoMuerte = "sonidos/invaderkilled.wav";// a piñón
	float escalaEnemigoT1 = 0.25f;// a piñón
	float escalaEnemigoT2 = 0.15f;// a piñón
	//Vec2 dimensionesEnemigoT1;// calculadas
	//Vec2 dimensionesEnemigoT2;// calculadas

	int dimensionesHordaX, dimensionesHordaY;
	float separacionHordaX; //= 200.f;
	float separacionHordaY;	// = 200.f;

	Node *nodoPadre;	// el nodo al que añadir la horda

	Vec2 coordenadasInicioHorda;		// TODO: posición inicial de la horda (en bloque). La posición superior izquierda? inferior izquierda?

						
	// "array" de n x m enemigos
	std::vector <std::vector<Enemigo *> > horda;
	
	void creaHorda(int dimX, int dimY);
	// crea una fila de n enemigos
	void creaFila(int cant, enum tipoEnemigo t, int numFila);	// numFila es para que sepa que coordenadas le tocan

	// crea un enemigo de tipo t, en la posicion de la matriz x,y
	// TODO: y si en vez de eso los creo como hijos de un objeto por definir?
	// así moviendo el objeto ese movería todos los hijos a la vez... mmm...
	// (pensando como con Unity, no sé si Cocos se comportará igual)
	Enemigo *creaEnemigo(enum tipoEnemigo t, int x, int y);

	// dado un punto de la matriz, en qué coordenadas de pantalla habría que pintar la nave correspondiente?
	Vec2 coordenadasNaveEnXY(int, int);

	// ha sido destruida la horda?
	bool hordaDestruida();

	
};

