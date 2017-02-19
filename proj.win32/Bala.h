#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

#include <string>	// Sprite::create usa string para la ruta, playEffect usa const char *

USING_NS_CC;

class Bala{
public:
	Bala(std::string pathSprite, const char *pathSonido);
	~Bala();
	
	void mueve();

	bool setSprite(std::string ruta);
	Sprite *getSprite();	// de momento para addChild(...) desde la escena correspondiente
	std::string getRutaSprite();
	void setSonido(const char *ruta);
	const char *getSonido();

	void activar(Vec2 posInicial, float vel);
	void desActivar();

	void suena();
	bool isActive();

private:
	Sprite *sprite;
	std::string rutaSprite;
	const char *rutaSonido;
	float velocidadAbs;
	cocos2d::Size visibleSize;
	bool active = true;
};

