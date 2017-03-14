#include "Pool.h"

Pool::Pool(Node *nodo) : nodoPadre(nodo){}

Pool::~Pool(){

}


bool Pool::creaPoolSprites(int cant, const char *spritePath, int zOrder, float scale, float rotation, const char *name){
	return creaPoolSprites(cant, spritePath, zOrder, scale, rotation, name, (int)Game::CategoriaColision::None, (int)Game::CategoriaColision::None, nullptr);
}

bool Pool::creaPoolSprites(int cant, const char *spritePath, int zOrder, float scale, float rotation, const char *name, int tipoColision, int colisionaCon, void *clasePadre){
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
		// y si tienen colisiones(f�sica), desactivada

		// algunos par�metros por defecto
		//tmp = Sprite::create(spritePath);
		tmp->setScale(scale);
		tmp->setRotation(rotation);
		tmp->setPosition(Vec2::ZERO);
		tmp->setVisible(false);

		// Tiene f�sica? Me pongo con ello
		if(tipoColision != (int)Game::CategoriaColision::None){
			// con f�sica
			Game::getInstance()->anadeFisica(tmp, tipoColision, colisionaCon, (name + std::to_string(i)).c_str());

			// la detecci�n de colisiones empieza desactivada
			tmp->getPhysicsBody()->setEnabled(false);

			//TODO: ya tengo para recuperar mis datos :)
			tmp->setUserData(clasePadre);
			// y su tipo
			tmp->setTag(tipoColision);
		} else{
			// No? Le pongo el nombre igual
			tmp->setName(name + std::to_string(i));
		}

		nodoPadre->addChild(tmp, zOrder);

		// hecho
		pool.push_back(tmp);
	}

	return true;
}


void Pool::desActiva(Sprite *sprite){
	sprite->setVisible(false);
	PhysicsBody *p = sprite->getPhysicsBody();
	if(p){
		p->setEnabled(false);
	}
}

Sprite *Pool::activa(Vec2 pos){
	for(auto tmp = pool.cbegin(); tmp != pool.cend(); ++tmp){
		if(!(*tmp)->isVisible()){
			// este me vale

			(*tmp)->setPosition(pos);
			(*tmp)->setVisible(true);
			PhysicsBody *p = (*tmp)->getPhysicsBody();
			if(p){
				p->setEnabled(true);
			}

			return *tmp;
		}
	}

	return nullptr;
}
		
		