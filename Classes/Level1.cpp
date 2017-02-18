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

// on "init" you need to initialize your instance
bool Level1::init() {

	#pragma region inits
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() ) {
        return false;
    }
    
    //auto visibleSize = Director::getInstance()->getVisibleSize();
	visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

	// verlo en la output window
	CCLOG("visiblesize x: %f y: %f", visibleSize.width + origin.x, visibleSize.height + origin.y);

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to go back
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto vuelveAtras = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(Level1::menuVuelveCallback, this));
    
    vuelveAtras->setPosition(Vec2(origin.x + visibleSize.width - vuelveAtras->getContentSize().width/2 ,origin.y + vuelveAtras->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(vuelveAtras, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
	
	#pragma endregion

	protaSprite = Sprite::create("spaceshipspr.png");
	//balaSprite = Sprite::create("bullet_2_orange.png");

	// creo al prota en la parte inferior
	// reduzco a la mitad el sprite
	protaSprite->setScale(0.5f);
	// ojo al posicionarlo, que el tamaño es getScale * getContentSize
	protaSprite->setPosition(origin.x + visibleSize.width/2.0f, protaSprite->getScale()*protaSprite->getContentSize().height/2.0f);
	addChild(protaSprite,0);
	
	// input listeners
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(Level1::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(Level1::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


	mueveAbj = mueveArr = mueveDch = mueveIzq = false;

	tiempoTranscurrido = 0;

	creaEnemigos();

	// init audio
	audio = CocosDenshion::SimpleAudioEngine::getInstance();

	// TODO: por cierto que esto no ha servido de nada
	//audio->preloadEffect("explosion.wav");
	//audio->preloadEffect("fastinvader1.wav");
	//audio->preloadEffect("fastinvader2.wav");
	//audio->preloadEffect("fastinvader3.wav");
	//audio->preloadEffect("fastinvader4.wav");
	//audio->preloadEffect("invaderkilled.wav");
	//audio->preloadEffect("shoot.wav");
	//audio->preloadEffect("ufo_highpitch.wav");
	//audio->preloadEffect("ufo_lowpitch.wav");
	// Ni esta brutalidad tampoco. Hmm...
	//audio->playEffect("explosion.wav", false, 1.0f, 0.0f, 0.0f);
	//audio->playEffect("fastinvader1.wav", false, 1.0f, 0.0f, 0.0f);
	//audio->playEffect("fastinvader2.wav", false, 1.0f, 0.0f, 0.0f);
	//audio->playEffect("fastinvader3.wav", false, 1.0f, 0.0f, 0.0f);
	//audio->playEffect("fastinvader4.wav", false, 1.0f, 0.0f, 0.0f);
	//audio->playEffect("invaderkilled.wav", false, 1.0f, 0.0f, 0.0f);
	//audio->playEffect("shoot.wav", false, 1.0f, 0.0f, 0.0f);
	//audio->playEffect("ufo_highpitch.wav", false, 1.0f, 0.0f, 0.0f);
	//audio->playEffect("ufo_lowpitch.wav", false, 1.0f, 0.0f, 0.0f);


	// schedules update every frame with order 0
	// OJO: el orden se puede definir, primero se ejecutan los de orden más bajo
	this->scheduleUpdate();

	CCLOG("tiempoTranscurrido=%d", tiempoTranscurrido);

	return true;
}

void Level1::menuCloseCallback(Ref* pSender) {
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
	#endif
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/
    
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
}

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

bool Level1::mueveBala(Sprite *b, float cuanto){
	if(!b){
		CCLOG("Intentando mover una bala sin inicializar");
		return false;
	}

	//auto visibleSize = Director::getInstance()->getVisibleSize();

	Vec2 pos = b->getPosition();

	pos.y += cuanto;

	if(pos.y > visibleSize.height){

		//auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
		// play a sound effect, just once.
		// TODO: ojo que la carga pega un parón curioso...
		audio->playEffect("sonidos/fastinvader2.wav", false, 1.0f, 1.0f, 1.0f);


		// hay que destruir la bala
		return true;
	}

	b->setPosition(pos);

	return false;
}

void Level1::mueveBalas(float cuanto){

	// voy a probar a borrar las balas asi...
	Vector<Sprite *> spritesBorrarTmp;

	// se recorre el array de balas y les aplica movimiento etc
	for(auto b = balas.cbegin(); b != balas.cend(); ++b){
		if(mueveBala(*b, cuanto)){
			// TODO: a ver como destruyo la bala... hmm
			// para empezar...
			removeChild(*b);
			// balas.capacity() -> no es exacto, incluye capacidad para crecer
			CCLOG("Num balas en vector: %d", balas.size());

			/*
			vector::erase()
			Removes from the vector either a single element (position) or a range of elements ([first,last)).
			This effectively reduces the container size by the number of elements removed, which are destroyed.

			Because vectors use an array as their underlying storage, erasing elements in positions other than the 
			vector end causes the container to relocate all the elements after the segment erased to their new positions. 
			This is generally an inefficient operation compared to the one performed for the same operation by other kinds 
			of sequence containers (such as list or forward_list).
			*/
			
			// no puedo borrar elementos de un vector mientras lo voy recorriendo
			// asi que... mas vectores!

			// me acabo de dar cuenta que solo va a haber una bala para borrar cada vez 
			// al menos mientras las dispare de una en una...
			spritesBorrarTmp.pushBack(*b);
		}
	}

	for(auto b = spritesBorrarTmp.cbegin(); b != spritesBorrarTmp.cend(); ++b){
		// eraseObject es un añadido de Cocos?
		balas.eraseObject(*b);
	}

	spritesBorrarTmp.clear();


}

bool Level1::mueveBalaEnemiga(Sprite *b, float cuanto){
	if(!b){
		CCLOG("Intentando mover una bala sin inicializar");
		return false;
	}

	//auto visibleSize = Director::getInstance()->getVisibleSize();

	Vec2 pos = b->getPosition();

	pos.y -= cuanto;

	if(pos.y < 0){

		//auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
		// play a sound effect, just once.
		// TODO: ojo que la carga pega un parón curioso...
		audio->playEffect("sonidos/invaderkilled.wav", false, 1.0f, 1.0f, 1.0f);


		// hay que destruir la bala
		return true;
	}

	b->setPosition(pos);

	return false;
}

void Level1::mueveBalasEnemigas(float cuanto){

	// voy a probar a borrar las balas asi...
	Vector<Sprite *> spritesBorrarTmp;

	// se recorre el array de balas y les aplica movimiento etc
	for(auto b = balasEnemigas.cbegin(); b != balasEnemigas.cend(); ++b){
		if(mueveBalaEnemiga(*b, cuanto)){
			// destruir la bala
			spritesBorrarTmp.pushBack(*b);
		}
	}

	for(auto b = spritesBorrarTmp.cbegin(); b != spritesBorrarTmp.cend(); ++b){
		removeChild(*b);
		CCLOG("Num balas enemigas en vector: %d", balasEnemigas.size());
		balasEnemigas.eraseObject(*b);
	}

	spritesBorrarTmp.clear();


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
	audio->playEffect("sonidos/fastinvader1.wav", false, 1.0f, 1.0f, 1.0f);
	
}


// el gameloop
void Level1::update(float delta){
	// así tengo siempre una referencia
	deltaT = delta;
	tiempoTranscurrido += delta;

	/*
	auto position = sprite->getPosition();
	position.x -= 250 * delta;
	if(position.x  < 0 - (sprite->getBoundingBox().size.width / 2))
		position.x = this->getBoundingBox().getMaxX() + sprite->getBoundingBox().size.width / 2;
	sprite->setPosition(position);
	*/
	//CCLOG("tick! %f", delta);
	
	if(!protaSprite){
		CCLOG("OJO CASCOTE, Prota sin definir");
		return;
	}

	//auto visibleSize = Director::getInstance()->getVisibleSize();


	#pragma region gestion de inputs
	// movimiento de la nave
	if(mueveIzq){
		//mueveIzq = false;
		auto position = protaSprite->getPosition();
		// hace el movimiento independiente de framerates
		position.x -= protaSpeed * delta;

		if(position.x < 0){
			position.x = 0;
		}

		protaSprite->setPosition(position);
	}

	if(mueveDch){
		//mueveDch = false;
		auto position = protaSprite->getPosition();
		// hace el movimiento independiente de framerates
		position.x += protaSpeed * delta;

		if(position.x > visibleSize.width){
			position.x = visibleSize.width;
		}

		protaSprite->setPosition(position);

	}


	if(mueveArr){
		auto position = protaSprite->getPosition();
		// hace el movimiento independiente de framerates
		position.y += protaSpeed * delta;

		if(position.y > visibleSize.height){
			position.y = visibleSize.height;
		}

		protaSprite->setPosition(position);
	}

	if(mueveAbj){
		auto position = protaSprite->getPosition();
		// hace el movimiento independiente de framerates
		position.y -= protaSpeed * delta;

		if(position.y < 0.0f){
			position.y = 0.0f;
		}

		protaSprite->setPosition(position);
	}

	if(sale){
		menuVuelveCallback(this);
	}

	if(dispara){
		dispara = false;	// de one en one
		// primera iteración: a lo bruto
		Sprite *bala = Sprite::create("bullet_2_blue.png");

		//balaSprite->setScale(1.0f);
		// ojo al posicionarlo, que el tamaño es getScale * getContentSize
		//balaSprite->setPosition(origin.x + visibleSize.width / 2.0f, protaSprite->getScale()*protaSprite->getContentSize().height / 2.0f);
		bala->setPosition(protaSprite->getPosition().x,protaSprite->getPosition().y + 20.0f);
		addChild(bala, 0);

		balas.pushBack(bala);
		
		// reproduce un sonido una sola vez
		//auto audio = SimpleAudioEngine::getInstance();
		//auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
		// play a sound effect, just once.
		// TODO: ojo que la carga pega un parón curioso...
		audio->playEffect("sonidos/shoot.wav", false, 1.0f, 1.0f, 1.0f);
		

	}

	#pragma endregion


	mueveBalas(delta * balaSpeed);
	mueveBalasEnemigas(delta * balaEnemigaSpeed);
	mueveEnemigos(delta * enemigoSpeed);

}