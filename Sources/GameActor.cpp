#include "GameActor.h"

GameActor::GameActor(){
	//CCLOG("Constructor de GameActor");
	// me aseguro de que está inicializado
	sprite = nullptr;

	// todos los "actores" tienen unas características comunes: movimiento, vida...
	gameActorHP = gameActorHPInicial;
	mueveIzq = mueveDch = mueveArr = mueveAbj = false;
}

GameActor::~GameActor(){
	//CCLOG("Destructor de GameActor");
}

void GameActor::update(float deltaT){
	//CCLOG("GameActor update @%f", Game::getInstance()->ellapsedTime);

	// TODO: hum, por ejemplo una bala entra por gameactor::update y luego quiero que vaya a bullet::mueve?
	// más bien que sobrescriba el método update la bala también?
	// JODER. Ha funcionado? Si paso una bala que tiene mueve sobrescrito, lo entiende y ejecuta el mueve correcto? Joder. Mola!!!
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

Sprite *GameActor::setSprite(Node *nodo, const char *ruta, const char *name, int tipoColision, int colisionaCon, bool isPolySprite){
	if(isPolySprite){
		AutoPolygon ap1 = AutoPolygon(ruta);
		sprite = Sprite::create(ap1.generateTriangles());
	} else{
		sprite = Sprite::create(ruta);
	}


	if(!sprite){
		CCLOG("GameActor::setSprite '%s'=SIN DEFINIR", ruta);
		return nullptr;// o return sprite pal caso
	}

	sprite->setName(name);

	Game::anadeFisica(sprite, tipoColision, colisionaCon, name);

	//TODO: ya tengo para recuperar mis datos :)
	sprite->setUserData(this);
	// y su tipo
	sprite->setTag(tipoColision);

	// lo creo invisible y sin colisiones activas
	desactiva();

	// hecho
	nodo->addChild(sprite);

	return sprite;
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

			// HACK: lo siento, lo quitaré vale?
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
	// si he llegado aquí, que no hay ni sprite...
	return false;
}


void GameActor::impacto(float dmg){
	const char *name = sprite->getName().c_str();

	CCLOG ("GameActor %s says: ouch %f", name, dmg);
}