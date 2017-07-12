#include "GameActor.h"

#include "Game.h"
#include "SpaceInvaders.h"

#include "Testz.h"

GameActor::GameActor(){
	//CCLOG("Constructor de GameActor");
	// me aseguro de que está inicializado
	sprite_ = nullptr;

	// todos los "actores" tienen unas características comunes: movimiento, vida...
	gameActorHP_ = gameActorHPInicial_;
	//mueveIzq = mueveDch = mueveArr = mueveAbj = false;
	//funcionMovimientoActual = nullptr;

	movimiento_ = nullptr;
	estaActivo_ = false;
}

GameActor::~GameActor(){
	//CCLOG("Destructor de GameActor");
}

// TODO: Aquí la he liado, no quiero un métood update en GameActor... quiero que lo hereden los hijos
void GameActor::update(float deltaT){
}

Vec2 GameActor::getPosition(){
	if(sprite_){
		return sprite_->getPosition();
	}
	// TODO: Ojo error
	return Vec2();
}

void GameActor::setPosition(Vec2 pos){
	if(sprite_){
		sprite_->setPosition(pos);
	} else{
		CCLOG("GameActor::setPosition -> no sprite");
	}
}

Sprite *GameActor::setSprite(Node *nodo, const char *ruta, const char *name, int tipoColision, int colisionaCon, float initialScale){
	// Ejemplo de Polysprite para la posteridad:
	//// AutoPolygon viene bien para ahorrar proceso a la gpu (a estas alturas cuesta menos mover unos cuantos vértices que mirar si un pixel es transparente o no, particularmente en móviles)
	//// PERO no sirve para colisiones como creía, las formas generadas son más complejas que lo que el motor de física puede manejar con facilidad
	//AutoPolygon ap1 = AutoPolygon(ruta);
	//sprite = Sprite::create(ap1.generateTriangles());

	// TODO: Muy bien pero cómo hago un PolySprite con un plist?
	// Pues en realidad me puedo olvidar de ellos... si en el .plist se incluye la información poligonal (algorythm=polygon), Sprite::createWithSpriteFrameName lo aplica. Yeah!!

	// OJO: Sprite::createWithSpriteFrameName(ruta) -> CASE SENSITIVE! 
	sprite_ = Sprite::createWithSpriteFrameName(ruta);

	if(!sprite_){
		CCLOG("GameActor::setSprite '%s'=SIN DEFINIR", ruta);
		return nullptr;// o return sprite pal caso
	}

	sprite_->setScale(initialScale);

	sprite_->setName(name);

	Game::anadeFisica(sprite_, tipoColision, colisionaCon, name, ruta);

	//TODO: ya tengo para recuperar mis datos :)
	sprite_->setUserData(this);
	// y su tipo
	sprite_->setTag(tipoColision);

	// lo creo invisible y sin colisiones activas
	desactiva();

	// hecho
	nodo->addChild(sprite_);

	return sprite_;
}

Sprite *GameActor::getSprite(){
	return sprite_;
}

void GameActor::mueve(Vec2 donde){

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// TODO: no es lo mismo activar una bala (dirigida, normal, ...) que un enemigo? tendría que permitir heredar por tipo de GameActor...
void GameActor::activa(Vec2 pos){
	if(sprite_){
		sprite_->setPosition(pos);

		sprite_->setVisible(true);
		// TODO: Activar físicas

		PhysicsBody *pb = sprite_->getPhysicsBody();
		if(pb){
			pb->setEnabled(true);
		}
	}

	estaActivo_ = true;
}

void GameActor::desactiva(){
	if(sprite_){
		sprite_->setVisible(false);

		PhysicsBody *pb = sprite_->getPhysicsBody();
		if(pb){
			pb->setEnabled(false);
		}
		
	}

	estaActivo_ = false;
}

bool GameActor::isActive(){
	return estaActivo_;
	//if(sprite_){
	//	// HACK: por ejemplo
	//	return sprite_->isVisible();
	//}
	//// si he llegado aquí, que no hay ni sprite...
	//return false;
}


void GameActor::impacto(float dmg){
	const char *name = sprite_->getName().c_str();

	CCLOG ("GameActor %s says: ouch %f", name, dmg);
}
