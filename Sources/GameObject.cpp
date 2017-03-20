#include "GameObject.h"

GameObject::GameObject(){
}

GameObject::~GameObject(){
}

Sprite *GameObject::getSprite(){
	return sprite;
}

Vec2 GameObject::getPosition(){
	if(sprite){
		return sprite->getPosition();
	} else{
		CCLOG("OJO! GameObject::getPosition() sin sprite");
		return Vec2::ZERO;
	}
}

Sprite *GameObject::cargaSprite(Node *padre, int zOrder, const char *rutaImagen){
	// con PolygonSprites
	AutoPolygon ap1 = AutoPolygon(rutaImagen);
	//use all default values
	PolygonInfo myInfo = ap1.generateTriangles();
	sprite = Sprite::create(myInfo);

	if(padre){
		// TODO: ajusta la posicion del sprite para aparecer en el centro del padre, 
		// en vez de en la esquina inferior izquieda
		sprite->setPosition(padre->convertToNodeSpace(padre->getPosition()));
		padre->addChild(sprite, zOrder);
	}

	return sprite;
}

void GameObject::setPosition(Vec2 pos){
	if(sprite){
		sprite->setPosition(pos);
	} else{
		CCLOG("OJO! GameObject::setPosition(pos) sin sprite");
	}
}
