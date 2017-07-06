#include "Enemy.h"
// OJO!!!: En general es mejor dejar en el .h los #includes necesarios, y para evitar referencias cíclicas declaraciones adelantadas para punteros a objetos de otros tipos
// y ya en el .cpp, #incluir lo necesario
// OJO: Y además, debería reducir el tiempo de compilación

#include "SpaceInvaders.h"
#include "Pool.h"
#include "Game.h"

//Enemy::Enemy(Node *nodo, const char *pathSprite, const char *rutaSonidoMuerte, float initialScale, float initialRotation, float hp){
//	//funcionMovimientoActual = nullptr;
//	createEnemy(nodo, pathSprite, rutaSonidoMuerte, initialScale, initialRotation, hp, Game::getInstance()->enemy_generic_points);
//}

Enemy::Enemy(tiposEnemigo tipo){
	GameActor::GameActor();
	//GameActor::gameActorHP = 0;
	GameActor::gameActorSpeed_ = 0;
	funcionControlActual_ = nullptr;
	tipoEnemigo_ = tipo;
	pointsOnDeath_ = 0;
	poolMisBalas_ = nullptr;
}

Enemy::~Enemy(){}



void Enemy::initEnemy(Node *nodo){
	Game *gameInstance = Game::getInstance();
	//funcionMovimientoActual = nullptr;

	switch(tipoEnemigo_){
	case tiposEnemigo::tipo1:
		initEnemy(nodo, gameInstance->enemy_t1_path_sprite.c_str(), "", gameInstance->enemy_t1_initial_size, gameInstance->enemy_t1_initial_rotation, gameInstance->enemy_generic_hp, gameInstance->enemy_generic_points, &Pool::currentBulletsTipoNormal);
		break;
	case tiposEnemigo::tipo2:
		initEnemy(nodo, gameInstance->enemy_t2_path_sprite.c_str(), "", gameInstance->enemy_t2_initial_size, gameInstance->enemy_t2_initial_rotation, gameInstance->enemy_generic_hp, gameInstance->enemy_generic_points, &Pool::currentBulletsTipo2);
		break;
	case tiposEnemigo::tipoBoss:
		initEnemy(nodo, gameInstance->enemy_boss_path_sprite.c_str(), "", gameInstance->enemy_boss_initial_size, gameInstance->enemy_boss_initial_rotation, gameInstance->enemy_boss_generic_hp, gameInstance->enemy_boss_points, &Pool::currentBulletsTipoBossHoming);
		break;
	default:
		CCLOG("Intento de crear tipo de enemigo desconocido %d", tipoEnemigo_);
		break;
	}
}

void Enemy::initEnemy(Node * nodo, const char * pathSprite, const char * rutaSonidoMuerte, float initialScale, float rotation, float hp, int points, std::vector<Bullet *> *poolMisBalas){
	GameActor::gameActorHP_ = hp;
	GameActor::gameActorSpeed_ = Game::getInstance()->enemy_generic_speed;

	setSprite(nodo, pathSprite, "Enemigo", (int)Game::CategoriaColision::Enemigo, (int)Game::CategoriaColision::Jugador | (int)Game::CategoriaColision::Bala, initialScale);

	//tIniDisparo_ = 0;
	//funcionControlActual = nullptr;
	////funcionMovimientoActual = nullptr;// esto lo hace el constructor de GameActor
	//funcionControlTiempoDisparo_ = 1.0f;

	//Pool::currentEnemies.push_back(this);
	poolMisBalas_ = poolMisBalas;

	pointsOnDeath_ = points;
	// inicializar esto luego, que habrá que pasarle parámetros y es mejor dedicarle un poco más de atención
	movimiento_ = nullptr;
}

void Enemy::impacto(float dmg){
	// TODO: Escudo?

	// recibe daño
	gameActorHP_ -= dmg;

	if(gameActorHP_ <= 0){
		// muere
		desactiva();
		// TODO: explosion
		// TODO: dar puntos
		// TODO: y distinguir el tipo de enemigo que es!
		//Game::getInstance()->puntos += ENEMY_GENERIC_POINTS;
		Game::getInstance()->sumaPuntos(pointsOnDeath_);

		// HACK: Ajusto el nº de invaders que quedan cuando muere uno
		// TODO: ponerlo en un sitio más adecuado, una vez probado
		// TODO: Ojo el contador se decrementa cuando cae el "ovni"
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

	if(ellapsedTime - tIniDisparo_ > interval){
		// TODO: Disparar (enemigo)
		dispara();

		tIniDisparo_ = ellapsedTime;
	}

}

void Enemy::funControlFireRandom(float pufo){
	// dispara segun probabilidad aleatoria	
	// HACK: y según el nº de invaders vivos :)
	int dado = (int)((float)funcionControlProbDisparoAleatoria_ * SpaceInvaders::porcenInvadersVivos);
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
		//if(this->funcionMovimientoActual){
		//	(this->*funcionMovimientoActual)();
		//}

		if(this->funcionControlActual_){
			// Añado una funcion de control de disparo. OOOH funciona!!
			(this->*funcionControlActual_)(funcionControlTiempoDisparo_);
		}
	}
}
void Enemy::dispara(){
	if(poolMisBalas_){
		//CCLOG("Enemigo dispara!");

		Pool::activa(*poolMisBalas_, sprite_->getPosition());
		
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