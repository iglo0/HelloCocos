#include "Bala.h"

Bala::Bala(const char *pathSprite){

	setSprite(pathSprite);
	//setSonido(pathSonido);

	active = false;
}

Bala::Bala(const char *name, const char *pathSprite, int tipoColision, int colisionoCon, float dmg){
	danyoBala = dmg;

	if(!setSpriteConFisica(name, pathSprite, tipoColision, colisionoCon)){
		CCLOG("Bala->sprite '%s'=SIN DEFINIR", pathSprite);
		return;
	}


	active = false;
}

Bala::~Bala(){
}

bool Bala::setSprite(const char *ruta){
	sprite = Sprite::create(ruta);

	if(!sprite){
		return false;
	}
	return true;
}

Sprite *Bala::getSprite(){
	return sprite;
}

void Bala::setSonido(sonidosBala sb, const char *ruta){
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

bool Bala::isActive(){
	return active;
}

void Bala::setVelocidad(float vel){
	velocidadAbs = vel;
}

void Bala::activar(Vec2 posInicial){
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

void Bala::desActivar(){
	active = false;

	reproduceSonido(sonidosBala::impacto);

	// o lo muevo fuera de la pantalla. Esto me parece más limpio.
	sprite->setVisible(false);

	if(sprite->getPhysicsBody()){
		sprite->getPhysicsBody()->setEnabled(false);
	}

}

void Bala::reproduceSonido(sonidosBala sb){
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

void Bala::update(){
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
bool Bala::setSpriteConFisica(const char *name, const char *ruta, int tipoColision, int colisionaCon){

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

float Bala::getDanyoBala() {
	return danyoBala;
}

//void Bala::creaPool(int cant, int tipoColision, int colisionaCon, const char *nombreId, const char *rutaSprite){
//	Sprite *tmp;
//
//	setSpriteConFisica(nombreId, rutaSprite, tipoColision, colisionaCon);
//
//}