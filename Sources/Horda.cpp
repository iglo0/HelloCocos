#include "Horda.h"

// no me termino de hacer a esta sintaxis :>
Horda::Horda(Node *n, Vec2 posIni) : nodoPadre(n), coordenadasInicioHorda(posIni){}
	//// HACK: temporalmente mientras veo como estructurarlo...
	//Sprite *tmp = Sprite::create(enemigoT1PathSprite);

	//if(tmp != nullptr){
	//	dimensionesEnemigoT1 = tmp->getContentSize() * escalaEnemigoT1;
	//}

	//tmp = Sprite::create(enemigoT2PathSprite);
	//if(tmp != nullptr){
	//	dimensionesEnemigoT2 = tmp->getContentSize() * escalaEnemigoT2;
	//}
//}


void Horda::creaHorda(int dimX, int dimY){
	// quiero hacer algo con esto?

	dimensionesHordaX = dimX;
	dimensionesHordaY = dimY;

	for(int i = 0; i < dimY; i++){
		creaFila(dimX, Horda::tipoEnemigo::tipo1, i);
	}

	// con la horda creada... reasigno las posiciones

}

void Horda::creaFila(int cant, enum tipoEnemigo tipo, int numFila){
	std::vector <Enemigo *> fila;
	Enemigo *tmp;

	for(int i = 0; i < cant; i++){
		// TODO: configurar bien los desplazamientos
		tmp = creaEnemigo(tipo, i, numFila);

		//tmp->setPosition(coordenadasNaveEnXY(i, numFila));

		fila.push_back(tmp);
	}

	horda.push_back(fila);
}


Enemigo *Horda::creaEnemigo(enum tipoEnemigo t, int x, int y){

	Enemigo *tmp = new Enemigo;
	Sprite *spriteTmp;

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
		spriteTmp->setUserData(tmp);
		spriteTmp->setTag((int)Game::CategoriaColision::Enemigo);

		spriteTmp->setPosition(coordenadasNaveEnXY(x,y));

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

Vec2 Horda::coordenadasNaveEnXY(int x, int y){

	// TODO: sacar todos los cálculos que pueda y almacenarlos en variables, en vez de repetirlos con CADA nave
	// ahora estoy dándole vueltas

	if(x >= dimensionesHordaX || y >= dimensionesHordaY) {
		CCLOG("Te has pasado con las coordenadas");
		return nullptr;
	}
	cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
	float marginX = 200.f;
	float marginY = 200.f;

	// TODO: y si... en vez de repartir los enemigos por el ancho y alto, mejor repartirlos solo por el ancho, el alto que sea fijo?


	float originX = 0;	// posicion arbitraria de la x máxima para un sprite (para que no se corte por la izquierda). MEH posiblemente con el margen vale
	float originY = 50.f;	// posicion arbitraria de la y máxima para un sprite (para que no se corte por arriba). El margen NO vale porque no los quiero centrados verticalmente
	float sepX, sepY;

	separacionHordaX = (visibleSize.width - marginX) / dimensionesHordaX;
	//separacionHordaY = (visibleSize.height - marginY) / dimensionesHordaY;
	separacionHordaY = 100.f;

	// distribuye los enemigos en el ancho/alto de la pantalla menos los margenes

	Vec2 tmp = coordenadasInicioHorda;

	tmp.x += originX + marginX + separacionHordaX * x;
	// umm el margenY no lo uso para desplazar hacia abajo las naves enemigas
	// quiero que empiecen siempre desde arriba
	//tmp.y -= marginY + separacionHordaY * y;
	tmp.y -= originY + separacionHordaY * y;

	return tmp;

	//Enemigo *tmp;

	//// TODO: Esto funciona, verdad?
	//tmp = horda[x][y];
	//// oh yeah
	//return tmp->getPosition();

}