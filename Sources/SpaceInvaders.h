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
	SpaceInvaders(int tamaX, int tamaY, float comprX=0.5f, float comprY=0.3f, float margX=100.0f, float margY=150.0f);
	~SpaceInvaders();

	enum tipoEnemigo{ tipo1, tipo2 };	// placeholder 

	void creaInvaders(Node *nodo, std::vector<Enemy::tiposEnemigo> &tipos, std::vector<Bullet *> &pool, float velMovHtal = 50.f, float velMovVcal = 10.f, int probDisparoAleat = 600);
	//void creaFilaInvaders(Node *nodo, Enemy::tiposEnemigo tipo, int dimX, std::vector<Bullet *> &pool, float velMovHtal = 50.f, float velMovVcal = 10.f, int probDisparoAleat = RAND_MAX / 33, float dmg = 1.f);

	Vec2 devuelvePosicionInicial(int dimX, int dimY);	// asigna una posicion inicial en pantalla según la posición en la matriz de invaders


	// Ign Prueba

	// y definir los statics en el cpp
	static bool spaceInvaderMovement_goingRight;		// pa donde tira
	static bool spaceInvaderMovement_goingDown;			// pa donde tira


private:
	size_t dimMaxX, dimMaxY;	// dimensiones de la "caja" de los invaders
	float compressX;		// qué % de la pantalla usar
	float compressY;
	float marginX;			// donde empiezan los marcianos (contando desde la izquierda)
	float marginY;			// donde empiezan los marcianos (contando desde arriba)

};

