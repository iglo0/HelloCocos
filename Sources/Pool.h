#pragma once

#include "cocos2d.h"
#include "Game.h"

// ---------------
#include "GameActor.h"	// TODO: A ver si cuela para hacer "genéricos" los pools
#include "Bala.h"

#include <vector>

USING_NS_CC;


class Pool{
public:

	// TODO: intentaba usar un vector de GameActor para acoger un vector de Bullets : GameActor. Buen intento :D
	//static void activa(std::vector<GameActor *> &pool, Vec2 pos);
	//static void desactiva(GameActor &actor);
	static void activa(std::vector<Bullet *> &pool, Vec2 pos);
	static void desactiva(Bullet &bullet);

private:
};


// La idea de esta clase es crear los pools de sprites necesarios para que en vez de instanciar objetos cuando se necesiten,
// estos se encuentren pre-creados y solo haya que desactivarles la lógica y moverlos o hacerlos invisibles

// Originalmente iba a ser una clase más genérica con la idea de separar los gráficos del comportamiento, pero...
// a fecha de hoy mis colisiones funcionan con física, necesito saber a qué objeto pertenece un sprite
// para gestionar la lógica de las colisiones, y la física se monta sobre el sprite. Un embrollo...

// TL;DR: Esta clase tendrá especializaciones para cada tipo de objeto que quiera "poolear"

// TODO: Mirar si unas plantillas me ayudarían... (ya he mirado y he salido corriendo, una vez)

#include "Bala.h"	// para PoolDEPRECATED ?_?

class PoolDEPRECATED{
public:

	PoolDEPRECATED();
	~PoolDEPRECATED();

	// con física

	// scale y rotation están más pensados para la carga inicial, es decir un sprite que se que su tamaño es X y que lo quiero sacar siempre a 180º o algo
	// si el tipoColision!=None -> con fisica
	// devuelve false si la creacion falla
	bool creaPoolBalas(Node *nodo, int cant, const char *spritePath, float scale, float rotation, const char *name, float speed, const char *pathSonidoDisparo, const char *pathSonidoImpacto, int tipoColision, int colisionaCon);

	// devuelve uno libre
	// si le pasas un padre, cuelga este sprite del otro y pos pasa a ser relativo al padre
	// si le pasas nullptr, pos es absoluta
	Sprite *activa(Vec2 posRel, Sprite *spritePadre, int zOrder);
	Sprite *activa(Vec2 posAbs, Node *nodePadre, int zOrder);
	// desactiva uno
	void desActiva(Sprite *);

protected:

	std::vector<Sprite *> pool;
	//Node *nodoPadre;

};

