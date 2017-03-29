#include "Player.h"

Player::Player(Node *nodo){
	CCLOG("Constructor de Player");
	
	inputComponent = nullptr;
	currentWeapon = nullptr;
	sprite = setSprite(nodo);

	disparar = false;

	// if Base::foo is not virtual then Derived::foo does not override Base::foo but hides it
	// C++ does not have a keyword for "the base class" (super or base) since it supports multiple inheritance which may lead to ambiguity.
	// Se puede hacer esto? ohhh...
	GameActor::gameActorSpeed = movementSpeed;
}

Player::~Player(){
}

Sprite *Player::setSprite(Node *nodo){
	// el jugador siempre será igual... o no?
	Vec2 pos;
	
	// con PolygonSprites
	AutoPolygon ap1 = AutoPolygon(pathSprite);
	PolygonInfo myInfo = ap1.generateTriangles();//use all default values
												 // sprite es autorelease
	sprite = Sprite::create(myInfo);

	if(!sprite){
		CCLOG("sprite nullptr en Player");
		return nullptr;
	}

	sprite->setScale(playerInitialScale);
	// lo situo en el centro
	pos.x = Director::getInstance()->getVisibleOrigin().x + Director::getInstance()->getVisibleSize().width / 2.0f;
	pos.y = sprite->getScale()*sprite->getContentSize().height / 2.0f;

	sprite->setPosition(pos);

	// añado física a este sprite
	//Game::getInstance()->anadeFisica(sprite, tipoColision, colisionaCon, "Jugador");
	Game::anadeFisica(sprite, (int)Game::CategoriaColision::Jugador, (int)Game::CategoriaColision::BalaEnemigo | (int)Game::CategoriaColision::Enemigo, "Jugador");

	// preparando el impacto
	sprite->setTag((int)Game::CategoriaColision::Jugador);
	sprite->setUserData(this);

	// hecho

	nodo->addChild(sprite, playerInitialZOrder);

	return sprite;
}

//void Player::mueve(bool izq, bool dch, bool arr, bool abj){
//	Vec2 pos = getPosition();
//	float deltaT = Director::getInstance()->getDeltaTime();
//
//	if(izq){
//		pos.x -= playerSpeed * deltaT;
//		if(pos.x < 0)
//			pos.x = 0;
//	}
//
//	if(dch){
//		pos.x += playerSpeed * deltaT;
//		if(pos.x > Director::getInstance()->getVisibleSize().width){
//			pos.x = Director::getInstance()->getVisibleSize().width;
//		}
//	}
//
//	if(arr){
//		pos.y += playerSpeed * deltaT;
//		if(pos.y > Director::getInstance()->getVisibleSize().height){
//			pos.y = Director::getInstance()->getVisibleSize().height;
//		}
//	}
//
//	if(abj){
//		pos.y -= playerSpeed * deltaT;
//		if(pos.y < 0){
//			pos.y = 0;
//		}
//	}
//
//	setPosition(pos);
//}

//void Player::setPosition(Vec2 pos){
//	if(sprite){
//		sprite->setPosition(pos);
//	}
//}

void Player::update(float deltaT){
	// TODO: Primero el de la clase base?
	GameActor::update(deltaT);

	//CCLOG("Player update @%f", Game::getInstance()->ellapsedTime);

	// Ese se ha encargado del movimiento, ahora al resto:
	if(disparar){
		disparar = false;
		currentWeapon->fire(getPosition());
	}
}
