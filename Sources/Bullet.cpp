#include "Bullet.h"

#include "Game.h"

#include "Movimiento.h"
#include "Player.h"

USING_NS_CC;

Bullet::Bullet(Node *nodo, const char *name, const char *pathSprite, const char *pathSonidoDisparo, const char *pathSonidoImpacto, float speed, float dmg, int tipoColision, int colisionoCon, float initialScale){
	// inicializa la clase base primero
	GameActor::GameActor();
	GameActor::type_ = GameActor::gameActorTypes::bullet;

	//bulletSpeed = speed;
	bulletDmg = dmg;
	gameActorSpeed_ = speed;
	
	//CCLOG("creando bala: %s", name);
	//if(!createBullet(nodo, pathSprite, name, tipoColision, colisionoCon)){
	if(!GameActor::setSprite(nodo, pathSprite,name,tipoColision, colisionoCon,initialScale)){
		CCLOG("No pude crear bala %s", pathSprite);
	}

	_ttl = BULLET_HOMING_TTL;
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

Bullet *Bullet::creaBala(Node *nodo, bulletTypes tipoBala, const char *bulletName){
	Movimiento *claseMovimiento;
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
		tipoColision = (int)Game::CategoriaColision::BalaJugador;
		colisionoCon = (int)Game::CategoriaColision::Enemigo | (int)Game::CategoriaColision::BalaEnemigo | (int)Game::CategoriaColision::Destructible;
		initialScale = gameInstance->bullet_default_scale;

		claseMovimiento = new MueveVcal(speed);

		break;
	case tipoEnemyDirigido:
		pathSprite = gameInstance->bullet_enemy_path_sprite2.c_str();
		pathSonidoDisparo = gameInstance->bullet_path_sound_fire.c_str();
		pathSonidoImpacto = gameInstance->bullet_path_sound_impact.c_str();
		// Ojo!!!: las balas enemigas van a -velocidad
		speed = -gameInstance->bullet_default_speed;
		dmg = gameInstance->bullet_default_dmg;
		tipoColision = (int)Game::CategoriaColision::BalaEnemigo;
		colisionoCon = (int)Game::CategoriaColision::Jugador | (int)Game::CategoriaColision::BalaJugador | (int)Game::CategoriaColision::Destructible;
		initialScale = gameInstance->bullet_default_scale;

		claseMovimiento = new MueveDireccion(speed);
		break;
	case tipoEnemyNormal:
		pathSprite = gameInstance->bullet_enemy_path_sprite1.c_str();
		pathSonidoDisparo = gameInstance->bullet_path_sound_fire.c_str();
		pathSonidoImpacto = gameInstance->bullet_path_sound_impact.c_str();
		// Ojo!!!: las balas enemigas van a -velocidad
		speed = -gameInstance->bullet_default_speed;
		dmg = gameInstance->bullet_default_dmg;
		tipoColision = (int)Game::CategoriaColision::BalaEnemigo;
		colisionoCon = (int)Game::CategoriaColision::Jugador | (int)Game::CategoriaColision::BalaJugador | (int)Game::CategoriaColision::Destructible;
		initialScale = gameInstance->bullet_default_scale;

		claseMovimiento = new MueveVcal(speed);

		break;
	case tipoBossHoming:
		pathSprite = gameInstance->bullet_enemy_path_sprite1.c_str();
		pathSonidoDisparo = gameInstance->bullet_path_sound_fire.c_str();
		pathSonidoImpacto = gameInstance->bullet_path_sound_impact.c_str();
		// Ojo!!!: las balas enemigas van a -velocidad
		speed = -gameInstance->bullet_homing_speed;
		dmg = gameInstance->bullet_default_dmg;
		tipoColision = (int)Game::CategoriaColision::BalaEnemigo;
		colisionoCon = (int)Game::CategoriaColision::Jugador | (int)Game::CategoriaColision::BalaJugador | (int)Game::CategoriaColision::Destructible;
		initialScale = gameInstance->bullet_default_boss_scale;

		claseMovimiento = new MueveHoming();

		break;
	default:
		CCLOG("Tipo bala desconocido: %d!", tipoBala);
		break;
	}

	tmp = new Bullet(nodo, bulletName, pathSprite, pathSonidoDisparo, pathSonidoImpacto, speed, dmg, tipoColision, colisionoCon, initialScale);

	tmp->_bulletType = tipoBala;

	tmp->movimiento_ = claseMovimiento;

	return tmp;
}

