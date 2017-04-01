#include "SpaceInvaders.h"



#pragma region Deprecated

// no me termino de hacer a esta sintaxis :>
HordaDEPRECATED::HordaDEPRECATED(Node *n, Vec2 posIni) : nodoPadre(n), coordenadasInicioHorda(posIni){
	// inicializaciones varias
	nCiclosMovimientoHorizontal = 0;
	desplazamientoHorda = Vec2::ZERO;
}


HordaDEPRECATED::~HordaDEPRECATED(){
	CCLOG("Horda says: Bye bye!");

	// TODO: de hecho no debería borrar los enemigos sino hacer otro pool con ellos y activarlos/desactivarlos
	for(auto ene = horda.cbegin(); ene != horda.cend(); ++ene){
		if(nodoPadre){
			nodoPadre->removeChild((*ene)->getSprite());
		}

		delete (*ene);
	}

	// del pool de balas me encargaría en la clase Level1
}


void HordaDEPRECATED::creaHorda(int dimX, int dimY, std::vector<BalaOLD *> &pool, float velMovHtal, float velMovVcal, int probDisparoAleat, float dmg){
	poolBalas = pool;

	// TODO: parametros de comportamiento
	velMovimientoHorizontal = velMovHtal;
	velMovimientoVertical = velMovVcal;
	probabilidadDisparoAleatoria = probDisparoAleat;

	dimensionesHordaX = dimX;
	dimensionesHordaY = dimY;

	// TODO: pensandolo un poco mejor, no tienen por que ser arrays bidimensionales
	// lo creo como si lo fuera, pero el resto de veces lo navego como una lista lineal
	EnemigoOOOLD *tmp;
	for(int y = 0; y < dimY; y++){
		for(int x = 0; x < dimX; x++){
			tmp = creaEnemigo(tipo2, x, y, dmg);
			horda.push_back(tmp);

			// lo asigna por cada fila... 
			// TODO: no se si es mas rapido repetir asignaciones de punteros o un par de comprobaciones más...
			if(x == 0){
				enemigoIzquierdo = tmp;
			} 
			// en el caso particular de una horda de 1 también tiene que ejecutarse
			// así que no vale el if..else original
			if(x == dimX - 1){
				enemigoDerecho = tmp;
			}
		}
	}

}

EnemigoOOOLD *HordaDEPRECATED::creaEnemigo(enum tipoEnemigo t, int x, int y, float vida){

	EnemigoOOOLD *tmp = new EnemigoOOOLD;
	Sprite *spriteTmp;

	// ojo que creaSprite YA añade el hijo al nodo y no hace falta hacerlo luego
	// TODO: de momento cada bicho dispara el mismo tipo de bala
	switch(t){
	case tipoEnemigo::tipo1:
		spriteTmp = tmp->creaSprite(nodoPadre, enemigoT1PathSprite, enemigoPathSonidoMuerte, escalaEnemigoT1, 0, vida);
		break;
	case tipoEnemigo::tipo2:
		spriteTmp = tmp->creaSprite(nodoPadre, enemigoT2PathSprite, enemigoPathSonidoMuerte, escalaEnemigoT2, 0, vida);
		break;
	default:
		break;
	}

	if(spriteTmp){
		spriteTmp->setUserData(tmp);
		spriteTmp->setTag((int)Game::CategoriaColision::Enemigo);

		spriteTmp->setPosition(coordenadasInicialesNaveEnXY(x,y));

		//Game::getInstance()->anadeFisica(spriteTmp, (int)Game::CategoriaColision::Enemigo, (int)Game::CategoriaColision::Bala | (int)Game::CategoriaColision::Jugador, "Enemigo");
		Game::anadeFisica(spriteTmp, (int)Game::CategoriaColision::Enemigo, (int)Game::CategoriaColision::Bala | (int)Game::CategoriaColision::Jugador, "Enemigo");

		// Jodó, si ya lo había añadido en creaSprite >.<
		// OJO con el Visual, aunque Cocos Asserts que el hijo ya está añadido, visual te dice la condicion CCASSERT(parent==nullptr) con lo que me liaba
		//nodo->addChild(spriteTmp);

		return tmp;
	} else{
		CCLOG("CASCOTE! Movida creando un enemigo de la horda!");
		return nullptr;
	}
}

