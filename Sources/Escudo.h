#pragma once

#include "cocos2d.h"
#include "Game.h"

USING_NS_CC;

//class Escudo : GameObject{
//public:
//	Escudo(Node *parent, float vida, float regen);
//	~Escudo();
//
//	// recibe un daño y devuelve potencialmente el daño que no ha podido absorber
//	float impacto(float dmg);
//	void update(float deltaT);
//	Sprite *getSprite();
//
//protected:
//	//Sprite *sprite;
//	//const char *spritePath = "shields.png";
//	float vidaEscudoActual, vidaEscudoInicial, regenRate;
//	const char *rutaImagen = "shields.png";
//	bool escudoDestruido;
//	float escudoDestruidoTIni;
//	const float esperaInicioRegenTrasDestruccion = 10.0f;
//};