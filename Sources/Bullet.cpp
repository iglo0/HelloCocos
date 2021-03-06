#include "Bullet.h"

#include "Game.h"

#include "Movimiento.h"
#include "Player.h"

#include "AnimSprites.h"
#include "XmlHelper.h"

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
	if(pathSprite != ""){
		if(!GameActor::setSprite(nodo, pathSprite, name, tipoColision, colisionoCon, initialScale)){
			CCLOG("No pude crear bala %s", pathSprite);
		}
	}

	//ttl_ = BULLET_HOMING_TTL;
	ttl_ = -1.0f;
	//static_assert(L'??' == 0x1F4A9, "Compiler lacks Unicode support");
	//animSprites_ = nullptr;

	// TODO: hory shit
	//XmlHelper *xh = new XmlHelper();
	//animSprites_ = xh->loadAnimation(nodo, "mibala");
}

Bullet::Bullet(Node *nodo){
	// inicializa la clase base primero
	GameActor::GameActor();
	GameActor::type_ = GameActor::gameActorTypes::bullet;

	ttl_ = -1.0f;
}

Bullet::~Bullet(){
}

// (OJO) static
int Bullet::devuelveTipoPorNombre(const char *bType){
	// strcmp devuelve 0 si igual
	if(!strcmp(bType, "tipoPlayer")){
		return bulletTypes::tipoPlayer;
	} else if(!strcmp(bType, "tipoEnemyNormal")){
		return bulletTypes::tipoEnemyNormal;
	} else if(!strcmp(bType, "tipoEnemyDirigido")){
		return bulletTypes::tipoEnemyDirigido;
	} else if(!strcmp(bType, "tipoBossHoming")){
		return bulletTypes::tipoBossHoming;
	} else if(!strcmp(bType, "")){
		CCLOG("Tipo movimiento vacio!");
		return -1;
	}

	CCLOG("tipo movimiento desconocido: '%s'", bType);
	return -1;

	//tipoPlayer, tipoEnemyNormal, tipoEnemyDirigido, tipoBossHoming};

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

void Bullet::createBulletPool(Node *nodo, std::vector<Bullet *> &pool, int poolSize, bulletTypes tipoBala, const char *xmlDefName){
	Bullet *tmp;
	for(int i = 0; i < poolSize; i++){
		tmp = creaBala(nodo, xmlDefName, (std::string(xmlDefName) + std::to_string(i)).c_str());
		pool.push_back(tmp);
	}
}

// creaBala se llama desde mi propio createBulletPool
//Bullet *Bullet::creaBala(Node *nodo, bulletTypes tipoBala, const char *bulletName){
//	Movimiento *claseMovimiento;
//	Bullet *tmp;
//	Game *gameInstance = Game::getInstance();
//	AnimSprites *animS;
//	XmlHelper *xh = nullptr;
//
//	// los parametros de la bala
//	const char *pathSprite;
//	const char *pathSonidoDisparo;
//	const char *pathSonidoImpacto;
//	float speed;
//	float dmg;
//	int tipoColision;
//	int colisionoCon;
//	float initialScale;
//	
//	switch(tipoBala){
//	case tipoPlayer:
//		pathSprite = gameInstance->bullet_player_path_sprite1.c_str();
//		pathSonidoDisparo = gameInstance->bullet_path_sound_fire.c_str();
//		pathSonidoImpacto = gameInstance->bullet_path_sound_impact.c_str();
//		speed = gameInstance->bullet_default_speed;
//		dmg = gameInstance->bullet_default_dmg;
//		tipoColision = (int)Game::CategoriaColision::BalaJugador;
//		colisionoCon = (int)Game::CategoriaColision::Enemigo | (int)Game::CategoriaColision::BalaEnemigo | (int)Game::CategoriaColision::Destructible;
//		initialScale = gameInstance->bullet_default_scale;
//
//		claseMovimiento = new MueveVcal(speed);
//
//		// TODO: sistema de animacion sin configurar
//		animS = nullptr;
//
//		break;
//	case tipoEnemyDirigido:
//		pathSprite = gameInstance->bullet_enemy_path_sprite2.c_str();
//		pathSonidoDisparo = gameInstance->bullet_path_sound_fire.c_str();
//		pathSonidoImpacto = gameInstance->bullet_path_sound_impact.c_str();
//		// Ojo!!!: las balas enemigas van a -velocidad
//		speed = -gameInstance->bullet_default_speed;
//		dmg = gameInstance->bullet_default_dmg;
//		tipoColision = (int)Game::CategoriaColision::BalaEnemigo;
//		colisionoCon = (int)Game::CategoriaColision::Jugador | (int)Game::CategoriaColision::BalaJugador | (int)Game::CategoriaColision::Destructible;
//		initialScale = gameInstance->bullet_default_scale;
//
//		claseMovimiento = new MueveDireccion(speed);
//
//		// TODO: sistema de animacion sin configurar
//		//xh = new XmlHelper();
//		animS = xh->loadAnimation(nodo, "balaDirigida");
//		//animS = nullptr;
//
//		break;
//	case tipoEnemyNormal:
//		pathSprite = gameInstance->bullet_enemy_path_sprite1.c_str();
//		pathSonidoDisparo = gameInstance->bullet_path_sound_fire.c_str();
//		pathSonidoImpacto = gameInstance->bullet_path_sound_impact.c_str();
//		// Ojo!!!: las balas enemigas van a -velocidad
//		speed = -gameInstance->bullet_default_speed;
//		dmg = gameInstance->bullet_default_dmg;
//		tipoColision = (int)Game::CategoriaColision::BalaEnemigo;
//		colisionoCon = (int)Game::CategoriaColision::Jugador | (int)Game::CategoriaColision::BalaJugador | (int)Game::CategoriaColision::Destructible;
//		initialScale = gameInstance->bullet_default_scale;
//
//		claseMovimiento = new MueveVcal(speed);
//
//		// TODO: sistema de animacion sin configurar
//		xh = new XmlHelper();
//		animS = xh->loadAnimation(nodo, "balaEnemiga");
//		//animS = nullptr;
//
//		break;
//	case tipoBossHoming:
//		pathSprite = gameInstance->bullet_enemy_path_sprite1.c_str();
//		pathSonidoDisparo = gameInstance->bullet_path_sound_fire.c_str();
//		pathSonidoImpacto = gameInstance->bullet_path_sound_impact.c_str();
//		// Ojo!!!: las balas enemigas van a -velocidad
//		speed = -gameInstance->bullet_homing_speed;
//		dmg = gameInstance->bullet_default_dmg;
//		tipoColision = (int)Game::CategoriaColision::BalaEnemigo;
//		colisionoCon = (int)Game::CategoriaColision::Jugador | (int)Game::CategoriaColision::BalaJugador | (int)Game::CategoriaColision::Destructible;
//		initialScale = gameInstance->bullet_default_boss_scale;
//
//		claseMovimiento = new MueveHoming();
//
//		// TODO: sistema de animacion sin configurar
//		xh = new XmlHelper();
//		animS = xh->loadAnimation(nodo, "balaHoming");
//
//		break;
//	default:
//		CCLOG("Tipo bala desconocido: %d!", tipoBala);
//		animS = nullptr;
//		break;
//	}
//	
//	//// TODO: Nuevo -> con xml
//	//// funciona :)
//	//if(bulletDef != ""){
//	//	xh = new XmlHelper();
//	//	tmp = xh->loadBullet(nodo, bulletDef);
//	//	return tmp;
//	//}
//	//// ---------------------------------------------------------------------------
//
//	tmp = new Bullet(nodo, bulletName, pathSprite, pathSonidoDisparo, pathSonidoImpacto, speed, dmg, tipoColision, colisionoCon, initialScale);
//	tmp->bulletType_ = tipoBala;
//	tmp->movimiento_ = claseMovimiento;
//
//	if(xh){
//		xh->assignPhysicsToAnimation(animS, tmp, tipoColision, colisionoCon);
//	}
//
//	tmp->animSprites_ = animS;
//
//	return tmp;
//}

 // creaBala se llama desde mi propio createBulletPool
Bullet *Bullet::creaBala(Node *nodo, const char *bulletDef, const char *bulletName){
	Bullet *tmp;
	XmlHelper *xh;

	// TODO: Nuevo -> con xml
	// funciona :)
	xh = new XmlHelper();
	tmp = xh->loadBullet(nodo, bulletDef, bulletName);
	return tmp;
}

void Bullet::impacto(float dmg){
	// Sigo sorprendido que haciendo gameActorX->impacto(), sepa ejectura la funci�n correcta :D

	// esta es facil, la bala desaparece
	desactiva();

	// TODO: no tan facil, el ttl!
	// TODO: que hace esta linea aqui O.o
	//ttl_ = BULLET_HOMING_TTL;

	// TODO: reproducir sonido

	// TODO: animacion impacto
}

void Bullet::update(float deltaT){
	auto visibleSize = Director::getInstance()->getVisibleSize();

	// actualiza la posici�n del sprite y del animsprites
	Vec2 oldPos = getPosition();
	Vec2 nuPos = movimiento_->mueve(oldPos, deltaT);
	setPosition(nuPos);
	// -----------------------------------------------

	if((nuPos.x > visibleSize.width) || (nuPos.x < 0) || (nuPos.y<0) || (nuPos.y>visibleSize.height)){
		// se sale
		// TODO: Probando con balas dirigidas
		//ttl_ = BULLET_HOMING_TTL;
		desactiva();
	}

	if(ttl_ > 0){
		if(Game::getInstance()->ellapsedTime >= tEnd_){
			desactiva();
		}
	}

	//ttl_ -= Director::getInstance()->getDeltaTime();
	//if(ttl_ <= 0){
	//	// TODO: Probando con balas dirigidas
	//	ttl_ = BULLET_HOMING_TTL;
	//	desactiva();
	//}

	if(animSprites_){
		// TODO: ajusta posicion de la animacion
		animSprites_->update(deltaT);
		animSprites_->setPosition(nuPos);
	}

}

void Bullet::activa(Vec2 posIni){
	GameActor::activa(posIni);

	// OJO: la diferencia entre activar un GameActor Enemy o Bullet (p.ej.) es que los enemigos a fecha de hoy hacen siempre lo mismo, pero una bala dirigida tiene que
	// poder cambiar el target cuando se activa, y cuando vaya a�adiendo comportamientos ser� m�s probable que tenga que inicializarlas distinto

	// y luego...
	switch(bulletType_){
	case bulletTypes::tipoBossHoming:
	{	// si voy a inicializar variables en el case, o las inicializo en el default, o lo meto entre llaves... o utilizo funciones dentro del case, para dejarlo todo m�s limpio.
		//CCLOG("Activando bala tipoBossHoming");
		
		MueveHoming *m = (MueveHoming *)movimiento_;
		// TODO: ooops! �qu� pasa si intento seguir a una animaci�n?
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
		break;
	default:
		CCLOG("Activando bala (desconocida) %d", bulletType_);
		break;
	}
}

void Bullet::activa(float x, float y){
	activa(Vec2(x, y));
}

void Bullet::setType(bulletTypes t){
	bulletType_ = t;
}