Vec2 HordaDEPRECATED::coordenadasInicialesNaveEnXY(int x, int y){

	// TODO: sacar todos los cálculos que pueda y almacenarlos en variables, en vez de repetirlos con CADA nave?

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
	//float sepX, sepY;

	separacionHordaX = (visibleSize.width - marginX) / dimensionesHordaX;
	//separacionHordaY = (visibleSize.height - marginY) / dimensionesHordaY;
	separacionHordaY = 100.f;

	// distribuye los enemigos en el ancho/alto de la pantalla menos los margenes

	Vec2 tmp = coordenadasInicioHorda;

	tmp.x += originX + marginX + separacionHordaX * x + desplazamientoHorda.x;
	// umm el margenY no lo uso para desplazar hacia abajo las naves enemigas
	// quiero que empiecen siempre desde arriba
	//tmp.y -= marginY + separacionHordaY * y;
	tmp.y -= originY + separacionHordaY * y + desplazamientoHorda.y;

	return tmp;

	//Enemigo *tmp;

	//// TODO: Esto funciona, verdad?
	//tmp = horda[x][y];
	//// oh yeah
	//return tmp->getPosition();

}


// un ciclo
void HordaDEPRECATED::tick(){
	float deltaT = Director::getInstance()->getDeltaTime();

	mueve();

	// TODO: esto mejor llevarlo a cada Enemigo creo yo
	// disparo o que?
	//dispara(); // !
	if(random() < probabilidadDisparoAleatoria){
		//CCLOG("PINYAUN!");
		dispara();
	}
}



void HordaDEPRECATED::mueve(){
	float deltaT = Director::getInstance()->getDeltaTime();

	if(moverALaIzquierda){
		desplazamientoHorda = -deltaT * velMovimientoHorizontal * Vec2(1.f, 0);
		if(cambiarDireccion()){
			nCiclosMovimientoHorizontal++;
			moverALaIzquierda = false;
		}
	} else {
		desplazamientoHorda = deltaT * velMovimientoHorizontal * Vec2(1.f, 0);
		if(cambiarDireccion()){
			moverALaIzquierda = true;
		}
	}

	for(auto ene = horda.cbegin(); ene != horda.cend(); ++ene){
		(*ene)->mueveRelativo(desplazamientoHorda);
	}

}

bool HordaDEPRECATED::cambiarDireccion(){
	// decide si toca cambiar de direccion

	// según si se mueve a izquierda o derecha compruebo el borde correspondiente
	Vec2 posEnemigoUltimo;
	float bordeDerechoX = Director::getInstance()->getVisibleSize().width;

	if(moverALaIzquierda){
		if(enemigoIzquierdo->getPosition().x < 0){
			return true;
		}

	} else{
		if(enemigoDerecho->getPosition().x > bordeDerechoX){
			return true;
		}
	}

	return false;
}

std::vector<EnemigoOOOLD *> HordaDEPRECATED::listaEnemigosVivos(){
	std::vector<EnemigoOOOLD *> tmp;

	for(auto ene = horda.cbegin(); ene != horda.cend(); ++ene){
		if((*ene)->estaActivo()){
			tmp.push_back(*ene);
		}
	}

	if(tmp.cbegin() == tmp.cend()){
		CCLOG("HORDA DESTRUIDA!");
		Game::getInstance()->estadoActual = Game::finHorda;
	}

	return tmp;
}


void HordaDEPRECATED::dispara(){
	std::vector<EnemigoOOOLD *> enemigosQueQuedan;

	enemigosQueQuedan = listaEnemigosVivos();

	int iMax = (int)enemigosQueQuedan.size()-1;

	if(iMax < 0){
		// si no quedan enemigos
		// tendría que parar en algún sitio, supongo que no debería llegar aquí
		CCLOG ("intento de disparar sin enemigos");
		return;
	}

	// dispara, pero quien?
	int i = RandomHelper::random_int(0, iMax);
	Vec2 posBala = enemigosQueQuedan[i]->getPosition();
	posBala.y += 20.f;

	for(auto b = poolBalas.cbegin(); b != poolBalas.cend(); ++b){
		if(!(*b)->isActive()){
			(*b)->activar(posBala);
			break;
		}
	}
}

void HordaDEPRECATED::baja(){
	CCLOG("Movimiento hacia abajo por implementar");
}


#pragma endregion