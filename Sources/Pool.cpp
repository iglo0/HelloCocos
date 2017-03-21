#include "Pool.h"

Pool::Pool(){
}
//Pool::Pool(Node *nodo) : nodoPadre(nodo){}

Pool::~Pool(){

}

bool Pool::creaPoolSprites(int cant, const char *spritePath, float scaleIni, float rotationIni, const char *name){
	return creaPoolSprites(cant, spritePath, scaleIni, rotationIni, name, (int)Game::CategoriaColision::None, (int)Game::CategoriaColision::None, nullptr);
}

bool Pool::creaPoolSprites(int cant, const char *spritePath, float scale, float rotation, const char *name, int tipoColision, int colisionaCon, void *clasePadre){
	Sprite *tmp;
	
	for(int i = 0; i < cant; i++){

		// para sprites poligonales	
		AutoPolygon ap = AutoPolygon(spritePath);
		tmp = Sprite::create(ap.generateTriangles());
		if(!tmp){
			CCLOG("ORROR TERROR Y PAVOR: no pude crear el sprite: %s", spritePath);
			return false;
		}

		// los sprites se crean invisibles por defecto
		// y si tienen colisiones(física), desactivada

		// algunos parámetros por defecto
		//tmp = Sprite::create(spritePath);
		tmp->setScale(scale);
		tmp->setRotation(rotation);
		tmp->setPosition(Vec2::ZERO);
		tmp->setVisible(false);

		// Tiene física? Me pongo con ello
		if(tipoColision != (int)Game::CategoriaColision::None){
			// con física
			//Game::getInstance()->anadeFisica(tmp, tipoColision, colisionaCon, (name + std::to_string(i)).c_str());
			Game::anadeFisica(tmp, tipoColision, colisionaCon, (name + std::to_string(i)).c_str());

			// la detección de colisiones empieza desactivada
			tmp->getPhysicsBody()->setEnabled(false);

			//TODO: ya tengo para recuperar mis datos :)
			tmp->setUserData(clasePadre);
			// y su tipo
			tmp->setTag(tipoColision);
		} else{
			// No? Le pongo el nombre igual
			tmp->setName(name + std::to_string(i));
		}

		//nodoPadre->addChild(tmp, zOrder);

		// hecho
		pool.push_back(tmp);
	}

	return true;
}


//template <typename T>
//void Pool::test(T x){
//	x = x;
//}
//

void Pool::desActiva(Sprite *sprite){
	sprite->setVisible(false);
	PhysicsBody *p = sprite->getPhysicsBody();
	if(p){
		p->setEnabled(false);
	}

	if(sprite->getParent() != nullptr){
		sprite->getParent()->removeChild(sprite);
	}
}

Sprite *Pool::activa(Vec2 posRel, Sprite *spritePadre, int zOrder){
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
		
Sprite *Pool::activa(Vec2 posAbs, Node *nodePadre, int zOrder){
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