#include "Level1.h"
#include "proj.win32\Menus.h"	// Oops, la he cagado con la estructura :/

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
	// otras inicializaciones
	// ----------------------------------------------------------------------------------------------------------------------------------------

	mueveAbj = mueveArr = mueveDch = mueveIzq = false;
	tiempoTranscurrido = 0;

	// "init audio". No se si tiene alguna ventaja acceder a traves de un puntero o de getInstance directamente... 
	//audio = CocosDenshion::SimpleAudioEngine::getInstance();

	creaNaveProta();
	creaEnemigos();
	creaPoolBalas(&poolBalas, 5, "bullet_2_blue.png", "sonidos/shoot.wav", 1.0f);
	creaPoolBalas(&poolBalasGordas, 5, "bullet_orange0000.png", "sonidos/shoot.wav", 2.0f);

	// schedules update every frame with order 0
	// OJO: el orden se puede definir, primero se ejecutan los de orden más bajo
	this->scheduleUpdate();

	// displays fps
	Director::getInstance()->setDisplayStats(true);

	return true;
}


void Level1::creaNaveProta(){
	protaSprite = Sprite::create("spaceshipspr.png");
	//balaSprite = Sprite::create("bullet_2_orange.png");

	// creo al prota en la parte inferior
	// reduzco a la mitad el sprite
	protaSprite->setScale(0.5f);
	// ojo al posicionarlo, que el tamaño es getScale * getContentSize
	protaSprite->setPosition(origin.x + visibleSize.width / 2.0f, protaSprite->getScale()*protaSprite->getContentSize().height / 2.0f);
	addChild(protaSprite, 0);

}

void Level1::creaPoolBalas(std::vector<Bala *> *pool, int cant, std::string pathSpriteBala, const char *pathSonidoBala, float scale){
	// pool de balas
	for(int i = 0; i < cant; i++){
		Bala *tmp = new Bala(pathSpriteBala, pathSonidoBala);
		tmp->getSprite()->setScale(scale);
		
		pool->push_back(tmp);

		// cuelgo cada sprite del nodo actual
		// o no se mostrará nada
		// TODO: ummm no sé si debo destruirlos a mano o se encarga cocos
		addChild(tmp->getSprite());
	}


}

//void Level1::menuCloseCallback(Ref* pSender) {
//    //Close the cocos2d-x game scene and quit the application
//    Director::getInstance()->end();
//
//    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    exit(0);
//	#endif
//    
//    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/
//    
//    //EventCustom customEndEvent("game_scene_close_event");
//    //_eventDispatcher->dispatchEvent(&customEndEvent);
//}

void Level1::menuVuelveCallback(Ref *pSender){
	// vuelve al menu
	auto scene = Menus::CreateScene();
	auto director = Director::getInstance();
	director->replaceScene(scene);
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
	Sprite *enemigo = Sprite::create("aliensprite2.png");

	//enemigo->setScale(0.5f);
	// ojo al posicionarlo, que el tamaño es getScale * getContentSize
	enemigo->setPosition(origin.x + visibleSize.width / 2.0f, visibleSize.height - enemigo->getScale()*enemigo->getContentSize().height / 2.0f);
	addChild(enemigo, 0);

	enemigos.pushBack(enemigo);

}

void Level1::mueveEnemigos(float cuanto){
	for(auto e = enemigos.cbegin(); e != enemigos.cend(); ++e){
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
	// con pool
	for(auto b = poolBalasGordas.cbegin(); b != poolBalasGordas.cend(); ++b){
		if(!(*b)->isActive()){
			// tengo una libre
			(*b)->activar(enemigo->getPosition(),balaEnemigaSpeed);
			break;
		}
	}

	/*
	Sprite *bala = Sprite::create("bullet_orange0000.png");

	bala->setScale(3.0f);
	bala->setPosition(enemigo->getPosition().x, enemigo->getScale()*enemigo->getPosition().y - 20.0f);
	addChild(bala, 1);

	balasEnemigas.pushBack(bala);

	// reproduce un sonido una sola vez
	//auto audio = SimpleAudioEngine::getInstance();
	//auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	// play a sound effect, just once.
	// TODO: ojo que la carga pega un parón curioso...
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sonidos/fastinvader1.wav", false, 1.0f, 1.0f, 1.0f);
	*/
}


void Level1::mueveProta(){
	// movimiento de la nave
	if(mueveIzq){
		//mueveIzq = false;
		auto position = protaSprite->getPosition();
		// hace el movimiento independiente de framerates
		position.x -= protaSpeed * deltaT;

		if(position.x < 0){
			position.x = 0;
		}

		protaSprite->setPosition(position);
	}

	if(mueveDch){
		//mueveDch = false;
		auto position = protaSprite->getPosition();
		// hace el movimiento independiente de framerates
		position.x += protaSpeed * deltaT;

		if(position.x > visibleSize.width){
			position.x = visibleSize.width;
		}

		protaSprite->setPosition(position);

	}


	if(mueveArr){
		auto position = protaSprite->getPosition();
		// hace el movimiento independiente de framerates
		position.y += protaSpeed * deltaT;

		if(position.y > visibleSize.height){
			position.y = visibleSize.height;
		}

		protaSprite->setPosition(position);
	}

	if(mueveAbj){
		auto position = protaSprite->getPosition();
		// hace el movimiento independiente de framerates
		position.y -= protaSpeed * deltaT;

		if(position.y < 0.0f){
			position.y = 0.0f;
		}

		protaSprite->setPosition(position);
	}

	if(dispara){
		dispara = false;	// de one en one

							// con pool de balas
		for(auto b = poolBalas.cbegin(); b != poolBalas.cend(); ++b){ // por que es ++b 
																	  // busco una bala inactiva
			if(!(*b)->isActive()){
				(*b)->activar(protaSprite->getPosition(), balaSpeed);

				break;	// no me interesa el resto
			}
		}

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

	if(!protaSprite){
		CCLOG("OJO CASCOTE, Prota sin definir");
		return;
	}

	//auto visibleSize = Director::getInstance()->getVisibleSize();


	mueveProta();
	mueveEnemigos(delta * enemigoSpeed);

	mueveBalas(poolBalas);
	mueveBalas(poolBalasEnemigas);
	mueveBalas(poolBalasGordas);
}