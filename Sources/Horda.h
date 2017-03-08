#pragma once

#include "cocos2d.h"

#include "Game.h"
#include "Enemigo.h"
#include "Bala.h"

USING_NS_CC;

class Horda : cocos2d::Layer{

/*
// Horda de enemigos. Básicamente un array de n x m
Por ejemplo:
x	x	x	x	x
x	x	x 	x	x
x	x	x 	x	x
x	x	x 	x	x
*/

public:
	// -----------------------------------------------------------------------------------------------------------------------------------------
	// constructor
	// -----------------------------------------------------------------------------------------------------------------------------------------
	Horda(Node *, Vec2 posInicio);

	// -----------------------------------------------------------------------------------------------------------------------------------------
	// variables miembro
	// -----------------------------------------------------------------------------------------------------------------------------------------
	Node *nodoPadre; // el nodo al que añadir la horda (o dónde añado los sprites)
	enum tipoEnemigo{ tipo1, tipo2 };	// placeholder 

	// -----------------------------------------------------------------------------------------------------------------------------------------
	// funciones públicas
	// -----------------------------------------------------------------------------------------------------------------------------------------
	void creaHorda(int dimX, int dimY, std::vector<Bala *> &pool);
	void tick(); // TODO: llamo a este metodo desde fuera para ejecutar un ciclo? Cómo debería organizar esto?

protected:
	// -----------------------------------------------------------------------------------------------------------------------------------------
	// variables internas
	// -----------------------------------------------------------------------------------------------------------------------------------------
	const char *enemigoT1PathSprite = "aliensprite2.png";// a piñón
	const char *enemigoT2PathSprite = "Spaceship15.png";// a piñón
	const char *enemigoPathSonidoMuerte = "sonidos/invaderkilled.wav";// a piñón
	float escalaEnemigoT1 = 0.25f;// a piñón
	float escalaEnemigoT2 = 0.30f;// a piñón

	// forma de la horda
	int dimensionesHordaX, dimensionesHordaY;
	float separacionHordaX;	// autocalculado, se ajusta al ancho menos un margen
	float separacionHordaY; // autocal... no, a piñón. La idea es que salga desde arriba y vaya haciendo ciclos izq-dch y bajando poco a poco
	Vec2 coordenadasInicioHorda; // TODO: posición inicial de la horda (en bloque). La posición superior izquierda? inferior izquierda?

	// ................................................................................................................................
	// dinámicas de los bichos
	bool moverALaIzquierda = true; // en que direccion se mueve la horda (primero a un lado y luego al otro y así hasta ...)
	float velMovimientoHorizontal = 50.f; // cuanto mueven en horizontal hasta llegar al borde y dar la vuelta
	float velMovimientoVertical = 10.f; // no se
	int nCiclosMovimientoHorizontal; // cuantos ciclos llevan
	int probabilidadDisparoAleatoria = RAND_MAX / 33; // Probabilidad cada "tick" que alguno de los monstruos dispare
	// TODO: si lo llamo cada Tick cuantos más fps más te disparan O:-)
	Vec2 desplazamientoHorda; // cual es el desplazamiento actual de la horda con respecto a origen
	bool cambiarDireccion(); // decide si toca cambiar de direccion
	// ................................................................................................................................

	// "array" de n x m enemigos. Los vectores no solo son fáciles de definir, encima se pueden acceder por índices como un array
	std::vector <std::vector<Enemigo *> > horda;

	std::vector<Bala *> poolBalas;	// pool para sus disparos. Crearlo desde fuera o dentro? mmm...

	// -----------------------------------------------------------------------------------------------------------------------------------------
	// funciones privadas
	// -----------------------------------------------------------------------------------------------------------------------------------------

	// crea una fila de n enemigos
	void creaFila(int cant, enum tipoEnemigo t, int numFila);	// numFila es para que sepa que coordenadas le tocan

	// crea un enemigo de tipo t, en la posicion de la matriz x,y
	// TODO: y si en vez de eso los creo como hijos de un objeto por definir?
	// así moviendo el objeto ese movería todos los hijos a la vez... mmm...
	// (pensando como con Unity, no sé si Cocos se comportará igual)
	Enemigo *creaEnemigo(enum tipoEnemigo t, int x, int y);

	// dado un punto de la matriz, en qué coordenadas de pantalla habría que pintar la nave correspondiente?
	Vec2 coordenadasInicialesNaveEnXY(int, int);
	Vec2 coordenadasNaveEnXY(int, int);
	Enemigo *enemigoEnXY(int, int);

	// dinámicas de la horda
	void mueve();
	void dispara();
	void baja();

	// ha sido destruida la horda?
	bool hordaDestruida();

	
};

