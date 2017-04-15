#include "Enemy.h"

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


	Pool::currentEnemies.push_back(this);
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
		//Pool::activa(weapon->bulletPool, getPosition());

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
