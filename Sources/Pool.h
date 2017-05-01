#pragma once

#include "cocos2d.h"
#include "Game.h"

// ---------------
#include "GameActor.h"	// TODO: A ver si cuela para hacer "genéricos" los pools
#include "Bullet.h"

#include <vector>
#include "Enemy.h"	// si no lo incluyo no puedo llamar a "enemy->activa(xxx)"
// TODO: si Pool incluye Enemy.h y Enemy incluye Pool.h... ¿cómo es que no casca?

USING_NS_CC;

// La idea de esta clase es crear los pools de sprites necesarios para que en vez de instanciar objetos cuando se necesiten,
// estos se encuentren pre-creados y solo haya que desactivarles la lógica y moverlos o hacerlos invisibles

// Originalmente iba a ser una clase más genérica con la idea de separar los gráficos del comportamiento, pero...
// a fecha de hoy mis colisiones funcionan con física, necesito saber a qué objeto pertenece un sprite
// para gestionar la lógica de las colisiones, y la física se monta sobre el sprite. Un embrollo...

// TL;DR: Esta clase tendrá especializaciones para cada tipo de objeto que quiera "poolear"

// TODO: Mirar si unas plantillas me ayudarían... (ya he mirado y he salido corriendo, una vez)

class Pool{
public:

	static void activa(std::vector<Bullet *> &pool, Vec2 pos);
	// TODO: Test con varios parámetros a cambiar al vuelo
	static void activa(std::vector<Bullet *> &pool, Vec2 pos, Node *nodoPadre, float initial_size, float initial_rotation, float dmg, int zOrder);
	static void desactiva(Bullet &bullet);

	// updates all GameActors in the pools
	static void updateAll(float deltaT);
	
	static void deletePools();
	static void disablePools();	// desactiva todos los pools, (útil p.ej. tras una muerte)

	// intento de plantilla con especializaciones
	// TODO: Y si no quiero usar la plantilla genérica y solo las especializaciones?
	// ¿qué gano con esto (si solo quiero las especializaciones) vs sobrecarga?
	template <class T>
	static void deletePool(std::vector<T *> &v);
	// especializaciones
	template <>
	static void deletePool<Enemy>(std::vector<Enemy *> &v);
	template <>
	static void deletePool<Bullet>(std::vector<Bullet *> &v);

	template <class T>
	static void disablePool(std::vector<T *> &v);
	template <>
	static void disablePool<Bullet>(std::vector<Bullet *> &v);

	// ------------------------------------------------------------------------------------------------
	// GameActor pools
	// ------------------------------------------------------------------------------------------------
	// Variables estáticas de una clase debo inicializarlas en el CPP. Como es un vector, se inicializa "solo", pero o lo declaro aquí o GOLPE DE REMO (Linker error)
	// comprobar que en el CPP está todo, ojo *allí* toca incluir Pool:: antes del nombre y quitar el static
	static std::vector<Enemy *> currentEnemies;
	static std::vector<Bullet *> currentBulletsPlayerTipo1;
	static std::vector<Bullet *> currentBulletsTipoBoss;
	static std::vector<Bullet *> currentBulletsTipo1;
	static std::vector<Bullet *> currentBulletsTipo2;

private:
};
