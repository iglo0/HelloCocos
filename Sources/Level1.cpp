#include "Level1.h"


// Ojo! 
// Este USING aquí tiene que estar en Level1.h
// Si espero aquí a ponerlo, no compila pero no marca error tampoco en el .h
//USING_NS_CC;

Scene* Level1::createScene() {
    // 'scene' is an autorelease object
    //auto scene = Scene::create();


	// NUEVO: físicas!
	// Gracias a las físicas obtengo colisiones "gratis", y como un juego tipo space invaders 
	// no creo que tenga muchos problemas de rendimiento... tiraré por ahí a ver
	// ----------------------------------------------------------------------------------------------------------------------------------------
	// INI del motor de físicas Chipmunk
	auto scene = Scene::createWithPhysics();

	// set the world’s gravity to zero in both directions, which essentially disables gravity
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
	// enable debug drawing to see your physics bodies
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	// ----------------------------------------------------------------------------------------------------------------------------------------

	// 'layer' is an autorelease object
    auto layer = Level1::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

Level1::~Level1(){
	Director::getInstance()->setDisplayStats(false);
}

// on "init" you need to initialize your instance
bool Level1::init() {
	
	// ----------------------------------------------------------------------------------------------------------------------------------------
	// inits
	// ----------------------------------------------------------------------------------------------------------------------------------------
	if ( !Layer::init() ) {
        return false;
    }

	gameInstance = Game::getInstance();

    //auto visibleSize = Director::getInstance()->getVisibleSize();
	visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

	// ----------------------------------------------------------------------------------------------------------------------------------------
	// eventos
	// ----------------------------------------------------------------------------------------------------------------------------------------
	// input listeners
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(Level1::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(Level1::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	// contact listener
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Level1::onContactBegin, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

	// ----------------------------------------------------------------------------------------------------------------------------------------
	// GUI
	// ----------------------------------------------------------------------------------------------------------------------------------------

	// Botón de vuelta atrás
	auto vuelveAtras = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(Level1::menuVuelveCallback, this));
	vuelveAtras->setPosition(Vec2(origin.x + visibleSize.width - vuelveAtras->getContentSize().width / 2, origin.y + vuelveAtras->getContentSize().height / 2));
	// create menu, it's an autorelease object
	auto menu = Menu::create(vuelveAtras, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	lblMensajes = Label::create(mensajeIntro, "fonts/Marker Felt.ttf", 48.0, cocos2d::Size::ZERO, cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
	lblMensajes->setPosition(origin.x + visibleSize.width / 2.0f, origin.y + visibleSize.height / 2.0f);
	lblMensajes->setVisible(false);
	this->addChild(lblMensajes, 1, "lblMensajes");

	// ----------------------------------------------------------------------------------------------------------------------------------------
	// otras inicializaciones
	// ----------------------------------------------------------------------------------------------------------------------------------------

	mueveAbj = mueveArr = mueveDch = mueveIzq = false;
	gameInstance->ellapsedTime = 0;

	// crea al jugador y lo añade a la escena
	player = new Jugador(2.0f);
	// TODO: Ojo que creaSpriteFisicas antes devolvía bool y ahora devuelve Sprite *
	// En cualquier caso la comparacion if(!...) es lo mismo. Pero ojo.
	if( !player->creaSpriteFisicas(this, (int)Game::CategoriaColision::Jugador, (int)Game::CategoriaColision::Enemigo | (int)Game::CategoriaColision::BalaEnemigo) ){
		// si esto falla, apaga y vámonos
		sale = true;
	}

	// nota: estaba "precargando" el sonido por cada bala en el pool
	precargaSonidosDelNivel();

	// HACK: Esto también es temporal, de momento para pruebas
	// tendrá que evolucionar, ¿tendría que hacer un pool por cada tipo de sprite que pueda aparecer en el nivel?
	creaPoolBalasFisica(&poolBalas, 8, "bullet_2_blue.png", "sonidos/shoot.wav", "sonidos/fastinvader1.wav", 1.0f, balaSpeed, (int)Game::CategoriaColision::Bala,(int)Game::CategoriaColision::Enemigo);
	creaPoolBalasFisica(&poolBalasEnemigas, 32, "bullet_orange0000.png", "sonidos/shoot.wav", "sonidos/fastinvader1.wav", 1.0f, balaEnemigaSpeed, (int)Game::CategoriaColision::BalaEnemigo, (int)Game::CategoriaColision::Jugador);
	// TODO: temp. Balas inofensivas
	//creaPoolBalasFisica(&poolBalasEnemigas, 320, "bullet_orange0000.png", "sonidos/shoot.wav", "sonidos/fastinvader1.wav", 1.0f, balaEnemigaSpeed, (int)Game::CategoriaColision::BalaEnemigo, (int)Game::CategoriaColision::None);
	creaPoolBalasFisica(&poolBalasGordas, 5, "bullet_orange0000.png", "sonidos/shoot.wav", "sonidos/fastinvader1.wav", 4.0f, balaEnemigaSpeed, (int)Game::CategoriaColision::BalaEnemigo, (int)Game::CategoriaColision::Jugador);


	// TODO: esto tiene que evolucionar hacia un sistema por oleadas. De momento es para pruebas
	//creaEnemigos();

	// HACK: Otra temporalidad a falta de darle una vuelta. Indica el estado inicial de esta escena
	Game::getInstance()->estadoActual = Game::estadosJuego::introNivel;

	// variable para saber si tengo que resetear o no el tiempo transcurrido en el temporizador. Lo utilizo para cambiar de estados, por ejemplo.
	iniciaTemporizadorCambioEstado = true;


	// schedules update every frame with default order 0. Lower orders execute first
	this->scheduleUpdate();
	// displays fps
	Director::getInstance()->setDisplayStats(true);

	return true;
}

void Level1::precargaSonidosDelNivel() {
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sonidos/shoot.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sonidos/invaderkilled.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sonidos/explosion.wav");
}

// crea un pool de balas. El que lo haga tendrá que saber el tipo y demás.
void Level1::creaPoolBalas(std::vector<Bala *> *pool, int cant, const char *pathSpriteBala, const char *pathSonidoDisparo, const char *pathSonidoImpacto, float scale, float speed){
	// pool de balas
	for(int i = 0; i < cant; i++){
		Bala *tmp = new Bala(pathSpriteBala);
		tmp->getSprite()->setScale(scale);
		tmp->getSprite()->setVisible(false);
		tmp->setVelocidad(speed);
		tmp->setSonido(Bala::sonidosBala::disparo, pathSonidoDisparo);
		tmp->setSonido(Bala::sonidosBala::impacto, pathSonidoImpacto);

		pool->push_back(tmp);

		// cuelgo cada sprite del nodo actual
		// o no se mostrará nada
		// TODO: ummm no sé si debo destruirlos a mano o se encarga cocos
		addChild(tmp->getSprite());
	}
}

// crea un pool de balas con física para las colisiones. El que lo haga tendrá que saber el tipo y demás.
void Level1::creaPoolBalasFisica(std::vector<Bala *> *pool, int cant, const char *pathSpriteBala, const char *pathSonidoDisparo, const char *pathSonidoImpacto, float scale, float speed, int tipoColision, int colisionaCon){
	// pool de balas
	for(int i = 0; i < cant; i++){
		// TODO: la que estoy montando con la conversion de tipos entre string y const char *...
		Bala *tmp = new Bala(("Bala " + std::to_string(i)).c_str(), pathSpriteBala, tipoColision, colisionaCon);
		if(!tmp->getSprite()){
			CCLOG("ORROR, intentando definir balafisica sin sprite: %s", pathSpriteBala);
			return;
		}

		tmp->getSprite()->setScale(scale);
		tmp->getSprite()->setVisible(false);
		// la velocidad varía la dirección (positiva o negativa)
		tmp->setVelocidad(speed);
		tmp->setSonido(Bala::sonidosBala::disparo, pathSonidoDisparo);
		tmp->setSonido(Bala::sonidosBala::impacto, pathSonidoImpacto);

		pool->push_back(tmp);

		// cuelgo cada sprite del nodo actual
		// o no se mostrará nada
		// TODO: ummm no sé si debo destruirlos a mano o se encarga cocos
		addChild(tmp->getSprite());
	}
}

void Level1::menuVuelveCallback(Ref *pSender){
	// vuelve al menu
	//auto scene = Menus::CreateScene();
	//auto director = Director::getInstance();
	//director->replaceScene(scene);

	Director::getInstance()->replaceScene(Menus::CreateScene());
}

void Level1::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event){
	//log("Key with keycode %d pressed", keyCode);

	switch (keyCode){
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			mueveIzq = true;
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			mueveDch = true;
			break;
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
			mueveArr = true;
			break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			mueveAbj = true;
			break;
		case EventKeyboard::KeyCode::KEY_CTRL:
		case EventKeyboard::KeyCode::KEY_RIGHT_CTRL:
		case EventKeyboard::KeyCode::KEY_SPACE:
			// fiyah!
			dispara = true;
			break;
		case EventKeyboard::KeyCode::KEY_ESCAPE:
			sale = true;
		default:
			break;
	}
}

void Level1::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event){
	//log("Key with keycode %d released", keyCode);

	switch(keyCode){
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			mueveIzq = false;
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			mueveDch = false;
			break;
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
			mueveArr = false;
			break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			mueveAbj = false;
			break;
		case EventKeyboard::KeyCode::KEY_CTRL:
		case EventKeyboard::KeyCode::KEY_RIGHT_CTRL:
		case EventKeyboard::KeyCode::KEY_SPACE:
			// no fiyah!
			dispara = false;
			break;
		default:
			break;
	}

}

void Level1::mueveBalas(std::vector<Bala *> pool){
	// con pool de balas
	for(auto b = pool.cbegin(); b != pool.cend(); ++b){
		if((*b)->isActive()){
			(*b)->mueve();
		}
	}
}

void Level1::creaEnemigos(){

	// TODO: limpiar de verdad la horda vieja
	if(hordaActual)
		delete hordaActual;

	hordaActual = hordaNivel(nivelActual);
}

Horda *Level1::hordaNivel(int nivel){
	Horda *tmp;

	Vec2 posIniHorda;
	posIniHorda.x = 0;
	posIniHorda.y = visibleSize.height;

	tmp = new Horda(this, posIniHorda);

	// TODO: primer intento de progresion
	if(nivelActual > 5){
		tmp->creaHorda(10, 8, poolBalasEnemigas, 350.f, 10.f, RAND_MAX / 10, 6.0);

	} else if(nivelActual > 4){
		tmp->creaHorda(8, 6, poolBalasEnemigas, 250.f, 10.f, RAND_MAX / 12, 4.0);

	} else if(nivelActual > 3){
		tmp->creaHorda(6, 5, poolBalasEnemigas, 200.f, 10.f, RAND_MAX / 15, 3.0);

	} else if(nivelActual > 2){
		tmp->creaHorda(5, 4, poolBalasEnemigas, 150.f, 10.f, RAND_MAX / 30, 2.0);

	} else if(nivelActual > 1){
		tmp->creaHorda(4, 3, poolBalasEnemigas, 100.f, 10.f, RAND_MAX / 45, 2.0);

	} else{
		// primer nivel
		tmp->creaHorda(2, 2, poolBalasEnemigas, 50.f, 10.f, RAND_MAX / 60, 1.0);
	}

	return tmp;

}

void Level1::controlaProta(){

	player->mueve(mueveIzq, mueveDch, mueveArr, mueveAbj);


	if(dispara){
		//dispara = false;	// de one en one
		player->dispara(poolBalas);
	}
}

// el gameloop
void Level1::update(float delta){
	if(sale){
		menuVuelveCallback(this);
		return;
	}

	// así tengo siempre una referencia
	//deltaT = delta;
	//tiempoTranscurrido += delta;
	gameInstance->ellapsedTime += delta;

	// El juego se controla con estados
	switch(Game::getInstance()->estadoActual){
	case Game::estadosJuego::introNivel:
		// empieza a contar el tiempo de introNivel
		if(iniciaTemporizadorCambioEstado){
			tIniCambiaEstado = gameInstance->ellapsedTime;
			iniciaTemporizadorCambioEstado = false;
			mensajeIntro = "Level " + std::to_string(nivelActual) + ". START!";
			lblMensajes->setString(mensajeIntro);
			lblMensajes->setVisible(true);

			creaEnemigos();

		}

		if(gameInstance->ellapsedTime - tIniCambiaEstado >= tiempoIntro){
			lblMensajes->setVisible(false);

			Game::getInstance()->estadoActual = Game::estadosJuego::jugando;
			// empieza a contar hacia el gameOver
			iniciaTemporizadorCambioEstado = true;
		}

		break;
	case Game::estadosJuego::jugando:
		if(iniciaTemporizadorCambioEstado){
			tIniCambiaEstado = gameInstance->ellapsedTime;
			iniciaTemporizadorCambioEstado = false;
		}

		controlaProta();

		// enemigos
		if(hordaActual){
			hordaActual->tick();
		}

		//mueveEnemigos(delta * enemigoSpeed);

		mueveBalas(poolBalas);
		mueveBalas(poolBalasEnemigas);
		mueveBalas(poolBalasGordas);

		break;
	case Game::estadosJuego::finNivel:
		if(iniciaTemporizadorCambioEstado){
			tIniCambiaEstado = gameInstance->ellapsedTime;
			iniciaTemporizadorCambioEstado = false;
		}

		lblMensajes->setString(mensajeFin);
		lblMensajes->setVisible(true);

		if(gameInstance->ellapsedTime - tIniCambiaEstado >= tiempoFinNivel){
			Game::getInstance()->estadoActual = Game::estadosJuego::introNivel;
			// empieza a contar hacia el gameOver
			iniciaTemporizadorCambioEstado = true;
		}

		break;
	case Game::estadosJuego::finHorda:
		subeNivel();
		break;
	case Game::estadosJuego::muerte:
		if(iniciaTemporizadorCambioEstado){
			tIniCambiaEstado = gameInstance->ellapsedTime;
			iniciaTemporizadorCambioEstado = false;
		}

		lblMensajes->setString(mensajeMuerte);
		lblMensajes->setVisible(true);

		player->resetea();

		if(gameInstance->ellapsedTime - tIniCambiaEstado >= tiempoMuerte){
			Game::getInstance()->estadoActual = Game::estadosJuego::introNivel;
			// empieza a contar hacia el gameOver
			iniciaTemporizadorCambioEstado = true;
		}
		break;

	default:
		CCLOG("Gamestate desconocido: %d", Game::getInstance()->estadoActual);
		break;
	}

}

void Level1::subeNivel(){

	if(iniciaTemporizadorCambioEstado){
		tIniCambiaEstado = gameInstance->ellapsedTime;
		iniciaTemporizadorCambioEstado = false;
	}

	lblMensajes->setString(mensajeFinHorda);
	lblMensajes->setVisible(true);

	if(gameInstance->ellapsedTime - tIniCambiaEstado >= tiempoFinHorda){
		Game::getInstance()->estadoActual = Game::estadosJuego::introNivel;

		// TODO: Pruebas. CreaEnemigos ahora se encarga de crear hordas más dificiles por cada "nivel"
		// CreaEnemigos se llama en introNivel
		nivelActual++;

		// empieza a contar hacia... nada realmente
		iniciaTemporizadorCambioEstado = true;
	}
}

float Level1::calculaDanyoImpacto(Sprite *sprA, Sprite *sprB){
	Bala *balaTmp;

	if(sprA->getTag() == (int)Game::CategoriaColision::Bala || sprA->getTag() == (int)Game::CategoriaColision::BalaEnemigo){
		balaTmp = (Bala *)sprA->getUserData();
		return balaTmp->getDanyoBala();
	} else if(sprB->getTag() == (int)Game::CategoriaColision::Bala || sprB->getTag() == (int)Game::CategoriaColision::BalaEnemigo){
		balaTmp = (Bala *)sprB->getUserData();
		return balaTmp->getDanyoBala();
	}

	CCLOG("Daño por defecto en impacto");

	return 1.f;
}

// ----------------------------------------------------------------------------------------------------------------------------------------
// física
// ----------------------------------------------------------------------------------------------------------------------------------------
// physicscontact test
bool Level1::onContactBegin(PhysicsContact &contact){
	float dmg;

	Sprite *sprA, *sprB;
	Bala *balaTmp;

	// HACK: Gestion de impactos, primera version que funciona

	//Node *nodeA = contact.getShapeA()->getBody()->getNode();
	//Node *nodeB = contact.getShapeB()->getBody()->getNode();

	// envía el impacto a los dos "impactantes"

	sprA = (Sprite *)contact.getShapeA()->getBody()->getNode();
	sprB = (Sprite *)contact.getShapeB()->getBody()->getNode();

	dmg = calculaDanyoImpacto(sprA, sprB);

	// HACK: a todos les paso el daño pero no todos hacen algo con el
	gestionaImpacto(sprA, dmg);
	gestionaImpacto(sprB, dmg);

	////nodeA->removeFromParent();
	////nodeB->removeFromParent();

	return true;
}

void Level1::gestionaImpacto(Sprite *sprite, float dmg){
	if(!sprite){
		CCLOG("ORROR: impacto sin sprite?");
		return;
	}
	
	switch(sprite->getTag()){
	case (int)Game::CategoriaColision::Bala:
	case (int)Game::CategoriaColision::BalaEnemigo:
		// las balas desaparecen
		Bala *balaTmp;
		// alehoop!
		balaTmp = (Bala *)sprite->getUserData();
		if(balaTmp){
			balaTmp->reproduceSonido(Bala::impacto);
			balaTmp->desActivar();
		} else{
			CCLOG("catacroker, no era una bala");
		}
		break;
	case (int)Game::CategoriaColision::Enemigo:
		// el enemigo sufre daño o muere
		Enemigo *enemigoTmp;
		enemigoTmp = (Enemigo *)sprite->getUserData();
		if(enemigoTmp){
			// TODO: que situacion mas peluda, tengo que acceder ahora al daño de la bala...
			enemigoTmp->impacto(dmg);
			// esto que lo haga el enemigo mismo
			//enemigoTmp->desActivar();
		} else{
			CCLOG("catacroker, no era un enemigo");
		}

		break;
	case (int)Game::CategoriaColision::Jugador:
		// el jugador sufre daño o muere
		Jugador *jugadorTmp;
		jugadorTmp = (Jugador *)sprite->getUserData();
		if(jugadorTmp){
			// La clase impacto deberá gestionar también el cambio de estado si se nos muere el prota
			// HACK: pues de momento no, por como gestiono el tema con las variables de iniciar esperas...
			if(jugadorTmp->impacto(dmg)){
				// el jugador se ha muerto
				Game::getInstance()->estadoActual = Game::estadosJuego::muerte;
				iniciaTemporizadorCambioEstado = true;
			}
		} else{
			CCLOG("catacroker, no era un jugador");
		}

		break;
	default:
		// wtf tipo de objeto ha venido?
		CCLOG("tipo de objeto desconocido en colision: %d", sprite->getTag());
		break;

	}
}
