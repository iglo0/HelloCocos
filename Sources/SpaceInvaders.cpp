#include "SpaceInvaders.h"

SpaceInvaders::SpaceInvaders(){}

SpaceInvaders::~SpaceInvaders(){}

void SpaceInvaders::creaInvaders(Node *nodo, int dimX, int dimY, std::vector<Bullet *> &pool, float velMovHtal, float velMovVcal, int probDisparoAleat, float dmg){
	// esta versión "2" se comporta distinto
	// ¿En vez de un gestor de "Hordas"... no sería mejor que cada nave tenga su lógica y se llame al update de cada una?
	// que cada una tenga su pool de balas también, pero puede ser una referencia al mismo pool
	// Para disparar -> Pool::activa(&pool, vec2 pos);

	auto visibleSize = Director::getInstance()->getVisibleSize();
	


	// TODO: aquí faltaría un nodo donde colgarlo, enemy speed, sonidos y supongo que más cosas
	// quizá incluso el nodo no es importante ahora, y solo en el activa()?
	//Enemy *tmp = new Enemy(nodo, ENEMY_T2_PATH_SPRITE, "", ENEMY_T2_INITIAL_SIZE, ENEMY_T2_INITIAL_ROTATION, ENEMY_GENERIC_HP);
	Enemy *tmp = new Enemy(nodo, ENEMY_T1_PATH_SPRITE, "", ENEMY_T1_INITIAL_SIZE, ENEMY_T1_INITIAL_ROTATION, ENEMY_GENERIC_HP);

	// Pruebo el nuevo Enemy:GameActor 
	// lo inicializo y le asigno un comportamiento
	// situo al enemigo arriba en el medio, con medio cuerpo de margen superior
	//Vec2 enePos = Vec2::ZERO;	//Vec2(visibleSize.width / 2.0f, visibleSize.height - enemy->getSprite()->getContentSize().height);
	Vec2 enePos = Vec2(visibleSize.width / 2.0f + 100.0f, visibleSize.height - 250.0f);
	tmp->activa(enePos);
	//tmp->weapon = new Weapon;
	//tmp->weapon->createBulletPool(nodo, 3, "balaEne_", BULLET_PATH_SPRITE2, BULLET_PATH_SOUND_FIRE, BULLET_PATH_SOUND_IMPACT, -BULLET_DEFAULT_SPEED, BULLET_DEFAULT_DMG,
	//	(int)Game::CategoriaColision::BalaEnemigo, (int)Game::CategoriaColision::Jugador, 3.0f);
	// TODO: uyuy esto casca fijo
	//tmp->weapon->bulletPool = pool;

	// Cómo querré que se mueva?
	//tmp->funcionMovimientoActual = &GameActor::mueveSeno;
	tmp->funcionMovimientoActual = &GameActor::mueveSpaceInvader;
	tmp->funcionMovimientoAmplitude = -1.0;
	tmp->funcionMovimientoPosIni = Vec2::ZERO;

	// y que ataque?
	tmp->funcionControlActual = &Enemy::funControl1;
	tmp->funcionControlTiempoDisparo = 0.1f;

	// hale, definido
	// TODO: cada vez que cree un Enemy lo meto en el array de Game
	// Y Si... lo hago en el new de cada Enemy ???
	//Game::getInstance()->ostras.push_back(tmp);


	// y ahora otro
	tmp = new Enemy(nodo, ENEMY_T2_PATH_SPRITE, "", ENEMY_T2_INITIAL_SIZE, ENEMY_T2_INITIAL_ROTATION, ENEMY_GENERIC_HP);

	// Pruebo el nuevo Enemy:GameActor 
	// lo inicializo y le asigno un comportamiento
	// situo al enemigo arriba en el medio, con medio cuerpo de margen superior
	//Vec2 enePos = Vec2::ZERO;	//Vec2(visibleSize.width / 2.0f, visibleSize.height - enemy->getSprite()->getContentSize().height);
	enePos = Vec2(visibleSize.width / 2.0f - 100.0f, visibleSize.height - 250.0f);
	tmp->activa(enePos);
	tmp->weapon = new Weapon(pool);
	//tmp->weapon->createBulletPool(nodo, 3, "balaEne_", BULLET_PATH_SPRITE2, BULLET_PATH_SOUND_FIRE, BULLET_PATH_SOUND_IMPACT, -BULLET_DEFAULT_SPEED, BULLET_DEFAULT_DMG,
	//	(int)Game::CategoriaColision::BalaEnemigo, (int)Game::CategoriaColision::Jugador, 3.0f);
	// TODO: uyuy esto casca fijo
	//tmp->weapon->bulletPool = pool;

	// Cómo querré que se mueva?
	//tmp->funcionMovimientoActual = &GameActor::mueveSeno;
	tmp->funcionMovimientoActual = &GameActor::mueveSpaceInvader;
	tmp->funcionMovimientoAmplitude = -1.0;
	tmp->funcionMovimientoPosIni = Vec2::ZERO;

	// y que ataque?
	tmp->funcionControlActual = &Enemy::funControl1;
	tmp->funcionControlTiempoDisparo = 0.1f;

	// TODO: cada vez que cree un Enemy lo meto en el array de Game
	//Game::getInstance()->ostras.push_back(tmp);

	//poolBalas = pool;
	
	//// TODO: parametros de comportamiento
	//velMovimientoHorizontal = velMovHtal;
	//velMovimientoVertical = velMovVcal;
	//probabilidadDisparoAleatoria = probDisparoAleat;

	//dimensionesHordaX = dimX;
	//dimensionesHordaY = dimY;

	//// TODO: pensandolo un poco mejor, no tienen por que ser arrays bidimensionales
	//// lo creo como si lo fuera, pero el resto de veces lo navego como una lista lineal
	//EnemigoOOOLD *tmp;
	//for(int y = 0; y < dimY; y++){
	//	for(int x = 0; x < dimX; x++){
	//		tmp = creaEnemigo(tipo2, x, y, dmg);
	//		horda.push_back(tmp);

	//		// lo asigna por cada fila... 
	//		// TODO: no se si es mas rapido repetir asignaciones de punteros o un par de comprobaciones más...
	//		if(x == 0){
	//			enemigoIzquierdo = tmp;
	//		}
	//		// en el caso particular de una horda de 1 también tiene que ejecutarse
	//		// así que no vale el if..else original
	//		if(x == dimX - 1){
	//			enemigoDerecho = tmp;
	//		}
	//	}
	//}
}
