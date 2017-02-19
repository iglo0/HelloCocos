#include "Bala.h"

#pragma region inits

Bala::Bala(std::string pathSprite, const char *pathSonido){

	visibleSize = Director::getInstance()->getVisibleSize();

	setSprite(pathSprite);
	setSonido(pathSonido);

	active = false;
}

Bala::~Bala(){
}

bool Bala::setSprite(std::string ruta){
	rutaSprite = ruta;
	sprite = Sprite::create(rutaSprite);

	if(!sprite){
		CCLOG("Bala->sprite '%s'=SIN DEFINIR", rutaSprite);
		return false;
	}

	return true;
}

std::string Bala::getRutaSprite(){
	return rutaSprite;
}

Sprite *Bala::getSprite(){
	return sprite;
}


void Bala::setSonido(const char *ruta){
	rutaSonido = ruta;
	
	// tiene sentido hacer un preload aqui? creo que si
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(rutaSonido);
}

const char *Bala::getSonido(){
	return rutaSonido;
}


#pragma endregion

bool Bala::isActive(){
	return active;
}

void Bala::activar(Vec2 posInicial, float vel){
	if(!sprite){
		CCLOG("Intento de activar una bala sin sprite: %s", rutaSprite);
		active = false;
		return;
	}

	active = true;

	sprite->setVisible(true);
	sprite->setPosition(posInicial);
	velocidadAbs = vel;
	suena();
}

void Bala::desActivar(){
	active = false;
	// o lo muevo fuera de la pantalla. Esto me parece más limpio.
	sprite->setVisible(false);
}

void Bala::suena(){
	if(active)
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(rutaSonido, false);
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
		if(pos.y > visibleSize.height){
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


