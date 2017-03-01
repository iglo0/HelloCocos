#include "Bala.h"

Bala::Bala(const char *pathSprite){

	setSprite(pathSprite);
	//setSonido(pathSonido);

	active = false;
}

Bala::Bala(const char *name, const char *pathSprite, int tipoColision, int colisionoCon){
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
		CCLOG("Tipo sonido bala conocido: %d", sb);
		rutaSonidoDisparo = ruta;
		break;
	case sonidosBala::impacto:
		CCLOG("Tipo sonido bala conocido: %d", sb);
		rutaSonidoImpacto = ruta;
		break;
	default:
		CCLOG("Tipo sonido bala desconocido: %d", sb);
		break;
	}
}

//void Bala::setSonido(const char *ruta){
//	rutaSonidoDisparo = ruta;
//	
//	//TODO: estoy haciendo preloads por todas partes y aun el sonido para el framerate :/
//	//CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(rutaSonidoDisparo);
//}

//const char *Bala::getSonido(){
//	return rutaSonidoDisparo;
//}

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
}

void Bala::reproduceSonido(sonidosBala sb){
	if(active){
		switch(sb){
		case sonidosBala::disparo:
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(rutaSonidoDisparo, false); 
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

void Bala::mueve(){
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

	Game::getInstance()->anadeFisica(sprite, tipoColision, colisionaCon, name);

	/* ENCAPSULADO
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

	return true;
}