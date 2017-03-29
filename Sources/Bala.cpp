#include "Bala.h"




#pragma region OOOOLD

BalaOLD::BalaOLD(const char *pathSprite){

	setSprite(pathSprite);
	//setSonido(pathSonido);

	active = false;
}

BalaOLD::BalaOLD(const char *name, const char *pathSprite, int tipoColision, int colisionoCon, float dmg){
	danyoBala = dmg;

	if(!setSpriteConFisica(name, pathSprite, tipoColision, colisionoCon)){
		CCLOG("Bala->sprite '%s'=SIN DEFINIR", pathSprite);
		return;
	}


	active = false;
}

BalaOLD::~BalaOLD(){
}

bool BalaOLD::setSprite(const char *ruta){
	sprite = Sprite::create(ruta);

	if(!sprite){
		return false;
	}
	return true;
}

Sprite *BalaOLD::getSprite(){
	return sprite;
}

void BalaOLD::setSonido(sonidosBala sb, const char *ruta){
	switch(sb){
	case sonidosBala::disparo:
		rutaSonidoDisparo = ruta;
		break;
	case sonidosBala::impacto:
		rutaSonidoImpacto = ruta;
		break;
	default:
		CCLOG("Tipo sonido bala desconocido: %d", sb);
		break;
	}
}

bool BalaOLD::isActive(){
	return active;
}

void BalaOLD::setVelocidad(float vel){
	velocidadAbs = vel;
}

void BalaOLD::activar(Vec2 posInicial){
	if(!sprite){
		CCLOG("Intento de activar una bala sin sprite");
		active = false;
		return;
	}

	active = true;
	
	if(sprite->getPhysicsBody()){
		sprite->getPhysicsBody()->setEnabled(true);
	}

	sprite->setVisible(true);
	sprite->setPosition(posInicial);
	//velocidadAbs = vel;
	reproduceSonido(sonidosBala::disparo);
}

void BalaOLD::desActivar(){
	active = false;

	reproduceSonido(sonidosBala::impacto);

	// o lo muevo fuera de la pantalla. Esto me parece más limpio.
	sprite->setVisible(false);

	if(sprite->getPhysicsBody()){
		sprite->getPhysicsBody()->setEnabled(false);
	}

}

void BalaOLD::reproduceSonido(sonidosBala sb){
	if(active){
		switch(sb){
		case sonidosBala::disparo:
			// audioengine si parece que soporta efectos pero no encuentro ejemplos de uso
			//AudioEngine::play2d("mi carro me lo robaron");
			cocos2d::experimental::AudioProfile *audioProfile;
			cocos2d::experimental::AudioEngine::play2d(rutaSonidoDisparo, false, 1.0f, nullptr);
		
			// TODO: no pitch/pan/gain on win32. Doh!
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(rutaSonidoDisparo, false, 1.0f,0.0f,1.0f);
			break;
		case sonidosBala::impacto:
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(rutaSonidoImpacto, false); 
			break;
		default:
			CCLOG("Tipo de sonidoBala desconocido %s", sb);
			break;
		}
	}
}

void BalaOLD::update(){
	if(!active){
		return;
	}
	
	Vec2 pos = sprite->getPosition();

	// mueve según deltaTime
	// TODO: mejorable?
	pos.y += velocidadAbs * Director::getInstance()->getDeltaTime();

	// super ñapa
	if(velocidadAbs > 0){
		// va hacia arriba
		if(pos.y > Director::getInstance()->getVisibleSize().height){
			// hay que destruir la bala
			desActivar();
		}
	} else{
		// va hacia abajo
		if(pos.y < 0){
			// hay que destruir la bala
			desActivar();
		}

	}
	sprite->setPosition(pos);

}

// colisiones / física
bool BalaOLD::setSpriteConFisica(const char *name, const char *ruta, int tipoColision, int colisionaCon){

	// para sprites poligonales	
	AutoPolygon ap1 = AutoPolygon(ruta);
	sprite = Sprite::create(ap1.generateTriangles());

	// para sprites-caja
	//sprite = Sprite::create(ruta);

	if(!sprite){
		CCLOG("Bala->sprite '%s'=SIN DEFINIR", ruta);
		return false;
	}

	sprite->setName(name);

	//Game::getInstance()->anadeFisica(sprite, tipoColision, colisionaCon, name);
	Game::anadeFisica(sprite, tipoColision, colisionaCon, name);

	// la detección de colisiones empieza desactivada
	sprite->getPhysicsBody()->setEnabled(false);

	//TODO: ya tengo para recuperar mis datos :)
	sprite->setUserData(this);
	// y su tipo
	sprite->setTag((int)Game::CategoriaColision::Bala);

	// hecho

	return true;
}

float BalaOLD::getDanyoBala() {
	return danyoBala;
}


#pragma endregion

Bullet::Bullet(Node *nodo, const char *name, const char *pathSprite, const char *pathSonidoDisparo, const char *pathSonidoImpacto, float speed, float dmg, int tipoColision, int colisionoCon){
	// inicializa la clase base primero
	GameActor::GameActor();
	//bulletSpeed = speed;
	bulletDmg = dmg;
	gameActorSpeed = speed;
	
	CCLOG("creando bala: %s", name);
	if(!createBullet(nodo, pathSprite, name, tipoColision, colisionoCon)){
		CCLOG("No pude crear bala %s", pathSprite);
	}
}

Bullet::~Bullet(){
}

bool Bullet::createBullet(Node *nodo, const char *ruta, const char *name, int tipoColision, int colisionaCon){

	// para sprites poligonales	
	//AutoPolygon ap1 = AutoPolygon(ruta);
	//sprite = Sprite::create(ap1.generateTriangles());

	// para sprites-caja
	sprite = Sprite::create(ruta);

	if(!sprite){
		CCLOG("Bala->sprite '%s'=SIN DEFINIR", ruta);
		return false;
	}

	sprite->setName(name);

	Game::anadeFisica(sprite, tipoColision, colisionaCon, name);
	
	//TODO: ya tengo para recuperar mis datos :)
	sprite->setUserData(this);
	// y su tipo
	sprite->setTag((int)Game::CategoriaColision::Bala);

	// la bala la creo invisible y sin colisiones activas
	desactiva();

	// HACK: darle movimiento
	mueveArr = true;

	// hecho
	nodo->addChild(sprite);

	return true;
}

// (OJO) static method
void Bullet::createBulletPool(Node *nodo, std::vector<Bullet *> &pool, int poolSize, const char *name, const char *pathSprite, const char *pathSonidoDisparo, const char *pathSonidoImpacto, float speed, float dmg, int tipoColision, int colisionoCon){
	Bullet *tmp;

	for(int i = 0; i < poolSize; i++){
		tmp = new Bullet(nodo, (name + std::to_string(i)).c_str(), pathSprite, pathSonidoDisparo, pathSonidoImpacto, speed, dmg, tipoColision, colisionoCon);
		
		// TODO: que mas cosas hacer a la bala?

		pool.push_back(tmp);
	}
	
	
}
