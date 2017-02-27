#include "Level1.h"
#include "Menus.h"	// Oops, la he cagado con la estructura :/

// TODO: Ojo! 
// Este USING aquí tiene que estar en Level1.h
// Si espero aquí a ponerlo, no compila pero no marca error tampoco en el .h
//USING_NS_CC;

Scene* Level1::createScene() {
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
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
    
    //auto visibleSize = Director::getInstance()->getVisibleSize();
	visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

	// verlo en la output window
	//CCLOG("visiblesize x: %f y: %f", visibleSize.width + origin.x, visibleSize.height + origin.y);

    // add a "close" icon to exit the progress. it's an autorelease object
	// el "close" icon lo que hace es volver al menú principal, lo mismo que dando a Esc
    auto vuelveAtras = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(Level1::menuVuelveCallback, this));
    vuelveAtras->setPosition(Vec2(origin.x + visibleSize.width - vuelveAtras->getContentSize().width/2 ,origin.y + vuelveAtras->getContentSize().height/2));
    // create menu, it's an autorelease object
    auto menu = Menu::create(vuelveAtras, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

	// ----------------------------------------------------------------------------------------------------------------------------------------
	// preparo el control por teclado
	// ----------------------------------------------------------------------------------------------------------------------------------------
	// input listeners
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(Level1::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(Level1::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


	// ----------------------------------------------------------------------------------------------------------------------------------------
	// GUI
	// ----------------------------------------------------------------------------------------------------------------------------------------

	lblMensajes = Label::create(mensajeIntro, "Marker Felt", 48.0, cocos2d::Size::ZERO, cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
	lblMensajes->setPosition(origin.x + visibleSize.width / 2.0f, origin.y + visibleSize.height / 2.0f);
	lblMensajes->setVisible(false);
	this->addChild(lblMensajes, 1, "lblMensajes");

	// ----------------------------------------------------------------------------------------------------------------------------------------
	// otras inicializaciones
	// ----------------------------------------------------------------------------------------------------------------------------------------

	mueveAbj = mueveArr = mueveDch = mueveIzq = false;
	tiempoTranscurrido = 0;

	// "init audio". No se si tiene alguna ventaja acceder a traves de un puntero o de getInstance directamente... 
	//audio = CocosDenshion::SimpleAudioEngine::getInstance();

	// crea al jugador y lo añade a la escena
	player = new Jugador();
	if(!player->creaSprite(this)){
		//menuVuelveCallback(this);	// por lo que se ve no puedo cargar una escena nueva en la inicialización...
		sale = true;	// lo primero que hace update es comprobar si ha de salir. OJO que se evalúe antes de hacer nada
		//return false; // mmm esto informará a cocos de algo útil? NO, de hecho le hace cascar
	}


	//creaNaveProta();
	creaEnemigos();
	
	// TODO: nota: estaba "precargando" el sonido por cada bala en el pool
	precargaSonidosDelNivel();

	creaPoolBalas(&poolBalas, 32, "bullet_2_blue.png", "sonidos/shoot.wav", "sonidos/fastinvader1.wav", 1.0f, balaSpeed);
	creaPoolBalas(&poolBalasGordas, 5, "bullet_orange0000.png", "sonidos/shoot.wav", "sonidos/fastinvader1.wav", 4.0f, balaEnemigaSpeed);

	// schedules update every frame with order 0
	// OJO: el orden se puede definir, primero se ejecutan los de orden más bajo
	this->scheduleUpdate();

	// displays fps
	Director::getInstance()->setDisplayStats(true);

	Game::getInstance()->state = Game::states::introNivel;
	iniciaTemporizadorCambioEstado = true;

	return true;
}

void Level1::precargaSonidosDelNivel(){
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sonidos/shoot.wav");
}

//void Level1::creaNaveProta(){
//	protaSprite = Sprite::create("spaceshipspr.png");
//
//	// creo al prota en la parte inferior
//	// reduzco a la mitad el sprite
//	protaSprite->setScale(0.5f);
//	// ojo al posicionarlo, que el tamaño es getScale * getContentSize
//	protaSprite->setPosition(origin.x + visibleSize.width / 2.0f, protaSprite->getScale()*protaSprite->getContentSize().height / 2.0f);
//	addChild(protaSprite, 0);
//
//}

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
	// TODO: Crear una horda

	Enemigo *tmp = new Enemigo;

	if(tmp->creaSprite(this, "Spaceship15.png", 0.5f, 0)){

		tmp->setPosition(origin.x + visibleSize.width / 2.0f, visibleSize.height - tmp->getScale()*tmp->getSprite()->getContentSize().height / 2.0f);
		tmp->getSprite()->setRotation(180.0f);	// esta está al revés :>

		enemigos.push_back(tmp);
	} else{
		CCLOG("No pude crear enemigo :I");
	}

	AutoPolygon apEnemigo = AutoPolygon("aliensprite2.png");
	PolygonInfo myInfo = apEnemigo.generateTriangles();//use all default values
	Sprite *enemigo = Sprite::create(myInfo);


	//Sprite *enemigo = Sprite::create("aliensprite2.png");

	//enemigo->setScale(0.5f);
	// ojo al posicionarlo, que el tamaño es getScale * getContentSize
	enemigo->setPosition(origin.x + visibleSize.width / 2.0f, visibleSize.height - enemigo->getScale()*enemigo->getContentSize().height / 2.0f);
	addChild(enemigo, 0);

	enemigosDeprecated.pushBack(enemigo);

}

void Level1::mueveEnemigos(float cuanto){
	for(auto e = enemigosDeprecated.cbegin(); e != enemigosDeprecated.cend(); ++e){
		mueveEnemigo(*e, cuanto);
	}
}

void Level1::mueveEnemigo(Sprite *enemigo, float cuanto){
	// TODO: OJOO que las variables static no se regeneran cuando se resetea la escena, esto está mal.
	static float tIni = tiempoTranscurrido;

	// esto tendrá que ser por cada enemigo, de momento todos igual
	static bool mueveIzq = true;

	if(!enemigo){
		CCLOG("Enemigo sin inicializar");
		return;
	}

	Vec2 pos = enemigo->getPosition();

	if(mueveIzq){

		pos.x -= cuanto;

		if(pos.x < 0){
			mueveIzq = false;
		}
	} else{
		pos.x += cuanto;

		if(pos.x > visibleSize.width){
			mueveIzq = true;
		}
	}

	enemigo->setPosition(pos);

	if(tiempoTranscurrido - tIni > 1.0f){
		// reseteo contador
		tIni = tiempoTranscurrido;

		enemigoDispara(enemigo);
	}
}

void Level1::enemigoDispara(Sprite *enemigo){
	if(!enemigo){
		CCLOG("ERROR. Intento de disparo de enemigo sin sprite");
		return;
	}
	// escoge una bala del pool que esté libre
	for(auto b = poolBalasGordas.cbegin(); b != poolBalasGordas.cend(); ++b){
		if(!(*b)->isActive()){
			// tengo una libre
			(*b)->activar(enemigo->getPosition());
			break;
		}
	}
}


void Level1::controlaProta(){

	player->mueve(mueveIzq, mueveDch, mueveArr, mueveAbj);


	if(dispara){
		//dispara = false;	// de one en one
		player->dispara(poolBalas);

		//					// con pool de balas
		//for(auto b = poolBalas.cbegin(); b != poolBalas.cend(); ++b){ // por que es ++b 
		//															  // busco una bala inactiva
		//	if(!(*b)->isActive()){
		//		(*b)->activar(protaSprite->getPosition(), balaSpeed);
		//		break;	// no me interesa el resto
		//	}
		//}

	}
}

// el gameloop
void Level1::update(float delta){
	if(sale){
		menuVuelveCallback(this);
		return;
	}

	// así tengo siempre una referencia
	deltaT = delta;
	tiempoTranscurrido += delta;

	//auto visibleSize = Director::getInstance()->getVisibleSize();


	// TODO: según el estado del juego, hace una cosa u otra
	switch(Game::getInstance()->state){
	case Game::states::introNivel:
		// empieza a contar el tiempo de introNivel
		if(iniciaTemporizadorCambioEstado){
			tIni = tiempoTranscurrido;
			iniciaTemporizadorCambioEstado = false;

			lblMensajes->setString(mensajeIntro);
			lblMensajes->setVisible(true);

		}

		if(tiempoTranscurrido - tIni >= tiempoIntro){
			lblMensajes->setVisible(false);

			Game::getInstance()->state = Game::states::jugando;
			// empieza a contar hacia el gameOver
			iniciaTemporizadorCambioEstado = true;
		}

		break;
	case Game::states::jugando:
		if(iniciaTemporizadorCambioEstado){
			tIni = tiempoTranscurrido;
			iniciaTemporizadorCambioEstado = false;
		}

		controlaProta();

		mueveEnemigos(delta * enemigoSpeed);

		mueveBalas(poolBalas);
		mueveBalas(poolBalasEnemigas);
		mueveBalas(poolBalasGordas);

		/*
		if(tiempoTranscurrido - tIni >= tiempoJugando){
			Game::getInstance()->state = Game::states::finNivel;
			// empieza a contar hacia el gameOver
			iniciaTemporizadorCambioEstado = true;
		}
		*/
		break;
	case Game::states::finNivel:
		if(iniciaTemporizadorCambioEstado){
			tIni = tiempoTranscurrido;
			iniciaTemporizadorCambioEstado = false;
		}

		lblMensajes->setString(mensajeFin);
		lblMensajes->setVisible(true);

		if(tiempoTranscurrido - tIni >= tiempoFinNivel){
			Game::getInstance()->state = Game::states::introNivel;
			// empieza a contar hacia el gameOver
			iniciaTemporizadorCambioEstado = true;
		}

		break;
	default:
		CCLOG("Gamestate desconocido: %d", Game::getInstance()->state);
		break;
	}


}