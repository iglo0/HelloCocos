#pragma once

#include "cocos2d.h"
#include "Game.h"

#include <vector>

USING_NS_CC;

// TODO: Pool de Sprites pero estos llevan información según el objeto que los usa... mmm...
// La información es para gestionar las colisiones, porque van con física y la física en el sprite... mmm....
// TODO: Pruebas con plantillas
class Pool{
public:

	Pool();
	//Pool(Node *);
	~Pool();

	// scale y rotation están más pensados para la carga inicial, es decir un sprite que se que su tamaño es X y que lo quiero sacar siempre a 180º o algo
	// si el tipoColision!=None -> con fisica
	// TODO: clasePadre = ¿si colisiona, que tipo de objeto es? ¿creo?
	// devuelve false si la creacion falla
	bool creaPoolSprites(int cant, const char *spritePath, float scale, float rotation, const char *name, int tipoColision, int colisionaCon, void *clasePadre);
	bool creaPoolBalas(int cant, const char *spritePath, float scale, float rotation, const char *name, int tipoColision, int colisionaCon);
	bool creaPoolEnemigos(int cant, const char *spritePath, float scale, float rotation, const char *name, int tipoColision, int colisionaCon);
	
	// sin fisica
	bool creaPoolSprites(int cant, const char *spritePath, float scale, float rotation, const char *name);

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

