#include "Enemy.h"

#include "SpaceInvaders.h"

// OJO!!!: En general es mejor dejar en el .h los #includes necesarios, y para evitar referencias cíclicas declaraciones adelantadas para punteros a objetos de otros tipos
// y ya en el .cpp, #incluir lo necesario
// OJO: Y además, debería reducir el tiempo de compilación
#include "Pool.h"
#include "Game.h"

Enemy::Enemy(Node *nodo, const char *pathSprite, const char *rutaSonidoMuerte, float initialScale, float initialRotation, float hp){
	createEnemy(nodo, pathSprite, rutaSonidoMuerte, initialScale, initialRotation, hp, Game::getInstance()->enemy_generic_points);
}

Enemy::Enemy(Node *nodo, enum tiposEnemigo tipo){
	Game *gameInstance = Game::getInstance();

	switch(tipo){
	case tiposEnemigo::tipo1:
		createEnemy(nodo, gameInstance->enemy_t1_path_sprite.c_str(), "", gameInstance->enemy_t1_initial_size, gameInstance->enemy_t1_initial_rotation, gameInstance->enemy_generic_hp, gameInstance->enemy_generic_points);
		break;
	case tiposEnemigo::tipo2:
		createEnemy(nodo, gameInstance->enemy_t2_path_sprite.c_str(), "", gameInstance->enemy_t2_initial_size, gameInstance->enemy_t2_initial_rotation, gameInstance->enemy_generic_hp, gameInstance->enemy_generic_points);
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

	setSprite(nodo, pathSprite, "Enemigo", (int)Game::CategoriaColision::Enemigo, (int)Game::CategoriaColision::Jugador | (int)Game::CategoriaColision::Bala, initialScale);

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

	// recibe daño
	gameActorHP -= dmg;

	if(gameActorHP <= 0){
		// muere
		desactiva();
		// TODO: explosion
		// TODO: dar puntos
		// TODO: y distinguir el tipo de enemigo que es!
		//Game::getInstance()->puntos += ENEMY_GENERIC_POINTS;
		Game::getInstance()->sumaPuntos(pointsOnDeath);

		// HACK: Ajusto el nº de invaders que quedan cuando muere uno
		// TODO: ponerlo en un sitio más adecuado, una vez probado
		--SpaceInvaders::numInvadersVivos;
		if(SpaceInvaders::numInvadersVivos > 0){
			SpaceInvaders::porcenInvadersVivos = (float)SpaceInvaders::numInvadersVivos / (float)SpaceInvaders::numInvadersInicial;
		}

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
	// HACK: y según el nº de invaders vivos :)
	int dado = (int)((float)funcionControlProbDisparoAleatoria * SpaceInvaders::porcenInvadersVivos);
	if(dado < 1){
		dado = 1;
	}
	if(cocos2d::RandomHelper::random_int<int>(1, dado) == 1){
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