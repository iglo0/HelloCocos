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
			enePos = devuelvePosicionInicial(i, j);
			tmp->activa(enePos);

			// Cómo querré que se mueva?
			tmp->funcionMovimientoActual = &GameActor::mueveSpaceInvader;

			tmp->spaceInvaderMovement_goingRight = true;
			tmp->spaceInvaderMovement_goingDown = false;
			tmp->spaceInvaderMovement_speedX = velMovHtal;
			tmp->spaceInvaderMovement_speedY = velMovVcal;
			tmp->spaceInvaderMovement_vcalMoveCurrTarget = 0;	// calculado luego
			tmp->spaceInvaderMovement_vcalMoveAmount = 60.0f;
			// TODO: calcular los límites de movimiento correctamente (puestos a huevo)
			tmp->spaceInvaderMovement_xMax = devuelvePosicionInicial(i, j).x + Director::getInstance()->getVisibleSize().width/2.5f - marginX;
			tmp->spaceInvaderMovement_xMin = devuelvePosicionInicial(i, j).x;

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
