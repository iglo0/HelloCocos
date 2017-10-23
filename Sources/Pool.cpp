#include "Pool.h"
#include "Bullet.h"
#include "Enemy.h"	// si no lo incluyo no puedo llamar a "enemy->activa(xxx)"
#include "Movimiento.h"
#include "Player.h"	// este tiene la player position

#include "AnimSprites.h"

// ----------------------------------------------------------------------
// OJOOO!
// ----------------------------------------------------------------------
// Variables estáticas de una clase debo inicializarlas en el CPP. Como es un vector, se inicializa "solo", pero o lo declaro aquí o GOLPE DE REMO (Linker error)
// ojo a "Pool::" para indicar qué estoy inicializando
// Enemigos
std::vector<Enemy *> Pool::currentEnemies;
// Balas
std::vector<Bullet *> Pool::currentBulletsPlayerTipo1;
std::vector<Bullet *> Pool::currentBulletsTipoNormal;
std::vector<Bullet *> Pool::currentBulletsTipo2;
std::vector<Bullet *> Pool::currentBulletsTipoBossHoming;

std::vector<GameActor *> Pool::currentExplosions;
std::vector<GameActor *> Pool::currentImpacts;



// ----------------------------------------------------------------------
// PRUEBAS --------------------------------------------------------------
// ----------------------------------------------------------------------
//
//MiSprite::MiSprite(){
//	sprite = nullptr;
//	inUse = false;
//}
//
//MiSprite::~MiSprite(){
//	// Sprite es autorelease así que supongo que se arregla solito
//}
//
//MiSprite *MiSprite::createWithSpriteFrameName(const std::string &spriteFrameName){
//	MiSprite *tmp = new MiSprite();
//
//	tmp->sprite = Sprite::createWithSpriteFrameName(spriteFrameName);
//	tmp->desActiva();
//
//	return tmp;
//}
//
//// activa y mueve
//void MiSprite::activa(Vec2 p){
//	inUse = true;
//
//	if(sprite){
//		sprite->setPosition(p);
//		sprite->setVisible(true);
//		auto pb = sprite->getPhysicsBody();
//
//		if(pb){
//			pb->setEnabled(true);
//		}
//	}
//}
//
//// solo activa
//void MiSprite::activa(){
//	inUse = true;
//
//	if(sprite){
//		sprite->setVisible(true);
//		auto pb = sprite->getPhysicsBody();
//
//		if(pb){
//			pb->setEnabled(true);
//		}
//	}
//}
//
//
//void MiSprite::desActiva(){
//	inUse = false;
//
//	if(sprite){
//		sprite->setVisible(false);
//		auto pb = sprite->getPhysicsBody();
//
//		if(pb){
//			pb->setEnabled(false);
//		}
//	}
//}


// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------



Bullet *Pool::activa(std::vector<Bullet *> &pool, Vec2 pos){
	bool foundOne = false;
	Bullet *tmp;

	for(auto x = pool.cbegin(); x != pool.cend(); ++x){
		tmp = *x;
		if(!tmp->isActive()){
			foundOne = true;
			tmp->activa(pos);
			
			////CCLOG("%s", typeid(m).name());	// este devuelve "class ClaseBase"
			//CCLOG("%s", typeid(*m).name());		// este es el bueno "class ClaseDerivada"
			//CCLOG("%s", typeid(*tmp->movimiento).name());
			
			//// OJO!!!: const char *c1 == const char *c2 compara las direcciones, para comparar contenido usar el *
			//if(*typeid(pos).name() ==*"class cocos2d::Vec2"){

			return tmp;
			break;
		}
	}

	if(!foundOne){
		CCLOG("Pool agotado!");
	}

	return nullptr;
}

void Pool::desactiva(Bullet &actor){
	actor.desactiva();
}

