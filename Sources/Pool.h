#pragma once

#include "cocos2d.h"
#include "Game.h"

// ---------------
#include "GameActor.h"	// TODO: A ver si cuela para hacer "gen�ricos" los pools
#include "Bala.h"

#include <vector>
#include "Enemy.h"

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

	// TODO: intentaba usar un vector de GameActor para acoger un vector de Bullets : GameActor. Buen intento :D
	//static void activa(std::vector<GameActor *> &pool, Vec2 pos);
	//static void desactiva(GameActor &actor);
	static void activa(std::vector<Bullet *> &pool, Vec2 pos);
	static void desactiva(Bullet &bullet);

	// updates all GameActors in the pools
	static void updateAll(float deltaT);

	
	// ------------------------------------------------------------------------------------------------
	// GameActor pools
	// ------------------------------------------------------------------------------------------------
	// TODO: por qu� no puedo usarlo si es static? (da varios errores de linker)
	// OOKKIII para poder declarar una variable miembro como est�tica, debo inicializarla en el .cpp
	// TODO: de hecho, mejor en pool o que? Primero a ver si funciona
	// un array por cada... lo ideal ser�a sprite? crear� uno para cada tipo de bala y ya ver�
	static std::vector<Enemy *> currentEnemies;
	static std::vector<Bullet *> currentBulletsPlayerTipo1;
	static std::vector<Bullet *> currentBulletsTipo1;
	static std::vector<Bullet *> currentBulletsTipo2;

private:
};