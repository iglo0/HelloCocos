#pragma once

#include "cocos2d.h"

#include "Game.h"
#include "Enemigo.h"
#include "Bala.h"

USING_NS_CC;

class Horda : cocos2d::Layer{

/*
// Horda de enemigos. B�sicamente un array de n x m
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
	~Horda();

	// -----------------------------------------------------------------------------------------------------------------------------------------
	// variables miembro
	// -----------------------------------------------------------------------------------------------------------------------------------------
	enum tipoEnemigo{ tipo1, tipo2 };	// placeholder 

	// -----------------------------------------------------------------------------------------------------------------------------------------
	// funciones p�blicas
	// -----------------------------------------------------------------------------------------------------------------------------------------
	void creaHorda(int dimX, int dimY, std::vector<Bala *> &pool, float velMovHtal = 50.f, float velMovVcal = 10.f, int probDisparoAleat = RAND_MAX / 33);
	void tick(); // TODO: llamo a este metodo desde fuera para ejecutar un ciclo? C�mo deber�a organizar esto?

protected:
	// -----------------------------------------------------------------------------------------------------------------------------------------
	// variables internas
	// -----------------------------------------------------------------------------------------------------------------------------------------
	const char *enemigoT1PathSprite = "aliensprite2.png";// a pi��n
	const char *enemigoT2PathSprite = "Spaceship15.png";// a pi��n
	const char *enemigoPathSonidoMuerte = "sonidos/invaderkilled.wav";// a pi��n
	float escalaEnemigoT1 = 0.25f;// a pi��n
	float escalaEnemigoT2 = 0.30f;// a pi��n

	// forma de la horda
	int dimensionesHordaX, dimensionesHordaY;
	float separacionHordaX;	// autocalculado, se ajusta al ancho menos un margen
	float separacionHordaY; // autocal... no, a pi��n. La idea es que salga desde arriba y vaya haciendo ciclos izq-dch y bajando poco a poco
	Vec2 coordenadasInicioHorda; // TODO: posici�n inicial de la horda (en bloque). La posici�n superior izquierda? inferior izquierda?

	// ................................................................................................................................
	// din�micas de los bichos
	bool moverALaIzquierda = true; // en que direccion se mueve la horda (primero a un lado y luego al otro y as� hasta ...)
	float velMovimientoHorizontal; // cuanto mueven en horizontal hasta llegar al borde y dar la vuelta
	float velMovimientoVertical; // no se
	int nCiclosMovimientoHorizontal; // cuantos ciclos llevan
	int probabilidadDisparoAleatoria; // Probabilidad cada "tick" que alguno de los monstruos dispare
	// TODO: si lo llamo cada Tick cuantos m�s fps m�s te disparan O:-)
	Vec2 desplazamientoHorda; // cual es el desplazamiento actual de la horda con respecto a ori... NO! con respecto al frame anterior.
	bool cambiarDireccion(); // decide si toca cambiar de direccion
	Enemigo *enemigoIzquierdo, *enemigoDerecho;	// enemigos que uso como control de donde est� el borde de la horda
	// ................................................................................................................................


	// -----------------------------------------------------------------------------------------------------------------------------------------
	// funciones privadas
	// -----------------------------------------------------------------------------------------------------------------------------------------

	// crea una fila de n enemigos
	void creaFila(int cant, enum tipoEnemigo t, int numFila);	// numFila es para que sepa que coordenadas le tocan

	// crea un enemigo de tipo t, en la posicion de la matriz x,y
	// TODO: y si en vez de eso los creo como hijos de un objeto por definir?
	// as� moviendo el objeto ese mover�a todos los hijos a la vez... mmm...
	// (pensando como con Unity, no s� si Cocos se comportar� igual)
	Enemigo *creaEnemigo(enum tipoEnemigo t, int x, int y);

	// dado un punto de la matriz, en qu� coordenadas de pantalla habr�a que pintar la nave correspondiente?
	Vec2 coordenadasInicialesNaveEnXY(int, int);
	//Vec2 coordenadasNaveEnXY(int, int);
	//Enemigo *enemigoEnXY(int, int);

	// din�micas de la horda
	void mueve();
	void dispara();
	void baja();

	// -----------------------------------------------------------------------------------------------------------------------------------------
	// variables privadas
	// -----------------------------------------------------------------------------------------------------------------------------------------
	// el nodo del que "cuelga" la horda. Solo lo uso para quitar sprites remanentes al borrar la clase
	Node *nodoPadre;

	// "array" de n x m enemigos. Los vectores no solo son f�ciles de definir, encima se pueden acceder por �ndices como un array
	//std::vector <std::vector<Enemigo *> > horda;
	std::vector<Enemigo *> horda;
	std::vector<Bala *> poolBalas;	// pool para sus disparos. Crearlo desde fuera o dentro? mmm...
	std::vector<Enemigo *> listaEnemigosVivos();

	// ha sido destruida la horda?
	bool hordaDestruida();

	
};

