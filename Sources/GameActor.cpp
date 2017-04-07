#include "GameActor.h"

GameActor::GameActor(){
	//CCLOG("Constructor de GameActor");
	// me aseguro de que está inicializado
	sprite = nullptr;

	// todos los "actores" tienen unas características comunes: movimiento, vida...
	gameActorHP = gameActorHPInicial;
	mueveIzq = mueveDch = mueveArr = mueveAbj = false;
	funcionMovimientoActual = nullptr;
}

GameActor::~GameActor(){
	//CCLOG("Destructor de GameActor");
}

// TODO: no quiero que todos los GameActor se comporten igual. ¿Cómo hago que un enemigo se controle de la forma 1 o 2 o 3?
////void GameActor::update(float deltaT, GameActor *instancia, void(GameActor::*funcionMovimiento)(Vec2, double), Vec2 posIni, double amplitude){
//void GameActor::update(float deltaT, GameActor *instancia, punteroAFuncionMovimiento funMov, Vec2 posIni, double amplitude){
void GameActor::update(float deltaT){
		//CCLOG("GameActor update @%f", Game::getInstance()->ellapsedTime);

	// TODO: hum, por ejemplo una bala entra por gameactor::update y luego quiero que vaya a bullet::mueve?
	// más bien que sobrescriba el método update la bala también?
	// JODER. Ha funcionado? Si paso una bala que tiene mueve sobrescrito, lo entiende y ejecuta el mueve correcto? Joder. Mola!!!
	if(isActive()){
		// Me tengo que mirar detenidamente pasar funciones como parametro
		if(this->funcionMovimientoActual){
			// TODO: si proporciono una funcion de movimiento, usa esta
			////(instancia->*funMov)(posIni, amplitude);
			//(this->*funMov)(funcionMovimientoPosIni, funcionMovimientoAmplitude);
			(this->*funcionMovimientoActual)(funcionMovimientoPosIni, funcionMovimientoAmplitude);
		} else{
			// TODO: por aquí pasa el player. No sé si me convence cómo lo tengo montado, aún...
			mueve();
		}
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

Sprite *GameActor::setSprite(Node *nodo, const char *ruta, const char *name, int tipoColision, int colisionaCon, float initialScale, bool createPolySprite){
	if(createPolySprite){
		// AutoPolygon viene bien para ahorrar proceso a la gpu (a estas alturas cuesta menos mover unos cuantos vértices que mirar si un pixel es transparente o no, particularmente en móviles)
		// PERO no sirve para colisiones como creía, las formas generadas son más complejas que lo que el motor de física puede manejar con facilidad
		AutoPolygon ap1 = AutoPolygon(ruta);
		sprite = Sprite::create(ap1.generateTriangles());
	} else{
		sprite = Sprite::create(ruta);
	}


	if(!sprite){
		CCLOG("GameActor::setSprite '%s'=SIN DEFINIR", ruta);
		return nullptr;// o return sprite pal caso
	}

	sprite->setScale(initialScale);

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

// TODO: TEST!!!!
void GameActor::mueveSeno(Vec2 posIni, double amplitude){
	Vec2 pos = getPosition();

	//float deltaT = Director::getInstance()->getDeltaTime();
	float ellapsed = Game::getInstance()->ellapsedTime;

	//pos.x = posIni.x + amplitude * Director::getInstance()->getDeltaTime() * sin(Game::getInstance()->ellapsedTime);
	// TODO: Esto es indepenidente de framerate? -> si porque ya depende de ellapsed time así que si meto el deltaT solo estoy enguarrando todo
	pos.x = posIni.x + amplitude * sin(ellapsed / 2.0);

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