#include "Horda.h"

// no me termino de hacer a esta sintaxis :>
Horda::Horda(Node *n, Vec2 posIni) : nodoPadre(n), coordenadasInicioHorda(posIni){
	// inicializaciones varias
	nCiclosMovimientoHorizontal = 0;
	desplazamientoHorda = Vec2::ZERO;
}


void Horda::creaHorda(int dimX, int dimY, std::vector<Bala *> &pool){
	poolBalas = pool;

	dimensionesHordaX = dimX;
	dimensionesHordaY = dimY;

	for(int i = 0; i < dimY; i++){
		creaFila(dimX, Horda::tipoEnemigo::tipo2, i);
	}
}

void Horda::creaFila(int cant, enum tipoEnemigo tipo, int numFila){
	std::vector <Enemigo *> fila;
	Enemigo *tmp;

	for(int i = 0; i < cant; i++){
		// TODO: configurar bien los desplazamientos
		// cada enemigo se coloca a si mismo en pantalla según su fila y columna
		tmp = creaEnemigo(tipo, i, numFila);

		fila.push_back(tmp);
	}

	horda.push_back(fila);
}


Enemigo *Horda::creaEnemigo(enum tipoEnemigo t, int x, int y){

	Enemigo *tmp = new Enemigo;
	Sprite *spriteTmp;

	// ojo que creaSprite YA añade el hijo al nodo y no hace falta hacerlo luego
	// TODO: de momento cada bicho dispara el mismo tipo de bala
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

		spriteTmp->setPosition(coordenadasInicialesNaveEnXY(x,y));

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
	Enemigo *tmp = enemigoEnXY(x, y);

	// este activo o no
	return tmp->getPosition();
}

Enemigo *Horda::enemigoEnXY(int x, int y){
	return horda[y][x];

}



Vec2 Horda::coordenadasInicialesNaveEnXY(int x, int y){

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
void Horda::tick(){
	float deltaT = Director::getInstance()->getDeltaTime();

	mueve();

	// disparo o que?
	//dispara(); // !
	if(random() < probabilidadDisparoAleatoria){
		//CCLOG("PINYAUN!");
		dispara();
	}
}



void Horda::mueve(){
	float deltaT = Director::getInstance()->getDeltaTime();

	if(moverALaIzquierda){
		desplazamientoHorda = -deltaT * velMovimientoHorizontal * Vec2(1.f, 0);
		//CCLOG("Despl: %f", desplazamientoHorda.x);
		if(cambiarDireccion()){
			nCiclosMovimientoHorizontal++;
			moverALaIzquierda = false;
		}
	} else {
		desplazamientoHorda = deltaT * velMovimientoHorizontal * Vec2(1.f, 0);
		//CCLOG("Despl: %f", desplazamientoHorda.x);
		if(cambiarDireccion()){
			moverALaIzquierda = true;
		}
	}

	// por cada fila de la horda
	// y por cada enemigo de la fila, los voy moviendo
	int x;
	int y = 0;
	for(auto fila = horda.cbegin(); fila != horda.cend(); ++fila,y++){
		x = 0;
		for(auto enemigo = (*fila).cbegin(); enemigo != (*fila).cend(); ++enemigo, x++){
			(*enemigo)->setPosition(coordenadasNaveEnXY(x, y) + desplazamientoHorda);
			//(*enemigo)->mueveRelativo(desplazamientoHorda);
		}
	}
}

bool Horda::cambiarDireccion(){
	// decide si toca cambiar de direccion

	// según si se mueve a izquierda o derecha compruebo el borde correspondiente
	Vec2 posEnemigoUltimo;
	float bordeDerechoX = Director::getInstance()->getVisibleSize().width;

	if(moverALaIzquierda){
		// por ejemplo 
		posEnemigoUltimo = coordenadasNaveEnXY(0, 0);
		if(posEnemigoUltimo.x < 0){
			return true;
		}
	} else{
		posEnemigoUltimo = coordenadasNaveEnXY(dimensionesHordaX-1, 0);
		if(posEnemigoUltimo.x > bordeDerechoX){
			return true;
		}
	}

	return false;

}


void Horda::dispara(){
	// oki, disparo pero... quien?
	// TODO: Comprobar que el enemigo existe, antes :>
	int dispX, dispY;
	bool encontradoUnoVivo = true;
	dispX = RandomHelper::random_int(0, dimensionesHordaX - 1);
	dispY = RandomHelper::random_int(0, dimensionesHordaY - 1);

	//// dis motherfucker
	//Enemigo *tmp = naveEnXY(dispX, dispY);

	//// TODO: y si está desactivado,qué?
	//// pues... voy a empezar no disparando y a ver qué tal queda
	//if(!tmp->estaActivo())
	//	return;
	//// ok. Queda horrible, al final no dispara nadie.
	//// tendré que buscar de otro modo


	// v2
	// =================================
	// =================================
	// =================================
	// TODO: HE CREADO UN MONSTRUO
	// =================================
	// =================================
	// =================================
	// Además que está mal, cuando empiezan a faltar muchos enemigos, las balas se van casi todas al primero

	// Tendré que buscar el primero libre de otra forma

	Enemigo *tmp;
	int x, y;

	// empiezo el circo de buscar uno activo
	// por hacer algo, recorro desde el x,y random hasta el final del array
	// y vuelvo a empezar desde 0,0 hasta el x,y random
	x = dispX;
	y = dispY;

	tmp = enemigoEnXY(x, y);
	while(!tmp->estaActivo())  {
		// vaya hombre, el que tocaba no está vivo

		// busco el siguiente

		if(++x >= dimensionesHordaX){
			x = 0;
			y++;
		}
		if(y >= dimensionesHordaY){
			y = 0;
			x = 0;
		}

		CCLOG("(x,y)=(%d,%d)", x, y);

		// condicion extra de salida, cuando vuelvo a llegar al enemigo inicial, salgo derrotado
		if(x == dispX && y == dispY){
			encontradoUnoVivo = false;
			break;
		}

		tmp = enemigoEnXY(x, y);

	}


	if(!encontradoUnoVivo){
		// TODO: WTF, la horda no debería existir, bug, bug!
		CCLOG("WTF, la horda no debería existir, bug, bug!");
		return;
	}

	Vec2 pos = tmp->getPosition();

	for(auto b = poolBalas.cbegin(); b != poolBalas.cend(); ++b){
		if(!(*b)->isActive()){
			// uso esta bala

			//Vec2 pos = sprite->getPosition();
			// la pongo un poco más abajo (+ hacia abajo)
			pos.y += 20.0f;

			(*b)->activar(pos);
			break;
		}
	}


}

void Horda::baja(){
}


