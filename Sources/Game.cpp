#include "Game.h"

Game::Game(){
	ellapsedTime = 0;
	estadoActual = estadosJuego::menus;
}

Game *Game::getInstance(){
	// thread safe. Así es como me aseguro que se inicializa exactamente una vez, aun con concurrencia
	// el modo tradicional (una variable miembro privada) podría fallar por lo anterior.
	static Game *instance = new Game;
	return instance;
}
	


void Game::anadeFisica(Sprite *sprite, int tipoColision, int colisionaCon, const char *name){

	if(!sprite){
		CCLOG("Error anadiendo fisica al sprite: sprite sin definir");
		return;
	}

	PhysicsBody *fisicaSprite;

	// TODO: PhysicsMaterial OJO
	// Density=0.1f, Restitution=1.0f, Friction=0
	// No tengo ni idea, copiado de un ejemplo sencillo para solo colisiones y que funciona

	// TODO: parece que no es posible convertir autoPolygons en PhysicsBody::createPolygon. LA recomendación es hacerlos a mano (p.ej. con PhysicsEditor -trial 7 dias-)
	// el autopolygon está optimizado para render, no para física
	// los poligonos de física soportan formas menos complejas que los sprites autopolygon
	// empiezo por crear una caja alrededor del sprite
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

	sprite->setName(name);

	// hecho

}


//Game::~Game(){
//}
