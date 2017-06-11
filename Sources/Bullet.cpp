#include "Bullet.h"

#include "Game.h"

#include "Movimiento.h"
#include "Player.h"

USING_NS_CC;

Bullet::Bullet(Node *nodo, const char *name, const char *pathSprite, const char *pathSonidoDisparo, const char *pathSonidoImpacto, float speed, float dmg, int tipoColision, int colisionoCon, float initialScale){
	// inicializa la clase base primero
	GameActor::GameActor();
	//bulletSpeed = speed;
	bulletDmg = dmg;
	gameActorSpeed = speed;
	
	//CCLOG("creando bala: %s", name);
	//if(!createBullet(nodo, pathSprite, name, tipoColision, colisionoCon)){
	if(!GameActor::setSprite(nodo, pathSprite,name,tipoColision, colisionoCon,initialScale)){
		CCLOG("No pude crear bala %s", pathSprite);
	}

	ttl = BULLET_HOMING_TTL;
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

Bullet *Bullet::creaBala(Node *nodo, tiposBala tipoBala, const char *bulletName){
	return creaBala(nodo, tipoBala, bulletName, &GameActor::mueve, new MueveDireccion());
}

Bullet *Bullet::creaBala(Node *nodo, tiposBala tipoBala, const char *bulletName, punteroAFuncionMovimiento funcionMovimiento, Movimiento *claseMovimiento){
	Bullet *tmp;
	Game *gameInstance = Game::getInstance();

	// los parametros de la bala
	const char *pathSprite;
	const char *pathSonidoDisparo;
	const char *pathSonidoImpacto;
	float speed;
	float dmg;
	int tipoColision;
	int colisionoCon;
	float initialScale;

	switch(tipoBala){
	case tipoPlayer:
		pathSprite = gameInstance->bullet_player_path_sprite1.c_str();
		pathSonidoDisparo = gameInstance->bullet_path_sound_fire.c_str();
		pathSonidoImpacto = gameInstance->bullet_path_sound_impact.c_str();
		speed = gameInstance->bullet_default_speed;
		dmg = gameInstance->bullet_default_dmg;
		tipoColision = (int)Game::CategoriaColision::Bala;
		colisionoCon = (int)Game::CategoriaColision::Enemigo;
		initialScale = gameInstance->bullet_default_scale;
		break;
	case tipoEnemy:
		pathSprite = gameInstance->bullet_enemy_path_sprite2.c_str();
		pathSonidoDisparo = gameInstance->bullet_path_sound_fire.c_str();
		pathSonidoImpacto = gameInstance->bullet_path_sound_impact.c_str();
		// Ojo!!!: las balas enemigas van a -velocidad
		speed = -gameInstance->bullet_default_speed;
		dmg = gameInstance->bullet_default_dmg;
		tipoColision = (int)Game::CategoriaColision::BalaEnemigo;
		colisionoCon = (int)Game::CategoriaColision::Jugador;
		initialScale = gameInstance->bullet_default_scale;
		break;
	case tipoBoss:
		pathSprite = gameInstance->bullet_enemy_path_sprite2.c_str();
		pathSonidoDisparo = gameInstance->bullet_path_sound_fire.c_str();
		pathSonidoImpacto = gameInstance->bullet_path_sound_impact.c_str();
		// Ojo!!!: las balas enemigas van a -velocidad
		speed = -gameInstance->bullet_default_speed;
		dmg = gameInstance->bullet_default_dmg;
		tipoColision = (int)Game::CategoriaColision::BalaEnemigo;
		colisionoCon = (int)Game::CategoriaColision::Jugador;
		initialScale = gameInstance->bullet_default_boss_scale;
		// TODO: Ñapa para probar distintos tipos de movimiento de balas
		delete claseMovimiento;
		claseMovimiento = new MueveHoming();
		
		
		break;
	default:
		CCLOG("Tipo bala desconocido: %d!", tipoBala);
		break;
	}

	tmp = new Bullet(nodo, bulletName, pathSprite, pathSonidoDisparo, pathSonidoImpacto, speed, dmg, tipoColision, colisionoCon, initialScale);
	tmp->funcionMovimientoActual = funcionMovimiento;
	
	//if(RandomHelper::random_int(1, 5) == 1){
	//	tmp->movimiento = new MueveHoming();
	//} else{
	//	tmp->movimiento = new MueveDireccion();
	//}
	
	//tmp->movimiento = new MueveDireccion();
	tmp->movimiento = claseMovimiento;
	tmp->movimiento->init(speed, 0, Vec2::ZERO, Vec2(0, 1.0f), Vec2::ZERO, Player::getCurrentPlayer()->getSprite());
	return tmp;
}

void Bullet::createBulletPool(Node *nodo, std::vector<Bullet *> &pool, int poolSize, tiposBala tipoBala) {
	Bullet *tmp;
	//const char *name;

	for(int i = 0; i < poolSize; i++){
		//tmp = creaBala(nodo, tipoBala, (name + std::to_string(i)).c_str());
		tmp = creaBala(nodo, tipoBala, ("balaTipo" + std::to_string(tipoBala) + "_" + std::to_string(i)).c_str());

		// TODO: que mas cosas hacer a la bala?

		pool.push_back(tmp);
	}


}

void Bullet::impacto(float dmg){
	// Sigo sorprendido que haciendo gameActorX->impacto(), sepa ejectura la función correcta :D

	// esta es facil, la bala desaparece
	desactiva();

	// TODO: no tan facil, el ttl!
	ttl = BULLET_HOMING_TTL;

	// TODO: reproducir sonido

	// TODO: animacion impacto
}


void Bullet::mueve(){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	
	Vec2 oldPos = getPosition();
	Vec2 nuPos = movimiento->mueve(oldPos);
	setPosition(nuPos);

	if((nuPos.x > visibleSize.width) || (nuPos.x < 0) || (nuPos.y<0) || (nuPos.y>visibleSize.height)){
		// se sale
		// TODO: Probando con balas dirigidas
		ttl = BULLET_HOMING_TTL;
		desactiva();
	}


	ttl -= Director::getInstance()->getDeltaTime();
	if(ttl <= 0){
		// TODO: Probando con balas dirigidas
		ttl = BULLET_HOMING_TTL;
		desactiva();
	}

	/*
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
	*/
}