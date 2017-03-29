#include "Pool.h"

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


#pragma region poolOLD

PoolDEPRECATED::PoolDEPRECATED(){
}
//Pool::Pool(Node *nodo) : nodoPadre(nodo){}

PoolDEPRECATED::~PoolDEPRECATED(){

}

//bool Pool::creaPoolSprites(int cant, const char *spritePath, float scaleIni, float rotationIni, const char *name){
//	return creaPoolSprites(cant, spritePath, scaleIni, rotationIni, name, (int)Game::CategoriaColision::None, (int)Game::CategoriaColision::None, nullptr);
//}
//
//bool Pool::creaPoolSprites(int cant, const char *spritePath, float scale, float rotation, const char *name, int tipoColision, int colisionaCon, void *clasePadre){
//	Sprite *tmp;
//	
//	for(int i = 0; i < cant; i++){
//
//		// para sprites poligonales	
//		AutoPolygon ap = AutoPolygon(spritePath);
//		tmp = Sprite::create(ap.generateTriangles());
//		if(!tmp){
//			CCLOG("ORROR TERROR Y PAVOR: no pude crear el sprite: %s", spritePath);
//			return false;
//		}
//
//		// los sprites se crean invisibles por defecto
//		// y si tienen colisiones(física), desactivada
//
//		// algunos parámetros por defecto
//		//tmp = Sprite::create(spritePath);
//		tmp->setScale(scale);
//		tmp->setRotation(rotation);
//		tmp->setPosition(Vec2::ZERO);
//		tmp->setVisible(false);
//
//		// Tiene física? Me pongo con ello
//		if(tipoColision != (int)Game::CategoriaColision::None){
//			// con física
//			//Game::getInstance()->anadeFisica(tmp, tipoColision, colisionaCon, (name + std::to_string(i)).c_str());
//			Game::anadeFisica(tmp, tipoColision, colisionaCon, (name + std::to_string(i)).c_str());
//
//			// la detección de colisiones empieza desactivada
//			tmp->getPhysicsBody()->setEnabled(false);
//
//			//TODO: ya tengo para recuperar mis datos :)
//			tmp->setUserData(clasePadre);
//			// y su tipo
//			tmp->setTag(tipoColision);
//		} else{
//			// No? Le pongo el nombre igual
//			tmp->setName(name + std::to_string(i));
//		}
//
//		//nodoPadre->addChild(tmp, zOrder);
//
//		// hecho
//		pool.push_back(tmp);
//	}
//
//	return true;
//}
//


bool PoolDEPRECATED::creaPoolBalas(Node *nodo, int cant, const char *spritePath, float scale, float rotation, const char *name, float speed, const char *pathSonidoDisparo, const char *pathSonidoImpacto, int tipoColision, int colisionaCon){
	// pool de balas
	for(int i = 0; i < cant; i++){
		// TODO: la que estoy montando con la conversion de tipos entre string y const char *...
		BalaOLD *tmp = new BalaOLD(("Bala " + std::to_string(i)).c_str(), spritePath, tipoColision, colisionaCon);
		if(!tmp->getSprite()){
			CCLOG("ORROR, intentando definir balafisica sin sprite: %s", spritePath);
			return false;
		}

		tmp->getSprite()->setScale(scale);
		tmp->getSprite()->setVisible(false);
		// la velocidad varía la dirección (positiva o negativa)
		tmp->setVelocidad(speed);
		tmp->setSonido(BalaOLD::sonidosBala::disparo, pathSonidoDisparo);
		tmp->setSonido(BalaOLD::sonidosBala::impacto, pathSonidoImpacto);

		//pool.push_back(tmp);
		pool.push_back(tmp->getSprite());

		// cuelgo cada sprite del nodo actual
		// o no se mostrará nada
		// TODO: ummm no sé si debo destruirlos a mano o se encarga cocos
		nodo->addChild(tmp->getSprite());
	}

	return true;
}


void PoolDEPRECATED::desActiva(Sprite *sprite){
	sprite->setVisible(false);
	PhysicsBody *p = sprite->getPhysicsBody();
	if(p){
		p->setEnabled(false);
	}

	if(sprite->getParent() != nullptr){
		sprite->getParent()->removeChild(sprite);
	}
}

Sprite *PoolDEPRECATED::activa(Vec2 posRel, Sprite *spritePadre, int zOrder){
	if(!spritePadre){
		CCLOG ("Pool tiene que tener padre, saliendo");
		return nullptr;
	}

	for(auto tmp = pool.cbegin(); tmp != pool.cend(); ++tmp){
		if(!(*tmp)->isVisible()){
			// este me vale
			(*tmp)->setPosition(posRel + spritePadre->convertToNodeSpace(spritePadre->getPosition()));
			spritePadre->addChild(*tmp, zOrder);

			PhysicsBody *p = (*tmp)->getPhysicsBody();
			if(p){
				p->setEnabled(true);
			}

			(*tmp)->setVisible(true);

			return *tmp;
		}
	}

	return nullptr;
}
		
Sprite *PoolDEPRECATED::activa(Vec2 posAbs, Node *nodePadre, int zOrder){
	if(!nodePadre){
		CCLOG ("Pool tiene que tener padre, saliendo");
		return nullptr;
	}

	for(auto tmp = pool.cbegin(); tmp != pool.cend(); ++tmp){
		if(!(*tmp)->isVisible()){
			// este me vale
			(*tmp)->setPosition(posAbs);
			nodePadre->addChild(*tmp, zOrder);

			PhysicsBody *p = (*tmp)->getPhysicsBody();
			if(p){
				p->setEnabled(true);
			}

			(*tmp)->setVisible(true);

			return *tmp;
		}
	}

	return nullptr;

}


#pragma endregion

