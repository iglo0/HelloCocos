#include "Enemy.h"

Enemy::Enemy(Node *nodo, const char *pathSprite, const char *rutaSonidoMuerte, float initialScale, float initialRotation, float hp){
	GameActor::GameActor();

	setSprite(nodo, pathSprite, "Enemigo", (int)Game::CategoriaColision::Enemigo, (int)Game::CategoriaColision::Jugador | (int)Game::CategoriaColision::Bala,initialScale,true);
	
	GameActor::gameActorHP = hp;
	GameActor::gameActorSpeed = ENEMY_GENERIC_SPEED;

	tIniDisparo = 0;
	funcionControlActual = nullptr;
	//funcionMovimientoActual = nullptr;// esto lo hace el constructor de GameActor
	funcionControlTiempoDisparo = 1.0f;

	//Pool::currentEnemies.push_back(this);
	poolMisBalas = nullptr;
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
		// TODO: Disparar (enemigo)
		dispara();

		tIniDisparo = ellapsedTime;
	}

}

void Enemy::update(float deltaT){
	//CCLOG("GameActor update @%f", Game::getInstance()->ellapsedTime);

	if(isActive()){
		// TODO: Me tengo que mirar detenidamente pasar funciones como parametro
		// si proporciono una funcion de movimiento, usa esta
		if(this->funcionMovimientoActual){
			//(this->*funcionMovimientoActual)(this->funcionMovimientoPosIni, this->funcionMovimientoAmplitude);
			(this->*funcionMovimientoActual)();


		} else{
			// TODO: Quiero una función de movimiento estándar-por-defecto para algo? :(
			mueve();
		}

		if(this->funcionControlActual){
			// Añado una funcion de control de disparo. OOOH funciona!!
			(this->*funcionControlActual)(funcionControlTiempoDisparo);
		}
	}
}


void Enemy::dispara(){
	if(poolMisBalas){
		CCLOG("Enemigo dispara!");

		
		// qué tipo de bala disparo?
		// ...
		// qué daño hace?
		// ...
		// de qué tamaño?
		// ...		
	} else{
		CCLOG("Enemigo intenta disparar sin pool");
	}
}