#include "Enemigo.h"

Enemigo::Enemigo(){

}

Enemigo::~Enemigo(){

}

Sprite *Enemigo::creaSprite(Node *nodo, const char *pathSprite, const char *rutaSonidoMuerte, float scale, int z){
	pathSprite = pathSprite;
	/*	
	AutoPolygon ap1 = AutoPolygon(path1);
	PolygonInfo myInfo = ap1.generateTriangles();//use all default values
	sprite1 = Sprite::create(myInfo);
	*/

	AutoPolygon ap = AutoPolygon(pathSprite);
	PolygonInfo polInfo = ap.generateTriangles(); // use default values
	sprite = Sprite::create(polInfo);

	pathSonidoMuerte = rutaSonidoMuerte;

	//sprite = Sprite::create(pathSprite);

	if(!sprite){
		return nullptr;
	}

	spriteScale = scale;
	sprite->setScale(spriteScale);
	//HACK: ya tengo para recuperar mis datos :)
	sprite->setUserData(this);
	// y su tipo
	sprite->setTag((int)Game::CategoriaColision::Enemigo);
	zOrder = z;
	

	// y por ultimo le asigno la colision
	Game::getInstance()->anadeFisica(sprite, (int)Game::CategoriaColision::Enemigo, (int)Game::CategoriaColision::Bala | (int)Game::CategoriaColision::Jugador, "Enemigo");

	nodo->addChild(sprite, zOrder);

	return sprite;
}

float Enemigo::getScale(){
	return spriteScale;
}

void Enemigo::setPosition(float x, float y){
	setPosition(Vec2(x, y));
}

void Enemigo::setPosition(Vec2 nuPos){
	sprite->setPosition(nuPos);
}

void Enemigo::mueveRelativo(Vec2 cant){
	// ojo que venga multiplicado por deltaT

	setPosition(getPosition() + cant);
}

Sprite *Enemigo::getSprite(){
	return sprite;
}

Vec2 Enemigo::getPosition(){
	return sprite->getPosition();
}

void Enemigo::impacto(){
	// un algo impacta en el enemigo

	// pierde escudo

	// pierde vida

	// o muere
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(pathSonidoMuerte);

}

void Enemigo::desActivar(){
	// bueno, si lo desactivo no puedo obtener su ultima posicion (por ejemplo lo uso desde la horda)
	// y al hacerlo invisible y sin fisica ergo colisiones, digo yo que no hace falta moverlo a (cualquiersitio,dondesea)
	//sprite->setPosition(Vec2::ZERO);
	sprite->setVisible(false);
	sprite->getPhysicsBody()->setEnabled(false);

}

void Enemigo::activar(Vec2 posicion){
	sprite->setPosition(posicion);
	sprite->setVisible(true);
	sprite->getPhysicsBody()->setEnabled(true);
}

bool Enemigo::estaActivo(){
	// TODO: es una manera
	return sprite->isVisible();
}

/*
bool Jugador::cargaSprite(){
	sprite = Sprite::create(pathSprite);
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
*/