#include "Pool.h"
#include "Bullet.h"
#include "Enemy.h"	// si no lo incluyo no puedo llamar a "enemy->activa(xxx)"
#include "Movimiento.h"
#include "Player.h"	// este tiene la player position

// ----------------------------------------------------------------------
// OJOOO!
// ----------------------------------------------------------------------
// Variables estáticas de una clase debo inicializarlas en el CPP. Como es un vector, se inicializa "solo", pero o lo declaro aquí o GOLPE DE REMO (Linker error)
// ojo a "Pool::" para indicar qué estoy inicializando
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
	bool foundOne = false;
	Bullet *tmp;

	for(auto x = pool.cbegin(); x != pool.cend(); ++x){
		tmp = *x;
		if(!tmp->isActive()){
			foundOne = true;
			tmp->activa(pos);
			//tmp->movimiento->init(tmp->getPosition(), Player::getCurrentPlayerPosition(), tmp->gameActorSpeed);
			
			////CCLOG("%s", typeid(m).name());	// este devuelve "class ClaseBase"
			//CCLOG("%s", typeid(*m).name());		// este es el bueno "class ClaseDerivada"
			//CCLOG("%s", typeid(*tmp->movimiento).name());
			
			//// OJO!!!: const char *c1 == const char *c2 compara las direcciones, para comparar contenido usar el *
			//if(*typeid(pos).name() ==*"class cocos2d::Vec2"){
			//	CCLOG("%s", "puta madre!");
			//} else{
			//	CCLOG("%s", "puta mierda");
			//}
			////CCLOG("%s", typeid(pos).name());
			
			tmp->movimiento->mueve(tmp->getPosition());
			break;
		}
	}

	if(!foundOne){
		CCLOG("Pool agotado!");
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

	// bullets...
	// ...del player
	for(auto x = currentBulletsPlayerTipo1.cbegin(); x != currentBulletsPlayerTipo1.cend(); ++x){
		if((*x)->isActive()){
			//(*x)->update(deltaT);
			(*x)->mueveBala();
		}
	}

	// ...de los bichos
	for(auto x = currentBulletsTipo1.cbegin(); x != currentBulletsTipo1.cend(); ++x){
		if((*x)->isActive()){
			//(*x)->update(deltaT);
			(*x)->mueveBala();
		}
	}
	for(auto x = currentBulletsTipo2.cbegin(); x != currentBulletsTipo2.cend(); ++x){
		if((*x)->isActive()){
			//(*x)->update(deltaT);
			(*x)->mueveBala();

		}
	}

}


void Pool::deletePools(){
	// TODO: Intento borrar los pools 

	deletePool<Enemy>(currentEnemies);
	deletePool<Bullet>(currentBulletsPlayerTipo1);
	deletePool<Bullet>(currentBulletsTipo1);
	deletePool<Bullet>(currentBulletsTipo2);
	deletePool<Bullet>(currentBulletsTipoBoss);
}

void Pool::disablePools(){

	//disablePool<Enemy>(currentEnemies);
	disablePool<Bullet>(currentBulletsPlayerTipo1);
	disablePool<Bullet>(currentBulletsTipo1);
	disablePool<Bullet>(currentBulletsTipo2);
	disablePool<Bullet>(currentBulletsTipoBoss);
}


template <>	// TODO: Es una pena que no pueda convertir simplemente un vector<GameActor> en vector<Enemy> o vector<Bullet>
void Pool::deletePool<Enemy>(std::vector<Enemy *> &v){
	for(auto x = v.cbegin(); x != v.cend(); ++x){
		(*x)->getSprite()->removeFromParent();
		delete (*x);
	}
	v.clear();
}

template<>
void Pool::deletePool<Bullet>(std::vector<Bullet *> &v){
	for(auto x = v.cbegin(); x != v.cend(); ++x){
		(*x)->getSprite()->removeFromParent();
		delete (*x);
	}
	v.clear();
}

template <>	// TODO: Es una pena que no pueda convertir simplemente un vector<GameActor> en vector<Enemy> o vector<Bullet>
void Pool::disablePool<Bullet>(std::vector<Bullet *> &v){
	for(auto x = v.cbegin(); x != v.cend(); ++x){
		(*x)->desactiva();
	}
}
