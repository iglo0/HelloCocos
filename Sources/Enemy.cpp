#include "Enemy.h"
// OJO!!!: En general es mejor dejar en el .h los #includes necesarios, y para evitar referencias c�clicas declaraciones adelantadas para punteros a objetos de otros tipos
// y ya en el .cpp, #incluir lo necesario
// OJO: Y adem�s, deber�a reducir el tiempo de compilaci�n

#include "SpaceInvaders.h"
#include "Pool.h"
#include "Game.h"
#include "Bullet.h"
#include "Movimiento.h"
#include "AnimSprites.h"
#include "XmlHelper.h"
#include "SimpleAudioEngine.h"

Enemy::Enemy(){
	GameActor::GameActor();
	GameActor::gameActorSpeed_ = 0;
	funcionControlActual_ = nullptr;
	//tipoEnemigo_ = -1;
	pointsOnDeath_ = 0;
	poolMisBalas_ = nullptr;
	sonidoDispara_ = "";
	sonidoMuerte_ = "";
}

Enemy::Enemy(tiposEnemigo tipo){
	GameActor::GameActor();
	//GameActor::gameActorHP = 0;
	GameActor::gameActorSpeed_ = 0;
	funcionControlActual_ = nullptr;
	tipoEnemigo_ = tipo;
	pointsOnDeath_ = 0;
	poolMisBalas_ = nullptr;
	sonidoDispara_ = "";
	sonidoMuerte_ = "";
}

Enemy::~Enemy(){}

void Enemy::initEnemy(Node *nodo){
	Game *gameInstance = Game::getInstance();
	//funcionMovimientoActual = nullptr;

	switch(tipoEnemigo_){
	//case tiposEnemigo::tipo1:
	//	initEnemy(nodo, gameInstance->enemy_t1_path_sprite.c_str(), "", gameInstance->enemy_t1_initial_size, gameInstance->enemy_t1_initial_rotation, gameInstance->enemy_generic_hp, gameInstance->enemy_generic_points, &Pool::currentBulletsTipoNormal);
	//	break;
	//case tiposEnemigo::tipo2:
	//	initEnemy(nodo, gameInstance->enemy_t2_path_sprite.c_str(), "", gameInstance->enemy_t2_initial_size, gameInstance->enemy_t2_initial_rotation, gameInstance->enemy_generic_hp, gameInstance->enemy_generic_points, &Pool::currentBulletsTipo2);
	//	break;
	case tiposEnemigo::tipoBoss:
		initEnemy(nodo, gameInstance->enemy_boss_path_sprite.c_str(), "", gameInstance->enemy_boss_initial_size, gameInstance->enemy_boss_initial_rotation, gameInstance->enemy_boss_generic_hp, gameInstance->enemy_boss_points, &Pool::currentBulletsTipoBossHoming);
		break;
	default:
		CCLOG("Intento de crear tipo de enemigo desconocido %d", tipoEnemigo_);
		break;
	}
}

// OJO: Static
Enemy *Enemy::createEnemy(Node *nodo, const char *enemyDefName){
	XmlHelper *xh = new XmlHelper();
	Enemy *tmp;

	tmp = xh->loadEnemy(nodo, enemyDefName);

	return tmp;

	//GameActor::gameActorHP_ = hp;
	//GameActor::gameActorSpeed_ = Game::getInstance()->enemy_generic_speed;

	//setSprite(nodo, pathSprite, "Enemigo", (int)Game::CategoriaColision::Enemigo,
	//	(int)Game::CategoriaColision::Jugador | (int)Game::CategoriaColision::BalaJugador | (int)Game::CategoriaColision::Destructible,
	//	initialScale);

	//poolMisBalas_ = poolMisBalas;
	//pointsOnDeath_ = points;
	//movimiento_ = nullptr;
}

void Enemy::initEnemy(Node *nodo, const char *pathSprite, const char *rutaSonidoMuerte, float initialScale, float rotation, float hp, int points, std::vector<Bullet *> *poolMisBalas){
	GameActor::gameActorHP_ = hp;
	GameActor::gameActorSpeed_ = Game::getInstance()->enemy_generic_speed;

	setSprite(nodo, pathSprite, "Enemigo", (int)Game::CategoriaColision::Enemigo,
		(int)Game::CategoriaColision::Jugador | (int)Game::CategoriaColision::BalaJugador | (int)Game::CategoriaColision::Destructible,
		initialScale);

	poolMisBalas_ = poolMisBalas;

	pointsOnDeath_ = points;
	// inicializar esto luego, que habr� que pasarle par�metros y es mejor dedicarle un poco m�s de atenci�n
	movimiento_ = nullptr;
}

void Enemy::impacto(float dmg){
	// TODO: Escudo?

	// recibe da�o
	gameActorHP_ -= dmg;

	if(gameActorHP_ <= 0){
		// muere
		desactiva();
		// explota
		Pool::activa(Pool::currentExplosions, getPosition());
		// da puntos
		Game::getInstance()->sumaPuntos(pointsOnDeath_);

		// HACK: Ajusto el n� de invaders que quedan cuando muere uno
		// TODO: ponerlo en un sitio m�s adecuado, una vez probado
		// TODO: Ojo el contador se decrementa cuando cae el "ovni"
		--SpaceInvaders::numInvadersVivos;
		if(SpaceInvaders::numInvadersVivos > 0){
			SpaceInvaders::porcenInvadersVivos = (float)SpaceInvaders::numInvadersVivos / (float)SpaceInvaders::numInvadersInicial;
		}

		if(sonidoMuerte_ != ""){
			auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
			audio->playEffect(sonidoMuerte_.c_str());
		}

	} else{
		// TODO: efectos de recibir da�o sin morir
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
	// HACK: y seg�n el n� de invaders vivos :)
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

	// a 09/2017 es solo para el futuro animSprites
	GameActor::update(deltaT);

	if(isActive()){
		// TODO: Me tengo que mirar detenidamente pasar funciones como parametro
		// si proporciono una funcion de movimiento, usa esta
		//if(this->funcionMovimientoActual){
		//	(this->*funcionMovimientoActual)();
		//}

		if(movimiento_){
			// TODO: actualizar movimiento para que funcione con AnimSprites
			if(animSprites_){
				setPosition(movimiento_->mueve(animSprites_->getPosition(), deltaT));
			}
			if(sprite_){
				setPosition(movimiento_->mueve(sprite_->getPosition(), deltaT));
			}
		}

		if(this->funcionControlActual_){
			// A�ado una funcion de control de disparo. OOOH funciona!!
			(this->*funcionControlActual_)(funcionControlTiempoDisparo_);
		}
	}
}

void Enemy::dispara(){
	if(poolMisBalas_){
		//CCLOG("Enemigo dispara!");

		if(sonidoDispara_!=""){
			auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
			audio->playEffect(sonidoDispara_.c_str());
		}

		if(animSprites_){
			Pool::activa(*poolMisBalas_, animSprites_->getPosition());
		}
		if(sprite_){
			Pool::activa(*poolMisBalas_, sprite_->getPosition());
		}
		
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

void Enemy::setPoints(int points){
	pointsOnDeath_ = points;
}

void Enemy::setSonidoDispara(const char *pathSonido){
	sonidoDispara_ = std::string(pathSonido);
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->preloadEffect(pathSonido);
}

void Enemy::setSonidoMuerte(const char *pathSonido){
	sonidoMuerte_ = std::string(pathSonido);
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->preloadEffect(pathSonido);
}
