/*
// Horda de enemigos. B�sicamente un array de n x m
Por ejemplo:
x	x	x	x	x
x	x	x 	x	x
x	x	x 	x	x
x	x	x 	x	x
*/

#pragma once

#include <vector>		// Enemy ya inclu�a vector y por eso pod�a declarar std::vector. Supongo que est� mejor siendo expl�cito.
#include "cocos2d.h"	// la necesito para declarar Nodo* 
#include "Enemy.h"		// la necesito porque declaro un vector de Enemy::tiposEnemigo

//#include "Game.h"

//#include "Pool.h"

// creo que con este nombre queda m�s claro :D
class SpaceInvaders{
public:
	SpaceInvaders(int tamaX, int tamaY, float comprX=0.5f, float comprY=0.3f, float margX=100.0f, float margY=150.0f);
	~SpaceInvaders();

	enum tipoEnemigo{ tipo1, tipo2 };	// placeholder 

	//void creaInvaders(Node *nodo, std::vector<Enemy::tiposEnemigo> &tipos, std::vector<Bullet *> &pool, float velMovHtal = 50.f, float velMovVcal = 10.f, float vcalMoveAmount = 60.0f, int probDisparoAleat = 600);
	void creaInvaders(Node *nodo, std::vector<Enemy::tiposEnemigo> &tipos, float velMovHtal = 50.f, float velMovVcal = 10.f, float vcalMoveAmount = 60.0f, int probDisparoAleat = 600);

	Vec2 devuelvePosicionInicial(int dimX, int dimY);	// asigna una posicion inicial en pantalla seg�n la posici�n en la matriz de invaders


	// Ign Prueba

	// ...y como son static, hay que declararlos tambi�n en el cpp: static int i ==> int i
	static bool spaceInvaderMovement_goingRight;		// pa donde tira
	static bool spaceInvaderMovement_goingDown;			// pa donde tira

	// TODO: para modificar el ratio de disparo: que disparen m�s cuantos menos queden.
	static float porcenInvadersVivos;
	static int numInvadersInicial;
	static int numInvadersVivos;

private:
	size_t dimMaxX, dimMaxY;	// dimensiones de la "caja" de los invaders
	float compressX;		// qu� % de la pantalla usar
	float compressY;
	float marginX;			// donde empiezan los marcianos (contando desde la izquierda)
	float marginY;			// donde empiezan los marcianos (contando desde arriba)

};

