#include "GameActor.h"

#include "Game.h"
#include "SpaceInvaders.h"

#include "Testz.h"

GameActor::GameActor(){
	//CCLOG("Constructor de GameActor");
	// me aseguro de que está inicializado
	sprite = nullptr;

	// todos los "actores" tienen unas características comunes: movimiento, vida...
	gameActorHP = gameActorHPInicial;
	mueveIzq = mueveDch = mueveArr = mueveAbj = false;
	funcionMovimientoActual = nullptr;

	//spaceInvaderMovement_goingRight = false;
	spaceInvaderMovement_speedX = 0;
	spaceInvaderMovement_speedY = 0;
	spaceInvaderMovement_xMax = 0;
	spaceInvaderMovement_xMin = 0;
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
			//(this->*funcionMovimientoActual)(funcionMovimientoPosIni, funcionMovimientoAmplitude);
			(this->*funcionMovimientoActual)();
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

Sprite *GameActor::setSprite(Node *nodo, const char *ruta, const char *name, int tipoColision, int colisionaCon, float initialScale){
	// TODO: Mover esto a algún sitio y que se haga una sola vez??? -> Movido a AppDelegate
	//auto spritecache = SpriteFrameCache::getInstance();
	//spritecache->addSpriteFramesWithFile("spritesheet.plist");

	// Ejemplo de Polysprite para la posteridad:
	//// AutoPolygon viene bien para ahorrar proceso a la gpu (a estas alturas cuesta menos mover unos cuantos vértices que mirar si un pixel es transparente o no, particularmente en móviles)
	//// PERO no sirve para colisiones como creía, las formas generadas son más complejas que lo que el motor de física puede manejar con facilidad
	//AutoPolygon ap1 = AutoPolygon(ruta);
	//sprite = Sprite::create(ap1.generateTriangles());

	// TODO: Muy bien pero cómo hago un PolySprite con un plist?
	// Pues en realidad me puedo olvidar de ellos... si en el .plist se incluye la información poligonal (algorythm=polygon), Sprite::createWithSpriteFrameName lo aplica. Yeah!!

	// OJO: Sprite::createWithSpriteFrameName(ruta) -> CASE SENSITIVE! 
	sprite = Sprite::createWithSpriteFrameName(ruta);

	if(!sprite){
		CCLOG("GameActor::setSprite '%s'=SIN DEFINIR", ruta);
		return nullptr;// o return sprite pal caso
	}

	sprite->setScale(initialScale);

	sprite->setName(name);

	Game::anadeFisica(sprite, tipoColision, colisionaCon, name, ruta);

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

void GameActor::mueve(Vec2 donde){

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

void GameActor::mueveSeno(){
	Vec2 pos = getPosition();

	//float deltaT = Director::getInstance()->getDeltaTime();
	float ellapsed = Game::getInstance()->ellapsedTime;

	//pos.x = posIni.x + amplitude * sin(ellapsed / 2.0);
	pos.x = funcionMovimientoPosIni.x + funcionMovimientoAmplitude * sin(ellapsed * funcionMovimientoSpeed);

	setPosition(pos);

}

void GameActor::mueveSeno2(){
	Vec2 pos = getPosition();

	//float deltaT = Director::getInstance()->getDeltaTime();
	float ellapsed = Game::getInstance()->ellapsedTime;

	//pos.x = posIni.x + amplitude * sin(ellapsed / 2.0);
	pos.x = funcionMovimientoPosIni.x + funcionMovimientoAmplitude * sin(ellapsed * funcionMovimientoSpeed);
	pos.y += gameActorSpeed * Director::getInstance()->getDeltaTime();

	setPosition(pos);

}

void GameActor::mueveSpaceInvader(){
	float deltaT = Director::getInstance()->getDeltaTime();
	Vec2 curPos = sprite->getPosition();


	//if(spaceInvaderMovement_goingDown){
	if(SpaceInvaders::spaceInvaderMovement_goingDown) {
		// MOVIMIENTO VERTICAL
		curPos.y -= spaceInvaderMovement_speedY * deltaT;

		if(curPos.y <= spaceInvaderMovement_vcalMoveCurrTarget){
			//spaceInvaderMovement_goingDown = false;
			//spaceInvaderMovement_goingRight = !spaceInvaderMovement_goingRight;
			SpaceInvaders::spaceInvaderMovement_goingDown = false;
			SpaceInvaders::spaceInvaderMovement_goingRight = !SpaceInvaders::spaceInvaderMovement_goingRight;
		}
	} else{
		// MOVIMIENTO LATERAL
		//if(spaceInvaderMovement_goingRight){
		if(SpaceInvaders::spaceInvaderMovement_goingRight){
			curPos.x += spaceInvaderMovement_speedX * deltaT;

			if(curPos.x >= spaceInvaderMovement_xMax){
				////spaceInvaderMovement_goingRight = false;
				SpaceInvaders::spaceInvaderMovement_goingDown = true;
				//spaceInvaderMovement_goingDown = true;
				spaceInvaderMovement_vcalMoveCurrTarget = curPos.y - spaceInvaderMovement_vcalMoveAmount;
				// no hago "clipping" del movimiento porque me parece que se me van a apuchurrar los invaders a los lados
			}
		} else{
			curPos.x -= spaceInvaderMovement_speedX * deltaT;

			if(curPos.x <= spaceInvaderMovement_xMin){
				////spaceInvaderMovement_goingRight = true;
				SpaceInvaders::spaceInvaderMovement_goingDown = true;
				//spaceInvaderMovement_goingDown = true;
				spaceInvaderMovement_vcalMoveCurrTarget = curPos.y - spaceInvaderMovement_vcalMoveAmount;
				// no hago "clipping" del movimiento porque me parece que se me van a apuchurrar los invaders a los lados
			}
		}
	}

	sprite->setPosition(curPos);

}



// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


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

// TODO: unused, borrar!!!
void GameActor::loadSpriteSheet(Node *nodo){
	// load the Sprite Sheet
	auto spritecache = SpriteFrameCache::getInstance();

	// the .plist file can be generated with any of the tools mentioned below
	spritecache->addSpriteFramesWithFile("spritesheet.plist");
	
	// Our .plist file has names for each of the sprites in it.  We'll grab
	// the sprite named, "mysprite" from the sprite sheet:
	auto mysprite = Sprite::createWithSpriteFrameName("explosion0.png");
	// TODO: ¿Cómo sabe de qué spritesheet?
	// Y por cierto: CASE SENSITIVE! 

	//// this is equivalent to the previous example,
	//// but it is created by retrieving the SpriteFrame from the cache.
	//auto newspriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("explosion0.png");
	//auto newSprite = Sprite::createWithSpriteFrame(newspriteFrame);

	mysprite->setPosition(Vec2(400, 400));

	nodo->addChild(mysprite);

	// oki esto funciona
}