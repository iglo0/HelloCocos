#pragma once

#include "cocos2d.h"
#include "Game.h"

#include <vector>

USING_NS_CC;

class Pool{
public:

	Pool(Node *);
	~Pool();

	// scale y rotation están más pensados para la carga inicial, es decir un sprite que se que su tamaño es X y que lo quiero sacar siempre a 180º o algo
	// si el tipoColision!=None -> con fisica
	// devuelve false si la creacion falla
	bool creaPoolSprites(int cant, const char *spritePath, int zOrder, float scale, float rotation, const char *name, int tipoColision, int colisionaCon, void *clasePadre);
	// sin fisica
	bool creaPoolSprites(int cant, const char *spritePath, int zOrder, float scale, float rotation, const char *name);

	// devuelve uno libre
	Sprite *activa(Vec2 pos);
	// desactiva uno
	void desActiva(Sprite *);

protected:

	std::vector<Sprite *> pool;
	Node *nodoPadre;

};
