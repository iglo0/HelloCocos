#include "Enemigo.h"

Enemy::Enemy(Node *nodo, const char *pathSprite, const char *rutaSonidoMuerte, float initialScale, float initialRotation, float hp){
	GameActor::GameActor();

	setSprite(nodo, pathSprite, "Enemigo", (int)Game::CategoriaColision::Enemigo, (int)Game::CategoriaColision::Jugador | (int)Game::CategoriaColision::Bala,initialScale,true);
	
	GameActor::gameActorHP = hp;
	GameActor::gameActorSpeed = ENEMY_GENERIC_SPEED;

	tIniDisparo = 0;
	//enemigoDisparando = false;
	funcionControlActual = nullptr;
	//funcionMovimientoActual = nullptr;// esto lo hace el constructor de GameActor
	funcionControlTiempoDisparo = 1.0f;
}

Enemy::~Enemy(){
	CCLOG ("Enemy destructor");
}

void Enemy::impacto(float dmg){
	// TODO: Escudo?

	// recibe daño
	gameActorHP -= dmg;

	if(gameActorHP <= 0){
		// muere
		desactiva();
		// TODO: explosion
		// TODO: dar puntos
	} else{
		// no muere

		// la bala (si es lo que le ha golpeado) ya saca un grafico de impacto
	}


}

void Enemy::funControl1(float segundos){
	float ellapsedTime = Game::getInstance()->ellapsedTime;

	if(ellapsedTime - tIniDisparo > segundos){
		Pool::activa(weapon->bulletPool, getPosition());

		tIniDisparo = ellapsedTime;
	}

}

////void Enemy::update(float deltaT, GameActor *instancia, void(GameActor::*funcionMovimiento)(Vec2, double), Vec2 posIni, double amplitude){
//void Enemy::update(float deltaT, GameActor *instancia, GameActor::punteroAFuncionMovimiento funMov, Vec2 posIni, double amplitude){
//	//CCLOG("GameActor update @%f", Game::getInstance()->ellapsedTime);
//
//	// TODO: hum, por ejemplo una bala entra por gameactor::update y luego quiero que vaya a bullet::mueve?
//	// más bien que sobrescriba el método update la bala también?
//	// JODER. Ha funcionado? Si paso una bala que tiene mueve sobrescrito, lo entiende y ejecuta el mueve correcto? Joder. Mola!!!
//	if(isActive()){
//		// Me tengo que mirar detenidamente pasar funciones como parametro
//		//if(funMov){
//		if(this->funcionMovimientoActual){
//			// TODO: si proporciono una funcion de movimiento, usa esta
//			////(instancia->*funMov)(posIni, amplitude);
//			//(this->*funMov)(posIni, amplitude);
//			// "simplemente" invoco a la funcion de movimiento que tendrá que tener definida en la creación
//			(this->*funcionMovimientoActual)(this->funcionMovimientoPosIni, this->funcionMovimientoAmplitude);
//
//			// HACK: Añado una funcion de control de disparo. OOOH funciona!!
//			// mismo para la función que le controla
//			funcionControlEnemigo fce = &Enemy::funControl1;
//			(this->*fce)(weapon->bulletPool, 1.5f);
//
//		} else{
//			mueve();
//		}
//	}
//}

void Enemy::update(float deltaT){
	//CCLOG("GameActor update @%f", Game::getInstance()->ellapsedTime);

	if(isActive()){
		// TODO: Me tengo que mirar detenidamente pasar funciones como parametro
		// si proporciono una funcion de movimiento, usa esta
		if(this->funcionMovimientoActual){
			(this->*funcionMovimientoActual)(this->funcionMovimientoPosIni, this->funcionMovimientoAmplitude);


		} else{
			mueve();
		}

		if(this->funcionControlActual){
			// HACK: Añado una funcion de control de disparo. OOOH funciona!!
			// mismo para la función que le controla
			//funcionControlEnemigo fce = &Enemy::funControl1;
			//(this->*fce)(weapon->bulletPool, 1.5f);
			(this->*funcionControlActual)(funcionControlTiempoDisparo);
		}
	}
}

#pragma region Old

EnemigoOOOLD::EnemigoOOOLD(){

}

EnemigoOOOLD::~EnemigoOOOLD(){
	CCLOG("Memoria del Enemigo destruida?");

	// los cocos2d::sprites son autoliberados
	//if(sprite){
	//}
	// TODO: en realidad no hago ningún new en la clase así que tampoco tocan deletes?

	// retirar el sprite del nodo lo único, pero de cual? Cómo saberlo sin parámetros? Tendría que capturarlo en una variable miembro?

}

