#include "Enemy.h"

Enemy::Enemy(Node *nodo, const char *pathSprite, const char *rutaSonidoMuerte, float initialScale, float initialRotation, float hp){

	createEnemy(nodo, pathSprite, rutaSonidoMuerte, initialScale, initialRotation, hp);

}

Enemy::Enemy(Node *nodo, enum tiposEnemigo tipo){

	switch(tipo){
	case tiposEnemigo::tipo1:
		createEnemy(nodo, ENEMY_T1_PATH_SPRITE, "", ENEMY_T1_INITIAL_SIZE, ENEMY_T1_INITIAL_ROTATION, ENEMY_GENERIC_HP);
		break;
	case tiposEnemigo::tipo2:
		createEnemy(nodo, ENEMY_T2_PATH_SPRITE, "", ENEMY_T2_INITIAL_SIZE, ENEMY_T2_INITIAL_ROTATION, ENEMY_GENERIC_HP);
		break;
	case tiposEnemigo::tipoBoss:
		createEnemy(nodo, ENEMY_BOSS_PATH_SPRITE, "", ENEMY_BOSS_INITIAL_SIZE, ENEMY_BOSS_INITIAL_ROTATION, ENEMY_BOSS_GENERIC_HP);
		break;
	default:
		CCLOG("Intento de crear tipo de enemigo desconocido %d", tipo);
		break;
	}

}

void Enemy::createEnemy(Node * nodo, const char * pathSprite, const char * rutaSonidoMuerte, float initialScale, float rotation, float hp){
	GameActor::GameActor();
	GameActor::gameActorHP = hp;
	GameActor::gameActorSpeed = ENEMY_GENERIC_SPEED;

	setSprite(nodo, pathSprite, "Enemigo", (int)Game::CategoriaColision::Enemigo, (int)Game::CategoriaColision::Jugador | (int)Game::CategoriaColision::Bala, initialScale, true);

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
		// TODO: y distinguir el tipo de enemigo que es!
		//Game::getInstance()->puntos += ENEMY_GENERIC_POINTS;
		Game::getInstance()->sumaPuntos(ENEMY_GENERIC_POINTS);
	} else{
		// no muere

		// la bala (si es lo que le ha golpeado) ya saca un grafico de impacto
	}


}

void Enemy::funControlFireAtInterval(float interval){
	float ellapsedTime = Game::getInstance()->ellapsedTime;

	if(ellapsedTime - tIniDisparo > interval){
		// TODO: Disparar (enemigo)
		dispara();

		tIniDisparo = ellapsedTime;
	}

}

void Enemy::funControlFireRandom(float pufo){
	// dispara segun probabilidad aleatoria	
	if(cocos2d::RandomHelper::random_int<int>(1, funcionControlProbDisparoAleatoria) == 1){
		dispara();
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
		//CCLOG("Enemigo dispara!");

		Pool::activa(*poolMisBalas, sprite->getPosition());
		
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