#pragma once

// OJO!!!
// Lo ideal para evitarle complicaciones al compilador es incluir en el .h solo aquello que se requiera en la declaraci�n, y mover al .cpp el resto de includes necesarios para la definici�n.
// Por ejemplo: si voy a usar un puntero de clase Enemy, en la cabecera basta con la declaraci�n adelantada (class Enemy) y luego en el cuerpo incorporar el #include...

#include "cocos2d.h"
#include "Game.h"

// ---------------

#include <vector>

class Enemy;
class Bullet;
class GameActor;
//class AnimSprites;

USING_NS_CC;

//
//// a ver, y esto... funciona?
//class MiSprite: cocos2d::Sprite{
//public:
//	MiSprite();
//	~MiSprite();
//
//	// uso un "creador" est�tico por copiar un poco a Sprite, mayormente :P
//	static MiSprite *createWithSpriteFrameName(const std::string &spriteFrameName);
//
//	void activa();
//	void activa(Vec2);
//
//	void desActiva();
//
//	bool inUse;
//	Sprite *sprite;
//
//protected:
//
//};

// La idea de esta clase es crear los pools de sprites necesarios para que en vez de instanciar objetos cuando se necesiten,
// estos se encuentren pre-creados y solo haya que desactivarles la l�gica y moverlos o hacerlos invisibles

// Originalmente iba a ser una clase m�s gen�rica con la idea de separar los gr�ficos del comportamiento, pero...
// a fecha de hoy mis colisiones funcionan con f�sica, necesito saber a qu� objeto pertenece un sprite
// para gestionar la l�gica de las colisiones, y la f�sica se monta sobre el sprite. Un embrollo...

// TL;DR: Esta clase tendr� especializaciones para cada tipo de objeto que quiera "poolear"

// TODO: Mirar si unas plantillas me ayudar�an... (ya he mirado y he salido corriendo, una vez)

class Pool{
public:

	static Bullet *activa(std::vector<Bullet *> &pool, Vec2 pos);
	static void desactiva(Bullet &bullet);
	static GameActor *activa(std::vector<GameActor *> &pool, Vec2 pos);
	static void desactiva(GameActor &bullet);

	// updates all GameActors in the pools
	static void updateAll(float deltaT);
	// HACK: quiero ver explosiones al morir y poco m�s
	// deber�a? hacer uno gen�rico pero no s� si podr�a evitar especializarlo para cada tipo de pool, de momento esto es lo que hay
	static void updateMinimal(float deltaT);
	
	static void deletePools();
	static void disablePools();	// desactiva todos los pools, (�til p.ej. tras una muerte)

	// intento de plantilla con especializaciones
	// TODO: Y si no quiero usar la plantilla gen�rica y solo las especializaciones?
	// �qu� gano con esto (si solo quiero las especializaciones) vs sobrecarga?

	// OJO: Ser�a mejor hacer una clase pool gen�rica que gestione la memoria (ver pool_template)
	// y ya la especializar� para que funcione con los tipos que quiera

	template <class T>
	static void deletePool(std::vector<T *> &v);
	// especializaciones
	template <>
	static void deletePool<Enemy>(std::vector<Enemy *> &v);
	template <>
	static void deletePool<Bullet>(std::vector<Bullet *> &v);
	template <>
	static void deletePool<GameActor>(std::vector<GameActor *> &v);

	template <class T>
	static void disablePool(std::vector<T *> &v);
	// especializaciones
	template <>
	static void disablePool<Bullet>(std::vector<Bullet *> &v);
	template <>
	static void disablePool<GameActor>(std::vector<GameActor *> &v);

	// ------------------------------------------------------------------------------------------------
	// GameActor pools
	// ------------------------------------------------------------------------------------------------
	// Variables est�ticas de una clase debo inicializarlas en el CPP. Como es un vector, se inicializa "solo", pero o lo declaro aqu� o GOLPE DE REMO (Linker error)
	// comprobar que en el CPP est� todo, ojo *all�* toca incluir Pool:: antes del nombre y quitar el static
	static std::vector<Enemy *> currentEnemies;
	static std::vector<Bullet *> currentBulletsPlayerTipo1;
	static std::vector<Bullet *> currentBulletsTipoBossHoming;
	static std::vector<Bullet *> currentBulletsTipoNormal;
	static std::vector<Bullet *> currentBulletsTipo2;
	static std::vector<GameActor *> currentExplosions;
	static std::vector<GameActor *> currentImpacts;
	//static std::vector<AnimSprites *> currentAnimfucksgrlz;
private:
};
