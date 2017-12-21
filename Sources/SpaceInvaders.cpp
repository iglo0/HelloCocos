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

SpaceInvaders::SpaceInvaders(int tamaX, int tamaY, float comprX, float comprY, float margX, float margY) : dimMaxX_(tamaX), dimMaxY_(tamaY), compressX_(comprX), compressY_(comprY), marginX_(margX), marginY_(margY) {}

SpaceInvaders::SpaceInvaders(){
	dimMaxX_ = 0;
	dimMaxY_ = 0;
	compressX_ = 0;
	compressY_ = 0;
	marginX_ = 0;
	marginY_ = 0;
}


SpaceInvaders::~SpaceInvaders(){}

// Probabilidad: mas disparos cuanto más baja
void SpaceInvaders::creaInvaders(Node *nodo, std::vector<Enemy::tiposEnemigo> &tipos, float velMovHtal, float velMovVcal, float vcalMoveAmount, int probDisparoAleat){
	Enemy::tiposEnemigo tipo;
	Enemy *tmp;
	Vec2 enePos;
	auto visibleSize = Director::getInstance()->getVisibleSize();

	for(size_t j = 0; j < dimMaxY_; j++) {

		// coge el tipo que toca o repite el último ad infinitum
		if(j < tipos.size()){
			tipo = tipos[j];
		} else{
			tipo = tipos[tipos.size()-1];
		}

		for(size_t i = 0; i < dimMaxX_; i++){

			// TODO: fijo a cada tipo de enemigo un tipo de disparo con el "tipo" así que no necesito cambiarlo luego más abajo
			//tmp = new Enemy(tipo);
			//tmp->initEnemy(nodo);

			tmp = Enemy::createEnemy(nodo, "honesto");

			// Pruebo el nuevo Enemy:GameActor 
			// lo inicializo y le asigno un comportamiento
			enePos = devuelvePosicionInicial(i, j);
			tmp->activa(enePos);

			MueveSpaceInvader *m = new MueveSpaceInvader();
			// void init(float speedX, float speedY, float xMin, float xMax, float vcalMoveAmount, float vcalMoveCurrTarget);
			m->init(velMovHtal, velMovVcal, 50.0f, visibleSize.width - 50.0f, vcalMoveAmount, 0);
			
			tmp->movimiento_ = m;

			// Cómo querré que se mueva?
			//tmp->funcionMovimientoActual = &GameActor::mueveSpaceInvader;

			// HACK: Prueba SpaceInvaders
			SpaceInvaders::spaceInvaderMovement_goingDown = false;
			SpaceInvaders::spaceInvaderMovement_goingRight = true;

			// cada "space invader" se puede mover entre los limites de la pantalla, pero el primero que llegue avisa al resto para que cambien su movimiento
			// así con cualquier configuración de enemigos, siempre llegarán hasta los límites de la pantalla
			
			// y que ataque?
			tmp->funcionControlActual_ = &Enemy::funControlFireRandom;
			tmp->funcionControlProbDisparoAleatoria_ = probDisparoAleat;

			// hale, definido
			Pool::currentEnemies.push_back(tmp);
		}
	}

	// OJO: ¡Statics!
	porcenInvadersVivos = 1.0f;
	numInvadersInicial = dimMaxX_ * dimMaxY_;
	numInvadersVivos = numInvadersInicial;
}

Vec2 SpaceInvaders::devuelvePosicionInicial(int dimX, int dimY){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	float stepX, stepY;

	// TODO: permitir definirlo
	stepX = visibleSize.width * compressX_ / dimMaxX_;
	stepY = visibleSize.height * compressY_ / dimMaxY_;

	return Vec2(marginX_ + dimX * stepX, visibleSize.height - marginY_ - dimY * stepY);
}
