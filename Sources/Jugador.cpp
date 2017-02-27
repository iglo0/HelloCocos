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

	// en llegando aqu� hay sprite
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

	// llegando aqu� tengo un sprite perfectamente cargado por alguien
	// falta: ajustar tama�o y posici�n
	sprite->setScale(spriteScale);
	sprite->setPosition(posIni);
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

		// comprueba los m�rgenes
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
