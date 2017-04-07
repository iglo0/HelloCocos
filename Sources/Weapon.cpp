#include "Weapon.h"
#include "Bala.h"

Weapon::Weapon(){
	//ammo = nullptr;
	//Bala *b = new Bala("b", "pinyaun.png", (int)Game::CategoriaColision::Bala, 0, 33.0f);
	
}

Weapon::~Weapon(){
	// TODO: Destroy bullet pool
}

void Weapon::fire(Vec2 posIni){
	// crea un disparo en la posición dada

	Pool::activa(bulletPool, posIni);
}

void Weapon::createBulletPool(Node *nodo, int poolSize, const char *baseName, const char *pathSprite, const char *pathSonidoDisparo, const char *pathSonidoImpacto, float speed, float dmg, int tipoColision, int colisionoCon, float initialScale){
	
	Bullet::createBulletPool(nodo, bulletPool, poolSize,baseName, pathSprite,pathSonidoDisparo,pathSonidoImpacto,speed,dmg,tipoColision,colisionoCon, initialScale);

}
