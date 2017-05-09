#include "Enemy.h"

Enemy::Enemy(Node *nodo, const char *pathSprite, const char *rutaSonidoMuerte, float initialScale, float initialRotation, float hp){
	createEnemy(nodo, pathSprite, rutaSonidoMuerte, initialScale, initialRotation, hp);
}

Enemy::Enemy(Node *nodo, enum tiposEnemigo tipo){
	Game *gameInstance = Game::getInstance();

	switch(tipo){
	case tiposEnemigo::tipo1:
		createEnemy(nodo, gameInstance->enemy_t1_path_sprite.c_str(), "", gameInstance->enemy_t1_initial_size, gameInstance->enemy_t1_initial_rotation, gameInstance->enemy_generic_hp);
		break;
	case tiposEnemigo::tipo2:
		createEnemy(nodo, gameInstance->enemy_t2_path_sprite.c_str(), "", gameInstance->enemy_t2_initial_size, gameInstance->enemy_t2_initial_rotation, gameInstance->enemy_generic_hp);
		break;
	case tiposEnemigo::tipoBoss:
		createEnemy(nodo, gameInstance->enemy_boss_path_sprite.c_str(), "", gameInstance->enemy_boss_initial_size, gameInstance->enemy_boss_initial_rotation, gameInstance->enemy_boss_generic_hp, gameInstance->enemy_boss_points);
		break;
	default:
		CCLOG("Intento de crear tipo de enemigo desconocido %d", tipo);
		break;
	}

}

void Enemy::createEnemy(Node * nodo, const char * pathSprite, const char * rutaSonidoMuerte, float initialScale, float rotation, float hp, int points){
	GameActor::GameActor();
	GameActor::gameActorHP = hp;
	GameActor::gameActorSpeed = Game::getInstance()->enemy_generic_speed;

	setSprite(nodo, pathSprite, "Enemigo", (int)Game::CategoriaColision::Enemigo, (int)Game::CategoriaColision::Jugador | (int)Game::CategoriaColision::Bala, initialScale, true);

	tIniDisparo = 0;
	funcionControlActual = nullptr;
	//funcionMovimientoActual = nullptr;// esto lo hace el constructor de GameActor
	funcionControlTiempoDisparo = 1.0f;

	//Pool::currentEnemies.push_back(this);
	poolMisBalas = nullptr;

	pointsOnDeath = points;
}

Enemy::~Enemy(){
	CCLOG ("Enemy destructor");
}

void Enemy::impacto(float dmg){
	// TODO: Escudo?

	// recibe da�o
	gameActorHP -= dmg;

	if(gameActorHP <= 0){
		// muere
		desactiva();
		// TODO: explosion
		// TODO: dar puntos
		// TODO: y distinguir el tipo de enemigo que es!
		//Game::getInstance()->puntos += ENEMY_GENERIC_POINTS;
		Game::getInstance()->sumaPuntos(pointsOnDeath);
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
			// TODO: Quiero una funci�n de movimiento est�ndar-por-defecto para algo? :(
			mueve();
		}

		if(this->funcionControlActual){
			// A�ado una funcion de control de disparo. OOOH funciona!!
			(this->*funcionControlActual)(funcionControlTiempoDisparo);
		}
	}
}


void Enemy::dispara(){
	if(poolMisBalas){
		//CCLOG("Enemigo dispara!");

		Pool::activa(*poolMisBalas, sprite->getPosition());
		
		// qu� tipo de bala disparo?
		// ...
		// qu� da�o hace?
		// ...
		// de qu� tama�o?
		// ...		
	} else{
		CCLOG("Enemigo intenta disparar sin pool");
	}
}