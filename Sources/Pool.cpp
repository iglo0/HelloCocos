#include "Pool.h"

// ----------------------------------------------------------------------
// OJOOO!
// ----------------------------------------------------------------------
// Variables est�ticas de una clase debo inicializarlas en el CPP. Como es un vector, se inicializa "solo", pero o lo declaro aqu� o GOLPE DE REMO (Linker error)
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

// version con par�metros para usar el mismo "pool" para toda bala que sea de un sprite "x" aunque tenga otras caracter�sticas como tama�o distinto
// tambi�n me parece mejor decidir aqu� con qu� "zOrder" pintarla, etc.
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

			// la muevo a pos. NOP -> ya lo hace su m�todo "activa"
			//tmp->setPosition(pos);

			// le asigno un padre nuevo if necessary
			if(nodoPadre){
				tmpSprite->removeFromParent();
				tmpSprite->setParent(nodoPadre);
			}

			// le asigno par�metros al sprite
			tmpSprite->setScale(initial_size);
			tmpSprite->setRotation(initial_rotation);
			tmpSprite->setZOrder(zOrder);

			// par�metros de la bala
			tmp->bulletDmg = dmg;


			// la activo (esto incluye la f�sica)
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
