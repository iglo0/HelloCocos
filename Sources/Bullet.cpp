#include "Bullet.h"

Bullet::Bullet(Node *nodo, const char *name, const char *pathSprite, const char *pathSonidoDisparo, const char *pathSonidoImpacto, float speed, float dmg, int tipoColision, int colisionoCon, float initialScale){
	// inicializa la clase base primero
	GameActor::GameActor();
	//bulletSpeed = speed;
	bulletDmg = dmg;
	gameActorSpeed = speed;
	
	CCLOG("creando bala: %s", name);
	//if(!createBullet(nodo, pathSprite, name, tipoColision, colisionoCon)){
	if(!GameActor::setSprite(nodo, pathSprite,name,tipoColision, colisionoCon,initialScale, false)){
		CCLOG("No pude crear bala %s", pathSprite);
	}
}

Bullet::~Bullet(){
}

// (OJO) static method
void Bullet::createBulletPool(Node *nodo, std::vector<Bullet *> &pool, int poolSize, const char *name, const char *pathSprite, const char *pathSonidoDisparo, const char *pathSonidoImpacto, float speed, float dmg, int tipoColision, int colisionoCon, float initialScale){
	Bullet *tmp;

	for(int i = 0; i < poolSize; i++){
		tmp = new Bullet(nodo, (name + std::to_string(i)).c_str(), pathSprite, pathSonidoDisparo, pathSonidoImpacto, speed, dmg, tipoColision, colisionoCon, initialScale);
		
		// TODO: que mas cosas hacer a la bala?

		pool.push_back(tmp);
	}
	
	
}

void Bullet::impacto(float dmg){
	// Sigo sorprendido que haciendo gameActorX->impacto(), sepa ejectura la función correcta :D

	// esta es facil, la bala desaparece
	desactiva();

	// TODO: reproducir sonido

	// TODO: animacion impacto
}


void Bullet::mueve(){

	Vec2 pos = getPosition();
	float deltaT = Director::getInstance()->getDeltaTime();

	// HACK: balas del prota van hacia arriba, de los enemigos hacia abajo
	// TODO: implementarlo con vectores para cuando tenga disparos más dirigidos (típico "spray" de balas en abanico)
	pos.y += gameActorSpeed * deltaT;

	// compruebo si esta bala ha llegado al límite de la pantalla para desactivarla
	// recordatorio, en cocos-land 0,0 es la esquina inferior izquierda
	if(gameActorSpeed < 0){
		// chequeo si se sale por abajo
		if(pos.y < 0){
			pos.y = 0;
			desactiva();
		}
	} else{
		// o por arriba.
		if(pos.y > Director::getInstance()->getVisibleSize().height){
			pos.y = 0;
			desactiva();
		}
	}


	setPosition(pos);


}