#include "Horda.h"

// no me termino de hacer a esta sintaxis :>
Horda::Horda(Node *n, Vec2 posIni) : nodoPadre(n), posInicio(posIni) {
	// HACK: temporalmente mientras veo como estructurarlo...
	Sprite *tmp = Sprite::create(enemigoT1PathSprite);

	if(tmp != nullptr){
		dimensionesEnemigoT1 = tmp->getContentSize() * escalaEnemigoT1;
	}

	tmp = Sprite::create(enemigoT2PathSprite);
	if(tmp != nullptr){
		dimensionesEnemigoT2 = tmp->getContentSize() * escalaEnemigoT2;
	}

}

void Horda::creaFila(int cant, enum tipoEnemigo tipo, float coordY, float separacionX){
	std::vector <Enemigo *> fila;

	for(int i = 0; i < cant; i++){
		// TODO: configurar bien los desplazamientos
		fila.push_back(creaEnemigo(tipo, coordY, (float)i * separacionX));
	}
}

void Horda::creaHorda(int dimX, int dimY){
	// TODO: quiero hacer algo con esto?
}

Enemigo *Horda::creaEnemigo(enum tipoEnemigo t, float coordY, float desplX){

	Enemigo *tmp = new Enemigo;
	Sprite *spriteTmp;

	Vec2 pos = posInicio;

	// HACK: Otra cosa a mejorar
	if(t == tipoEnemigo::tipo1){
		pos.x += dimensionesEnemigoT1.x;
		pos.y -= dimensionesEnemigoT1.y + coordY;
	} else if(t == tipoEnemigo::tipo2){
		pos.x += dimensionesEnemigoT2.x;
		pos.y -= dimensionesEnemigoT2.y + coordY;
	}

	pos.x += desplX;
	// ojo que creaSprite YA añade el hijo al nodo y no hace falta hacerlo luego
	switch(t){
	case tipoEnemigo::tipo1:
		spriteTmp = tmp->creaSprite(nodoPadre, enemigoT1PathSprite, enemigoPathSonidoMuerte, escalaEnemigoT1, 0);
		break;
	case tipoEnemigo::tipo2:
		spriteTmp = tmp->creaSprite(nodoPadre, enemigoT2PathSprite, enemigoPathSonidoMuerte, escalaEnemigoT2, 0);
		break;
	default:
		break;
	}

	if(spriteTmp){
		//spriteTmp = tmp->getSprite();
		//spriteTmp->setName("Enemigo");
		spriteTmp->setUserData(tmp);
		spriteTmp->setTag((int)Game::CategoriaColision::Enemigo);

		spriteTmp->setPosition(pos);

		Game::getInstance()->anadeFisica(spriteTmp, (int)Game::CategoriaColision::Enemigo, (int)Game::CategoriaColision::Bala | (int)Game::CategoriaColision::Jugador, "Enemigo");

		// Jodó, si ya lo había añadido en creaSprite >.<
		// OJO con el Visual, aunque Cocos Asserts que el hijo ya está añadido, visual te dice la condicion CCASSERT(parent==nullptr) con lo que me liaba
		//nodo->addChild(spriteTmp);

		return tmp;
	} else{
		CCLOG("CASCOTE! Movida creando un enemigo de la horda!");
		return nullptr;
	}
}