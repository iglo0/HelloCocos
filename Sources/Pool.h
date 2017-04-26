#pragma once

#include "cocos2d.h"
#include "Game.h"

// ---------------
#include "GameActor.h"	// TODO: A ver si cuela para hacer "gen�ricos" los pools
#include "Bullet.h"

#include <vector>
#include "Enemy.h"	// si no lo incluyo no puedo llamar a "enemy->activa(xxx)"
// TODO: si Pool incluye Enemy.h y Enemy incluye Pool.h... �c�mo es que no casca?

USING_NS_CC;

// La idea de esta clase es crear los pools de sprites necesarios para que en vez de instanciar objetos cuando se necesiten,
// estos se encuentren pre-creados y solo haya que desactivarles la l�gica y moverlos o hacerlos invisibles

// Originalmente iba a ser una clase m�s gen�rica con la idea de separar los gr�ficos del comportamiento, pero...
// a fecha de hoy mis colisiones funcionan con f�sica, necesito saber a qu� objeto pertenece un sprite
// para gestionar la l�gica de las colisiones, y la f�sica se monta sobre el sprite. Un embrollo...

// TL;DR: Esta clase tendr� especializaciones para cada tipo de objeto que quiera "poolear"

// TODO: Mirar si unas plantillas me ayudar�an... (ya he mirado y he salido corriendo, una vez)

class Pool{
public:

	static void activa(std::vector<Bullet *> &pool, Vec2 pos);
	// TODO: Test con varios par�metros a cambiar al vuelo
	static void activa(std::vector<Bullet *> &pool, Vec2 pos, Node *nodoPadre, float initial_size, float initial_rotation, float dmg, int zOrder);
	static void desactiva(Bullet &bullet);

	// updates all GameActors in the pools
	static void updateAll(float deltaT);
	
	// ------------------------------------------------------------------------------------------------
	// GameActor pools
	// ------------------------------------------------------------------------------------------------
	// Variables est�ticas de una clase debo inicializarlas en el CPP. Como es un vector, se inicializa "solo", pero o lo declaro aqu� o GOLPE DE REMO (Linker error)
	// comprobar que en el CPP est� todo, ojo *all�* toca incluir Pool:: antes del nombre y quitar el static
	static std::vector<Enemy *> currentEnemies;
	static std::vector<Bullet *> currentBulletsPlayerTipo1;
	static std::vector<Bullet *> currentBulletsTipoBoss;
	static std::vector<Bullet *> currentBulletsTipo1;
	static std::vector<Bullet *> currentBulletsTipo2;

private:
};
