#pragma once

#include "cocos2d.h"
#include "Game.h"
#include "Bala.h"

#include <vector>

USING_NS_CC;


// La idea de esta clase es crear los pools de sprites necesarios para que en vez de instanciar objetos cuando se necesiten,
// estos se encuentren pre-creados y solo haya que desactivarles la l�gica y moverlos o hacerlos invisibles

// Originalmente iba a ser una clase m�s gen�rica con la idea de separar los gr�ficos del comportamiento, pero...
// a fecha de hoy las colisiones funcionan con f�sica, necesito saber a qu� objeto pertenece un sprite
// para gestionar la l�gica de las colisiones, y la f�sica se monta sobre el sprite. Un embrollo...

// TL;DR: Esta clase tendr� especializaciones para cada tipo de objeto que quiera "poolear"

// TODO: Mirar si unas plantillas me ayudar�an... (ya he mirado y he salido corriendo, una vez)

// TODO: 

class Pool{
public:

	Pool();
	~Pool();

	// con f�sica

	// scale y rotation est�n m�s pensados para la carga inicial, es decir un sprite que se que su tama�o es X y que lo quiero sacar siempre a 180� o algo
	// si el tipoColision!=None -> con fisica
	// devuelve false si la creacion falla
	bool creaPoolBalas(Node *nodo, int cant, const char *spritePath, float scale, float rotation, const char *name, float speed, const char *pathSonidoDisparo, const char *pathSonidoImpacto, int tipoColision, int colisionaCon);
	
	// los enemigos van en la horda by the moment
	//bool creaPoolEnemigos(int cant, const char *spritePath, float scale, float rotation, const char *name, int tipoColision, int colisionaCon);


	//bool creaPoolSprites(int cant, const char *spritePath, float scale, float rotation, const char *name, int tipoColision, int colisionaCon);
	// sin fisica
	//bool creaPoolSprites(int cant, const char *spritePath, float scale, float rotation, const char *name);

	// devuelve uno libre
	// si le pasas un padre, cuelga este sprite del otro y pos pasa a ser relativo al padre
	// si le pasas nullptr, pos es absoluta
	Sprite *activa(Vec2 posRel, Sprite *spritePadre, int zOrder);
	Sprite *activa(Vec2 posAbs, Node *nodePadre, int zOrder);
	// desactiva uno
	void desActiva(Sprite *);

protected:

	void creaBala();
	void creaEnemigo();

	std::vector<Sprite *> pool;
	//Node *nodoPadre;

};

