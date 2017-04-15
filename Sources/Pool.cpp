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
