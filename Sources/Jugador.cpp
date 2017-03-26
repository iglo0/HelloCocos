#include "Jugador.h"

#pragma region inicializaciones
Jugador::Jugador(float hp) : puntosDeGolpeIniciales(hp), puntosDeGolpeActuales(hp){

	mueveAbj = mueveArr = mueveDch = mueveIzq = disparo = sale = false;
	// a tomar por %$*&# undefined pointer.
	//escudo = nullptr;

	// TODO: Siempre pasa por aqui?
	CCLOG("Creando jugador");
	gameInstance = Game::getInstance();
	//puntosDeGolpeActuales = puntosDeGolpeIniciales;

	// HACK: alguien habrá inicializado esto un par de escenas atrás. Antes que haya jugador al menos O:-)
	tIniDelay = gameInstance->ellapsedTime;
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
	return true;
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
	return true;
}

Sprite *Jugador::creaSpriteFisicas(Node *nodo, int tipoColision, int colisionaCon){
	Vec2 pos;

	if(!cargaSprite()){
		CCLOG("ERROR Cargando sprite jugador: %s", pathSprite);
		return nullptr;
	}

	// en llegando aquí hay sprite
	// le asigno su escala
	sprite->setScale(spriteScale);
	// lo situo en el centro
	pos.x = Director::getInstance()->getVisibleOrigin().x + Director::getInstance()->getVisibleSize().width / 2.0f;
	pos.y = sprite->getScale()*sprite->getContentSize().height / 2.0f;

	sprite->setPosition(pos);

	// añado física a este sprite
	//Game::getInstance()->anadeFisica(sprite, tipoColision, colisionaCon, "Jugador");
	Game::anadeFisica(sprite, tipoColision, colisionaCon, "Jugador");

	// preparando el impacto
	sprite->setTag((int)Game::CategoriaColision::Jugador);
	sprite->setUserData(this);


	// hecho

	nodo->addChild(sprite, zOrder);

	return sprite;
}

void Jugador::resetea(){
	// TODO: cosas a hacer tras morir
	// para empezar y de momento:
	puntosDeGolpeActuales = puntosDeGolpeIniciales;

	// TODO: restituir el Tinte
}

void Jugador::setPoolBalas(Pool *pool){
	//poolBalasActual = pool;
}

void Jugador::setPoolBalas(std::vector<Bala *> &pool) {
	//TODO: Esto pasa la referencia como yo quiero?
	poolBalasActual = pool;
}

#pragma endregion

#pragma region control del jugador

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

	// TODO: solo movimento horizontal
	if(arr){
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
	if(gameInstance->ellapsedTime - tIniDelay < delayDisparo){
		return;
	}

	// empiezo a contar hasta el proximo disparo
	tIniDelay = gameInstance->ellapsedTime;

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

//void Jugador::dispara(){
//	if(gameInstance->ellapsedTime - tIniDelay < delayDisparo){
//		return;
//	}
//
//	// empiezo a contar hasta el proximo disparo
//	tIniDelay = gameInstance->ellapsedTime;
//
//	// prepara una bala
//	Vec2 pos = sprite->getPosition();
//	pos.y += 20.0f;
//
//	// a la bala le asigno al padre del prota (la escena espero)
//	// no quiero que se mueva junto con la nave
//	poolBalasActual->activa(pos, sprite->getParent(), 1);
//
//}

#pragma endregion

#pragma region eventos

void Jugador::update(float deltaT){
	//mueve(mueveIzq, mueveDch, mueveArr, mueveAbj);
	// limito el movimiento izquierda y derecha
	mueve(mueveIzq, mueveDch, mueveArr, mueveAbj);

	if(disparo){
		// dispara() se encarga de poner un delay entre disparos
		//dispara();
		
		CCLOG("bang!");
	}

}

bool Jugador::impacto(float dmg){
	// Hay escudo?
	// pseudocódigo
	// caso de haber escudo, calcula cuanta vida absorbe
	//float dmgRestante = impactoEscudo(dmg);

	//if(escudo){
	//	// el escudo absorbe parte del daño y devuelve lo que queda
	//	dmg = escudo->impacto(dmg);
	//}

	// si el daño ha quedado en el escudo, vuelve sin efectos adicionales
	if(dmg <= 0){
		return false;
	}

	// quitar vida
	puntosDeGolpeActuales -= dmg;

	if(puntosDeGolpeActuales <= 0){
		// espero que estés cacheado
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(pathSonidoMuerte);
		// a ver que pasa
		return true;
	} else{
		// efecto de daño
		float porcen = puntosDeGolpeActuales / puntosDeGolpeIniciales;

		// Actions
		// http://www.cocos2d-x.org/wiki/Actions
		// By -> relative, To -> absoulte

		// Tints a node to the specified RGB values
		// GLubyte = unsigned char
		GLubyte val = 255 * porcen;
		if(val < 128)
			val = 128;
		auto tintTo = TintTo::create(0.5f, val, val, val);
		sprite->runAction(tintTo);

		// Sequences

		// create a few actions.
		auto tintIda = TintTo::create(0.05f, Color3B(255, 0, 0));
		auto tintVuelta = TintTo::create(0.05f, Color3B(255, 255, 255));

		float escalaOri = sprite->getScale();
		auto escalaIda = ScaleTo::create(0.05f, escalaOri * 1.1);
		auto escalaVuelta = ScaleTo::create(0.05f, escalaOri);

		auto callbackTint = CallFunc::create([](){
			log("Tintorro!");
		});

		auto callbackScale = CallFunc::create([](){
			log ("Scaled");
		});

		// create a sequence with the actions and callbacks
		// TODO: mirar usos de los callbacks
		auto seq = Sequence::create(escalaIda, callbackScale, tintIda, tintVuelta, escalaVuelta, callbackScale, nullptr);

		// run it
		sprite->runAction(seq);


		return false;
	}


}

#pragma endregion

#pragma region auxiliares
Vec2 Jugador::getPosition(){
	return sprite->getPosition();
}

Sprite *Jugador::getSprite(){
	return sprite;
}


#pragma endregion

