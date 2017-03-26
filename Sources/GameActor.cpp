#include "GameActor.h"

GameActor::GameActor(){
	CCLOG("Constructor de GameActor");
	// me aseguro de que está inicializado
	sprite = nullptr;

	// todos los "actores" tienen unas características comunes: movimiento, vida...
	hp = hpInicial;
	mueveIzq = mueveDch = mueveArr = mueveAbj = false;
}

GameActor::~GameActor(){
	CCLOG("Destructor de GameActor");
}

void GameActor::update(float deltaT){
	CCLOG("GameActor update @%f", Game::getInstance()->ellapsedTime);

	mueve();

}

Vec2 GameActor::getPosition(){
	if(sprite){
		return sprite->getPosition();
	}
	// TODO: Ojo error
	return Vec2();
}

void GameActor::setPosition(Vec2 pos){
	if(sprite){
		sprite->setPosition(pos);
	} else{
		CCLOG("GameActor::setPosition -> no sprite");
	}
}

Sprite *GameActor::setSprite(){
	//sprite = nullptr;
	return nullptr;
}

Sprite *GameActor::getSprite(){
	return sprite;
}

void GameActor::mueve(){

	Vec2 pos = getPosition();
	float deltaT = Director::getInstance()->getDeltaTime();

	if(mueveIzq){
		pos.x -= movementSpeed * deltaT;
		if(pos.x < 0)
			pos.x = 0;
	}

	if(mueveDch){
		pos.x += movementSpeed * deltaT;
		if(pos.x > Director::getInstance()->getVisibleSize().width){
			pos.x = Director::getInstance()->getVisibleSize().width;
		}
	}

	if(mueveArr){
		pos.y += movementSpeed * deltaT;
		if(pos.y > Director::getInstance()->getVisibleSize().height){
			pos.y = Director::getInstance()->getVisibleSize().height;
		}
	}

	if(mueveAbj){
		pos.y -= movementSpeed * deltaT;
		if(pos.y < 0){
			pos.y = 0;
		}
	}

	setPosition(pos);

}