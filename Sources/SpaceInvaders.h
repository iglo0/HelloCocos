/*
// Horda de enemigos. Básicamente un array de n x m
Por ejemplo:
x	x	x	x	x
x	x	x 	x	x
x	x	x 	x	x
x	x	x 	x	x
*/

#pragma once

#include <vector>		// Enemy ya incluía vector y por eso podía declarar std::vector. Supongo que está mejor siendo explícito.
#include "cocos2d.h"	// la necesito para declarar Nodo* 
#include "Enemy.h"		// la necesito porque declaro un vector de Enemy::tiposEnemigo

//#include "Game.h"

//#include "Pool.h"

// creo que con este nombre queda más claro :D
class SpaceInvaders{
public:
	// esto para cargarlos luego de disco
	SpaceInvaders();
	// inicialización manual
	SpaceInvaders(int tamaX, int tamaY, float comprX=0.5f, float comprY=0.3f, float margX=100.0f, float margY=150.0f);
	~SpaceInvaders();

	enum tipoEnemigo{ tipo1, tipo2 };	// placeholder 

	// crea Invaders con parámetros
	void creaInvaders(Node *nodo, std::vector<Enemy::tiposEnemigo> &tipos, float velMovHtal = 50.f, float velMovVcal = 10.f, float vcalMoveAmount = 60.0f, int probDisparoAleat = 600);
	// crea Invaders usando las variables priva... públicas
	void creaInvaders(Node *nodo);

	Vec2 devuelvePosicionInicial(int dimX, int dimY);	// asigna una posicion inicial en pantalla según la posición en la matriz de invaders

	// Ign Prueba

	// ...y como son static, hay que declararlos también en el cpp: static int i ==> int i
	static bool spaceInvaderMovement_goingRight_;		// pa donde tira
	static bool spaceInvaderMovement_goingDown_;			// pa donde tira

	// TODO: para modificar el ratio de disparo: que disparen más cuantos menos queden.
	static float porcenInvadersVivos_;
	static int numInvadersInicial_;
	static int numInvadersVivos_;

	// Ign Nuevo! Con estas variables tengo todo lo que necesito
	std::vector<Enemy::tiposEnemigo> tipos_;
	float velMovHtal_, velMovVcal_, vcalMoveAmount_;
	int probDisparoAleat_;

private:
	size_t dimMaxX_, dimMaxY_;	// dimensiones de la "caja" de los invaders
	float compressX_;		// qué % de la pantalla usar
	float compressY_;
	float marginX_;			// donde empiezan los marcianos (contando desde la izquierda)
	float marginY_;			// donde empiezan los marcianos (contando desde arriba)

};

