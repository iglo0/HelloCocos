#include "Testz.h"

#include "Bala.h"

Weapon::Weapon(){
	ammo = nullptr;
	//Bala *b = new Bala("b", "pinyaun.png", (int)Game::CategoriaColision::Bala, 0, 33.0f);
}

Weapon::~Weapon(){

}

void Weapon::fire(Vec2 posIni){
	// crea un disparo en la posición dada

}