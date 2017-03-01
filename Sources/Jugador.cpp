#include "Jugador.h"

Jugador::Jugador(){

}

Jugador::~Jugador(){

}

bool Jugador::cargaSprite(){

	// con PolygonSprites
	AutoPolygon ap1 = AutoPolygon(pathSprite);
	PolygonInfo myInfo = ap1.generateTriangles();//use all default values
	sprite = Sprite::create(myInfo);

	//sprite = Sprite::create(pathSprite);
	if(sprite)
		return true;
	return false;
}

bool Jugador::creaSprite(Node *nodo){
	Vec2 pos;

	if(!cargaSprite()){
		return false;
	}

	// en llegando aquí hay sprite
	// le asigno su escala
	sprite->setScale(spriteScale);
	// lo situo en el centro
	pos.x = Director::getInstance()->getVisibleOrigin().x + Director::getInstance()->getVisibleSize().width / 2.0f;
	pos.y = sprite->getScale()*sprite->getContentSize().height / 2.0f;

	sprite->setPosition(pos);
	
	//sprite->getBoundingBox();

	nodo->addChild(sprite, zOrder);
}

bool Jugador::creaSprite(Node *nodo, Vec2 posIni){
	// intenta crear un sprite si no se ha hecho antes
	if(!cargaSprite()){
		return false;
	} 

	// llegando aquí tengo un sprite perfectamente cargado por alguien
	// falta: ajustar tamaño y posición
	sprite->setScale(spriteScale);
	sprite->setPosition(posIni);
	nodo->addChild(sprite, zOrder);
}

bool Jugador::creaSpriteFisicas(Node *nodo, int tipoColision, int colisionaCon){
	Vec2 pos;

	if(!cargaSprite()){
		CCLOG("ERROR Cargando sprite jugador: %s", pathSprite);
		return false;
	}

	// en llegando aquí hay sprite
	// le asigno su escala
	sprite->setScale(spriteScale);
	// lo situo en el centro
	pos.x = Director::getInstance()->getVisibleOrigin().x + Director::getInstance()->getVisibleSize().width / 2.0f;
	pos.y = sprite->getScale()*sprite->getContentSize().height / 2.0f;

	sprite->setPosition(pos);

	// añado física a este sprite
	Game::getInstance()->anadeFisica(sprite, tipoColision, colisionaCon, "Jugador");

	/*
	// fisica y colisiones
	PhysicsBody *fisicaSprite;
	// TODO: PhysicsMaterial OJO
	// Density=0.1f, Restitution=1.0f, Friction=0
	// No tengo ni idea, copiado de un ejemplo sencillo para solo colisiones y que funciona
	fisicaSprite = PhysicsBody::createBox(Size(sprite->getContentSize().width, sprite->getContentSize().height), PhysicsMaterial(0.1f, 1.0f, 0.0f));

	// set the category, collision and contact test bit masks
	// tipo del objeto
	fisicaSprite->setCategoryBitmask(tipoColision);
	// qué tipos de objeto generan eventos de colisión con este?
	fisicaSprite->setContactTestBitmask(colisionaCon);

	// estableciendo la física como dynamic, el motor no aplicará fuerzas a este objeto
	// Lo que significa que es controlado por el programador (como iskinetic en Unity)
	fisicaSprite->setDynamic(true);

	// que objetos deberian afectar a este en las colisiones
	// (no afectaría por que es dinamico, está puesto por completar)
	fisicaSprite->setCollisionBitmask((int)Game::CategoriaColision::None);

	sprite->setPhysicsBody(fisicaSprite);
	*/
	// hecho


	nodo->addChild(sprite, zOrder);

}

void Jugador::mueve(bool izq, bool dch, bool arr, bool abj){
	auto deltaT = Director::getInstance()->getDeltaTime();
	auto visibleSize = Director::getInstance()->getVisibleSize();

	Vec2 position = sprite->getPosition();

	// movimiento de la nave
	if(izq){
		// hace el movimiento independiente de framerates
		position.x -= playerSpeed * deltaT;

		// comprueba los márgenes
		if(position.x < 0){
			position.x = 0;
		} 
	}
	if(dch){
		position.x += playerSpeed * deltaT;
		if(position.x > visibleSize.width){
			position.x = visibleSize.width;
		}
	}


	if(arr) { 
		position.y += playerSpeed * deltaT;
		if(position.y > visibleSize.height){
			position.y = visibleSize.height;
		} 
	}

	if(abj){
		position.y -= playerSpeed * deltaT;
		if(position.y < 0.0f){
			position.y = 0.0f;
		}
	}

	sprite->setPosition(position);
}

void Jugador::dispara(std::vector<Bala *> &pool){
	// le paso un pool 
	for(auto b = pool.cbegin(); b != pool.cend(); ++b){
		if(!(*b)->isActive()){
			// uso esta bala
			
			Vec2 pos = sprite->getPosition();
			pos.y += 20.0f;

			(*b)->activar(pos);
			break;
		}
	}
}