GameActor *Pool::activa(std::vector<GameActor *> &pool, Vec2 pos){
	bool foundOne = false;
	GameActor *tmp;

	for(auto x = pool.cbegin(); x != pool.cend(); ++x){
		tmp = *x;
		if(!tmp->isActive()){
			foundOne = true;
			tmp->activa(pos);

			////CCLOG("%s", typeid(m).name());	// este devuelve "class ClaseBase"
			//CCLOG("%s", typeid(*m).name());		// este es el bueno "class ClaseDerivada"
			//CCLOG("%s", typeid(*tmp->movimiento).name());

			//// OJO!!!: const char *c1 == const char *c2 compara las direcciones, para comparar contenido usar el *
			//if(*typeid(pos).name() ==*"class cocos2d::Vec2"){

			return tmp;
			//break;
		}
	}

	if(!foundOne){
		CCLOG("Pool agotado!");
	}

	return nullptr;
}

void Pool::desactiva(GameActor &actor){
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
			(*x)->update(deltaT);
			//(*x)->mueveBala();
		}
	}

	// ...de los bichos
	for(auto x = currentBulletsTipoNormal.cbegin(); x != currentBulletsTipoNormal.cend(); ++x){
		if((*x)->isActive()){
			(*x)->update(deltaT);
			//(*x)->mueveBala();
		}
	}
	for(auto x = currentBulletsTipo2.cbegin(); x != currentBulletsTipo2.cend(); ++x){
		if((*x)->isActive()){
			(*x)->update(deltaT);
			//(*x)->mueveBala();

		}
	}

	// ...del boss
	for(auto x = currentBulletsTipoBossHoming.cbegin(); x != currentBulletsTipoBossHoming.cend(); ++x){
		if((*x)->isActive()){
			(*x)->update(deltaT);
			//(*x)->mueveBala();
		}
	}

	// ...y varios
	for(auto x = currentExplosions.cbegin(); x != currentExplosions.cend(); ++x){
		if((*x)->isActive()){
			(*x)->update(deltaT);
			//(*x)->mueveBala();
		}
	}
}


void Pool::deletePools(){
	// TODO: Intento borrar los pools 

	deletePool<Enemy>(currentEnemies);
	deletePool<Bullet>(currentBulletsPlayerTipo1);
	deletePool<Bullet>(currentBulletsTipoNormal);
	deletePool<Bullet>(currentBulletsTipo2);
	deletePool<Bullet>(currentBulletsTipoBossHoming);
	deletePool<GameActor>(currentExplosions);
	deletePool<GameActor>(currentImpacts);
}

void Pool::disablePools(){

	//disablePool<Enemy>(currentEnemies);
	disablePool<Bullet>(currentBulletsPlayerTipo1);
	disablePool<Bullet>(currentBulletsTipoNormal);
	disablePool<Bullet>(currentBulletsTipo2);
	disablePool<Bullet>(currentBulletsTipoBossHoming);
	disablePool<GameActor>(currentExplosions);
	disablePool<GameActor>(currentImpacts);
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

template<>
void Pool::deletePool<GameActor>(std::vector<GameActor *> &v){
	GameActor *gameActor;
	//Sprite *sprite;
	//AnimSprites *animSprites;

	for(auto x = v.cbegin(); x != v.cend(); ++x){
		gameActor = (*x);
		//sprite = gameActor->getSprite();
		//animSprites = gameActor->getAnimSprites();
		//if(sprite){
		//	sprite->removeFromParent();
		//}
		//if(animSprites){
		//	// me parece que el destructor se encarga de todo	
		//}
		// me parece que el destructor se encarga de todo	
		delete gameActor;
	}
	v.clear();
}



template <>	// TODO: Es una pena que no pueda convertir simplemente un vector<GameActor> en vector<Enemy> o vector<Bullet>
void Pool::disablePool<Bullet>(std::vector<Bullet *> &v){
	for(auto x = v.cbegin(); x != v.cend(); ++x){
		(*x)->desactiva();
	}
}


template <>
void Pool::disablePool<GameActor>(std::vector<GameActor *> &v){
	for(auto x = v.cbegin(); x != v.cend(); ++x){
		(*x)->desactiva();
	}
}

