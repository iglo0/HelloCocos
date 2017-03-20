#pragma once

#include "cocos2d.h"


USING_NS_CC;

class GameObject{
public:
	GameObject();
	~GameObject();

	Sprite *getSprite();
	Vec2 getPosition();
	Sprite *cargaSprite(Node *padre, int zOrder, const char *rutaImagen);
	void setPosition(Vec2);

protected:
	Sprite *sprite;
};

