#include "Pool.h"

// ----------------------------------------------------------------------
// OJOOO!
// ----------------------------------------------------------------------
// Variables estáticas de una clase debo inicializarlas en el CPP. Como es un vector, se inicializa "solo", pero o lo declaro aquí o GOLPE DE REMO (Linker error)
// ojo a "Game::"
// Enemigos
std::vector<Enemy *> Pool::currentEnemies;
// Balas
std::vector<Bullet *> Pool::currentBulletsPlayerTipo1;
std::vector<Bullet *> Pool::currentBulletsTipo1;
std::vector<Bullet *> Pool::currentBulletsTipo2;
std::vector<Bullet *> Pool::currentBulletsTipoBoss;

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------



void Pool::activa(std::vector<Bullet *> &pool, Vec2 pos){

	for(auto x = pool.cbegin(); x != pool.cend(); ++x){
		if(!(*x)->isActive()){
			(*x)->activa(pos);
			break;
		}
	}

}

// version con parámetros para usar el mismo "pool" para toda bala que sea de un sprite "x" aunque tenga otras características como tamaño distinto
// también me parece mejor decidir aquí con qué "zOrder" pintarla, etc.
void Pool::activa(std::vector<Bullet *> &pool, Vec2 pos, Node *nodoPadre, float initial_size, float initial_rotation, float dmg, int zOrder){
	// busco una bala libre
	// TODO: ir pensando en disparos de varias a la vez tipo "trabuco"
	Bullet *tmp;
	Sprite *tmpSprite;

	for(auto x = pool.cbegin(); x != pool.cend(); ++x){
		if(!(*x)->isActive()){
			//(*x)->activa(pos);
			tmp = (*x);
			tmpSprite = tmp->getSprite();

			// una bala libre:

			// la muevo a pos. NOP -> ya lo hace su método "activa"
			//tmp->setPosition(pos);

			// le asigno un padre nuevo if necessary
			if(nodoPadre){
				tmpSprite->removeFromParent();
				tmpSprite->setParent(nodoPadre);
			}

			// le asigno parámetros al sprite
			tmpSprite->setScale(initial_size);
			tmpSprite->setRotation(initial_rotation);
			tmpSprite->setZOrder(zOrder);

			// parámetros de la bala
			tmp->bulletDmg = dmg;


			// la activo (esto incluye la física)
			tmp->activa(pos);

			break;
		}
	}


}

void Pool::desactiva(Bullet &actor){
	actor.desactiva();
}


void Pool::updateAll(float deltaT){
	// enemies
	for(auto x = currentEnemies.cbegin(); x != currentEnemies.cend(); ++x){
		if((*x)->isActive()){
			(*x)->update(deltaT);
		}
	}

	// bullets
	for(auto x = currentBulletsTipo1.cbegin(); x != currentBulletsTipo1.cend(); ++x){
		if((*x)->isActive()){
			(*x)->update(deltaT);
		}
	}
	for(auto x = currentBulletsTipo2.cbegin(); x != currentBulletsTipo2.cend(); ++x){
		if((*x)->isActive()){
			(*x)->update(deltaT);
		}
	}
}