Sprite *EnemigoOOOLD::creaSprite(Node *nodo, const char *pathSprite, const char *rutaSonidoMuerte, float scale, int z, float hp){
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
	
	puntosDeGolpeIniciales = hp;
	puntosDeGolpeActuales = puntosDeGolpeIniciales;

	// y por ultimo le asigno la colision
	//Game::getInstance()->anadeFisica(sprite, (int)Game::CategoriaColision::Enemigo, (int)Game::CategoriaColision::Bala | (int)Game::CategoriaColision::Jugador, "Enemigo");
	Game::anadeFisica(sprite, (int)Game::CategoriaColision::Enemigo, (int)Game::CategoriaColision::Bala | (int)Game::CategoriaColision::Jugador, "Enemigo");

	nodo->addChild(sprite, zOrder);

	return sprite;
}

float EnemigoOOOLD::getScale(){
	return spriteScale;
}

void EnemigoOOOLD::setPosition(float x, float y){
	setPosition(Vec2(x, y));
}

void EnemigoOOOLD::setPosition(Vec2 nuPos){
	sprite->setPosition(nuPos);
}

void EnemigoOOOLD::mueveRelativo(Vec2 cant){
	// ojo que venga multiplicado por deltaT

	setPosition(getPosition() + cant);
}

Sprite *EnemigoOOOLD::getSprite(){
	return sprite;
}

Vec2 EnemigoOOOLD::getPosition(){
	return sprite->getPosition();
}

void EnemigoOOOLD::impacto(float dmg){
	// un algo impacta en el enemigo

	// pierde escudo

	// pierde vida
	puntosDeGolpeActuales -= dmg;
	
	// o muere
	if(puntosDeGolpeActuales <= 0){
		// TODO: estaría guay hacer algún efecto tipo que el enemigo se caiga o algo

		desActivar();

	} else{
		// efecto de daño
		float porcen = puntosDeGolpeActuales / puntosDeGolpeIniciales;

		// Actions
		// http://www.cocos2d-x.org/wiki/Actions
		// By -> relative, To -> absoulte

		// Scale uniformly by 3x over 2 seconds
		//auto scaleTo = ScaleTo::create(0.1f, sprite->getScale()*1.1f);
		//sprite->runAction(scaleTo);

		// Tints a node to the specified RGB values
		// GLubyte = unsigned char
		GLubyte val = 255 * porcen;
		if(val < 128)
			val = 128;
		auto tintTo = TintTo::create(0.5f, val, val, val);
		sprite->runAction(tintTo);

		// Tints a node BY the delta of the specified RGB values.
		//auto tintBy = TintBy::create(0.1f, 32, 0, 0);
		//sprite->runAction(tintBy);

		// Sequences

		// create a few actions.
		auto jump = JumpBy::create(0.5, Vec2(0, 0), 10, 1);

		//auto rotate = RotateTo::create(2.0f, 10);

		auto tintIda = TintTo::create(0.05f, Color3B(255, 0, 0));
		auto tintVuelta = TintTo::create(0.05f, Color3B(255, 255, 255));

		float escalaOri = sprite->getScale();

		auto escalaIda = ScaleTo::create(0.05f, escalaOri * 1.1);
		auto escalaVuelta = ScaleTo::create(0.05f, escalaOri);

		// create a few callbacks
		auto callbackJump = CallFunc::create([](){
			log("Jumped!");
		});

		auto callbackRotate = CallFunc::create([](){
			log("Rotated!");
		});

		auto callbackTint = CallFunc::create([](){
			log("Tintorro!");
		});

		auto callbackScale = CallFunc::create([](){
			log ("Scaled");
		});

		// create a sequence with the actions and callbacks
		// TODO: mirar usos de los callbacks
		auto seq = Sequence::create(tintIda, callbackTint, jump, callbackJump, tintVuelta, nullptr);

		// run it
		sprite->runAction(seq);

	}


}

void EnemigoOOOLD::desActivar(){
	// bueno, si lo desactivo no puedo obtener su ultima posicion (por ejemplo lo uso desde la horda)
	// y al hacerlo invisible y sin fisica ergo colisiones, digo yo que no hace falta moverlo a (cualquiersitio,dondesea)
	//sprite->setPosition(Vec2::ZERO);
	sprite->setVisible(false);
	sprite->getPhysicsBody()->setEnabled(false);

}

void EnemigoOOOLD::activar(Vec2 posicion){
	sprite->setPosition(posicion);
	sprite->setVisible(true);
	sprite->getPhysicsBody()->setEnabled(true);
}

bool EnemigoOOOLD::estaActivo(){
	// TODO: es una manera
	return sprite->isVisible();
}


#pragma endregion