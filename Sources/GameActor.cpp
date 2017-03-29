#include "GameActor.h"

GameActor::GameActor(){
	CCLOG("Constructor de GameActor");
	// me aseguro de que está inicializado
	sprite = nullptr;

	// todos los "actores" tienen unas características comunes: movimiento, vida...
	gameActorHP = gameActorHPInicial;
	mueveIzq = mueveDch = mueveArr = mueveAbj = false;
}

GameActor::~GameActor(){
	CCLOG("Destructor de GameActor");
}

void GameActor::update(float deltaT){
	//CCLOG("GameActor update @%f", Game::getInstance()->ellapsedTime);

	// TODO: Esto está muy bien... pero quiero saber si es una bala un enemigo o que >.<
	if(isActive()){
		mueve();
	}

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
		pos.x -= gameActorSpeed * deltaT;
		if(pos.x < 0)
			pos.x = 0;
	}

	if(mueveDch){
		pos.x += gameActorSpeed * deltaT;
		if(pos.x > Director::getInstance()->getVisibleSize().width){
			pos.x = Director::getInstance()->getVisibleSize().width;
		}
	}

	if(mueveArr){
		pos.y += gameActorSpeed * deltaT;
		if(pos.y > Director::getInstance()->getVisibleSize().height){
			pos.y = Director::getInstance()->getVisibleSize().height;

			// HACK: ?( ? )? (lo siento, lo quitaré vale?)
			if(sprite->getTag() == (int)Game::CategoriaColision::Bala){
				desactiva();
			}
		}
	}

	if(mueveAbj){
		pos.y -= gameActorSpeed * deltaT;
		if(pos.y < 0){
			pos.y = 0;
		}
	}

	setPosition(pos);

}

void GameActor::activa(Vec2 pos){
	if(sprite){
		sprite->setPosition(pos);

		sprite->setVisible(true);
		// TODO: Activar físicas

		PhysicsBody *pb = sprite->getPhysicsBody();
		if(pb){
			pb->setEnabled(true);
		}
	}
}

void GameActor::desactiva(){
	if(sprite){
		sprite->setVisible(false);

		PhysicsBody *pb = sprite->getPhysicsBody();
		if(pb){
			pb->setEnabled(false);
		}

	}
}

bool GameActor::isActive(){
	if(sprite){
		// HACK: por ejemplo
		return sprite->isVisible();
	}
}
