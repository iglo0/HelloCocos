#include "SpaceInvaders.h"

#include "Pool.h"

#include "Movimiento.h"

USING_NS_CC;


// ojo!!!: con los statics
bool SpaceInvaders::spaceInvaderMovement_goingRight;		// pa donde tira
bool SpaceInvaders::spaceInvaderMovement_goingDown;			// pa donde tira
float SpaceInvaders::porcenInvadersVivos;
int SpaceInvaders::numInvadersInicial;
int SpaceInvaders::numInvadersVivos;

SpaceInvaders::SpaceInvaders(int tamaX, int tamaY, float comprX, float comprY, float margX, float margY) : dimMaxX(tamaX), dimMaxY(tamaY), compressX(comprX), compressY(comprY), marginX(margX), marginY(margY) {}

SpaceInvaders::~SpaceInvaders(){}

void SpaceInvaders::creaInvaders(Node *nodo, std::vector<Enemy::tiposEnemigo> &tipos, std::vector<Bullet *> &pool, float velMovHtal, float velMovVcal, float vcalMoveAmount,int probDisparoAleat){
	// esta versi�n "2" se comporta distinto
	// �En vez de un gestor de "Hordas"... no ser�a mejor que cada nave tenga su l�gica y se llame al update de cada una?
	// que cada una tenga su pool de balas tambi�n, pero puede ser una referencia al mismo pool
	// Para disparar -> Pool::activa(&pool, vec2 pos);

	auto visibleSize = Director::getInstance()->getVisibleSize();
	
	Enemy::tiposEnemigo tipo;
	Enemy *tmp;
	Vec2 enePos;

	for(size_t j = 0; j < dimMaxY; j++) {

		// coge el tipo que toca o repite el �ltimo ad infinitum
		if(j < tipos.size()){
			tipo = tipos[j];
		} else{
			tipo = tipos[tipos.size()-1];
		}

		for(size_t i = 0; i < dimMaxX; i++){

			// TODO: fijo a cada tipo de enemigo un tipo de disparo con el "tipo" as� que no necesito cambiarlo luego m�s abajo
			tmp = new Enemy(tipo);
			tmp->initEnemy(nodo);

			// Pruebo el nuevo Enemy:GameActor 
			// lo inicializo y le asigno un comportamiento
			enePos = devuelvePosicionInicial(i, j);
			tmp->activa(enePos);

			MueveSpaceInvader *m = new MueveSpaceInvader();
			// void init(float speedX, float speedY, float xMin, float xMax, float vcalMoveAmount, float vcalMoveCurrTarget);
			m->init(velMovHtal, velMovVcal, 50.0f, visibleSize.width - 50.0f, vcalMoveAmount, 0);
			
			tmp->movimiento_ = m;

			// C�mo querr� que se mueva?
			//tmp->funcionMovimientoActual = &GameActor::mueveSpaceInvader;

			// HACK: Prueba SpaceInvaders
			SpaceInvaders::spaceInvaderMovement_goingDown = false;
			SpaceInvaders::spaceInvaderMovement_goingRight = true;

			//tmp->spaceInvaderMovement_goingRight = true;
			//tmp->spaceInvaderMovement_goingDown = false;
			//tmp->spaceInvaderMovement_speedX = velMovHtal;
			//tmp->spaceInvaderMovement_speedY = velMovVcal;
			//tmp->spaceInvaderMovement_vcalMoveCurrTarget = 0;	// calculado luego
			//tmp->spaceInvaderMovement_vcalMoveAmount = vcalMoveAmount;

			// cada "space invader" se puede mover entre los limites de la pantalla, pero el primero que llegue avisa al resto para que cambien su movimiento
			// as� con cualquier configuraci�n de enemigos, siempre llegar�n hasta los l�mites de la pantalla
			
			// TODO: cambiar el margen manual de 50?
			//tmp->spaceInvaderMovement_xMax = visibleSize.width - 50.0f;
			//tmp->spaceInvaderMovement_xMin = 50.0f;

			// y que ataque?
			//tmp->funcionControlActual = &Enemy::funControl1;
			tmp->funcionControlActual_ = &Enemy::funControlFireRandom;
			//tmp->funcionControlTiempoDisparo = 1.0f;
			tmp->funcionControlProbDisparoAleatoria_ = probDisparoAleat;

			// TODO: esto lo quito porque cada tipo de enemigo ya sabr� su tipo de pool
			//tmp->poolMisBalas_ = &pool;

			// hale, definido

			// TODO: cada vez que cree un Enemy lo meto en el array de Pool
			// TODO: lo podr�a meter en el new de Enemy?
			Pool::currentEnemies.push_back(tmp);
		}
	}

	// OJO: �Statics!
	porcenInvadersVivos = 1.0f;
	numInvadersInicial = dimMaxX * dimMaxY;
	numInvadersVivos = numInvadersInicial;
}

Vec2 SpaceInvaders::devuelvePosicionInicial(int dimX, int dimY){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	float stepX, stepY;

	// TODO: permitir definirlo
	stepX = visibleSize.width * compressX / dimMaxX;
	stepY = visibleSize.height * compressY / dimMaxY;

	return Vec2(marginX + dimX * stepX, visibleSize.height - marginY - dimY * stepY);
}