void Bullet::createBulletPool(Node *nodo, std::vector<Bullet *> &pool, int poolSize, bulletTypes tipoBala) {
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
	// Sigo sorprendido que haciendo gameActorX->impacto(), sepa ejectura la funci�n correcta :D

	// esta es facil, la bala desaparece
	desactiva();

	// TODO: no tan facil, el ttl!
	// TODO: que hace esta linea aqui O.o
	_ttl = BULLET_HOMING_TTL;

	// TODO: reproducir sonido

	// TODO: animacion impacto
}


void Bullet::mueveBala(){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	
	Vec2 oldPos = getPosition();
	Vec2 nuPos = movimiento_->mueve(oldPos);
	setPosition(nuPos);

	if((nuPos.x > visibleSize.width) || (nuPos.x < 0) || (nuPos.y<0) || (nuPos.y>visibleSize.height)){
		// se sale
		// TODO: Probando con balas dirigidas
		_ttl = BULLET_HOMING_TTL;
		desactiva();
	}

	_ttl -= Director::getInstance()->getDeltaTime();
	if(_ttl <= 0){
		// TODO: Probando con balas dirigidas
		_ttl = BULLET_HOMING_TTL;
		desactiva();
	}
}

void Bullet::activa(Vec2 posIni){
	GameActor::activa(posIni);

	// OJO: la diferencia entre activar un GameActor Enemy o Bullet (p.ej.) es que los enemigos a fecha de hoy hacen siempre lo mismo, pero una bala dirigida tiene que
	// poder cambiar el target cuando se activa, y cuando vaya a�adiendo comportamientos ser� m�s probable que tenga que inicializarlas distinto

	// y luego...
	switch(_bulletType){
	case bulletTypes::tipoBossHoming:
	{	// si voy a inicializar variables en el case, o las inicializo en el default, o lo meto entre llaves... o utilizo funciones dentro del case, para dejarlo todo m�s limpio.
		//CCLOG("Activando bala tipoBossHoming");
		
		MueveHoming *m = (MueveHoming *)movimiento_;
		m->init(gameActorSpeed_, Player::getCurrentPlayer()->getSprite());
		
		break;
	}
	case bulletTypes::tipoEnemyDirigido:
	{	// si voy a inicializar variables en el case, o las inicializo en el default, o lo meto entre llaves... o utilizo funciones dentro del case, para dejarlo todo m�s limpio.
		//CCLOG("Activando bala tipoEnemyDirigido");
		
		// Accedo a la clase movimiento que DEBE ser de tipo MueveDireccion
		MueveDireccion *m = (MueveDireccion *)movimiento_;
		// y la inicializo para esta instancia que quiero crear
		m->init(getPosition(), Player::getCurrentPlayerPosition());

		break;
	}
	case bulletTypes::tipoEnemyNormal:
		//CCLOG("Activando bala tipoEnemyNormal");
		break;
	case bulletTypes::tipoPlayer:
		//CCLOG("Activando bala tipoPlayer");
		// no hay que cambiarle par�metros, solo necesitan la velocidad que es fija
		//MueveVcal *b = (MueveVcal *)movimiento;
		//b->init(gameActorSpeed);
		break;
	default:
		CCLOG("Activando bala (desconocida) %d", _bulletType);
		break;
	}

}