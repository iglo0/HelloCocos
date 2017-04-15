#include "Weapon.h"

Weapon::Weapon(std::vector<Bullet *> &pool) : bulletPool(pool){

}

Weapon::~Weapon(){
	// TODO: Destroy bullet pool
}

void Weapon::fire(Vec2 posIni){
	// crea un disparo en la posición dada

	//Pool::activa(bulletPool, posIni);
}
