#include "SpaceInvaders.h"

SpaceInvaders::SpaceInvaders(int tamaX, int tamaY, float comprX, float comprY, float margX, float margY) : dimMaxX(tamaX), dimMaxY(tamaY), compressX(comprX), compressY(comprY), marginX(margX), marginY(margY) {}

SpaceInvaders::~SpaceInvaders(){}

void SpaceInvaders::creaInvaders(Node *nodo, std::vector<Enemy::tiposEnemigo> &tipos, std::vector<Bullet *> &pool, float velMovHtal, float velMovVcal, int probDisparoAleat){
	// esta versión "2" se comporta distinto
	// ¿En vez de un gestor de "Hordas"... no sería mejor que cada nave tenga su lógica y se llame al update de cada una?
	// que cada una tenga su pool de balas también, pero puede ser una referencia al mismo pool
	// Para disparar -> Pool::activa(&pool, vec2 pos);

	auto visibleSize = Director::getInstance()->getVisibleSize();
	
	Enemy::tiposEnemigo tipo;
	Enemy *tmp;
	Vec2 enePos;

	for(int j = 0; j < dimMaxY; j++) {

		// TODO: Probar! coge el tipo que toca o repite el último ad infinitum
		if(j < tipos.size()){
			tipo = tipos[j];
		} else{
			tipo = tipos[tipos.size()-1];
		}

		for(int i = 0; i < dimMaxX; i++){

			tmp = new Enemy(nodo, tipo);

			// Pruebo el nuevo Enemy:GameActor 
			// lo inicializo y le asigno un comportamiento
			// situo al enemigo arriba en el medio, con medio cuerpo de margen superior
			//Vec2 enePos = Vec2::ZERO;	//Vec2(visibleSize.width / 2.0f, visibleSize.height - enemy->getSprite()->getContentSize().height);
			//enePos = Vec2(visibleSize.width / 2.0f + 100.0f, visibleSize.height - 250.0f);
			enePos = devuelvePosicionInicial(i, j);
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
			//tmp->funcionControlActual = &Enemy::funControl1;
			tmp->funcionControlActual = &Enemy::funControlFireRandom;
			//tmp->funcionControlTiempoDisparo = 1.0f;
			tmp->funcionControlProbDisparoAleatoria = probDisparoAleat;

			tmp->poolMisBalas = &pool;

			// hale, definido


			// TODO: cada vez que cree un Enemy lo meto en el array de Pool
			// TODO: lo podría meter en el new de Enemy?
			Pool::currentEnemies.push_back(tmp);


		}
	}
}

Vec2 SpaceInvaders::devuelvePosicionInicial(int dimX, int dimY){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	float stepX, stepY;

	// TODO: permitir definirlo
	stepX = visibleSize.width * compressX / dimMaxX;
	stepY = visibleSize.height * compressY / dimMaxY;

	return Vec2(marginX + dimX * stepX, visibleSize.height - marginY - dimY * stepY);
}
