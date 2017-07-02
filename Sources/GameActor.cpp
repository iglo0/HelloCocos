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
	//mueveIzq = mueveDch = mueveArr = mueveAbj = false;
	//funcionMovimientoActual = nullptr;

	movimiento = nullptr;
}

GameActor::~GameActor(){
	//CCLOG("Destructor de GameActor");
}

// TODO: no quiero que todos los GameActor se comporten igual. ¿Cómo hago que un enemigo se controle de la forma 1 o 2 o 3?
void GameActor::update(float deltaT){
	//CCLOG("GameActor update @%f", Game::getInstance()->ellapsedTime);

	// TODO: hum, por ejemplo una bala entra por gameactor::update y luego quiero que vaya a bullet::mueve?
	// más bien que sobrescriba el método update la bala también?
	// JODER. Ha funcionado? Si paso una bala que tiene mueve sobrescrito, lo entiende y ejecuta el mueve correcto? Joder. Mola!!!
	if(isActive()){
		// Me tengo que mirar detenidamente pasar funciones como parametro
		//if(this->funcionMovimientoActual){
		//	// TODO: si proporciono una funcion de movimiento, usa esta
		//	//(this->*funcionMovimientoActual)(funcionMovimientoPosIni, funcionMovimientoAmplitude);
		//	(this->*funcionMovimientoActual)();
		//} else{
			// TODO: por aquí pasa el player. No sé si me convence cómo lo tengo montado, aún...
			//mueve();
		//}
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

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// TODO: no es lo mismo activar una bala (dirigida, normal, ...) que un enemigo? tendría que permitir heredar por tipo de GameActor...
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
//void GameActor::loadSpriteSheet(Node *nodo){
//	// load the Sprite Sheet
//	auto spritecache = SpriteFrameCache::getInstance();
//
//	// the .plist file can be generated with any of the tools mentioned below
//	spritecache->addSpriteFramesWithFile("spritesheet.plist");
//	
//	// Our .plist file has names for each of the sprites in it.  We'll grab
//	// the sprite named, "mysprite" from the sprite sheet:
//	auto mysprite = Sprite::createWithSpriteFrameName("explosion0.png");
//	// TODO: ¿Cómo sabe de qué spritesheet?
//	// Y por cierto: CASE SENSITIVE! 
//
//	//// this is equivalent to the previous example,
//	//// but it is created by retrieving the SpriteFrame from the cache.
//	//auto newspriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("explosion0.png");
//	//auto newSprite = Sprite::createWithSpriteFrame(newspriteFrame);
//
//	mysprite->setPosition(Vec2(400, 400));
//
//	nodo->addChild(mysprite);
//
//	// oki esto funciona
//}