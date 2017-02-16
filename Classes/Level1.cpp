#include "Level1.h"
#include "proj.win32\Menus.h"	// Oops, la he cagado con la estructura :/

#include "SimpleAudioEngine.h"

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
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// verlo en la output window
	CCLOG("visiablesize x: %f y: %f", visibleSize.width + origin.x, visibleSize.height + origin.y);

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

    /////////////////////////////
    // 3. add your codes below...

	//Sprite* spriteNave = Sprite::create("spaceshipspr.png");
	prota = Sprite::create("spaceshipspr.png");
	//Sprite* spriteNave = Sprite::create("boss aircraft.png");

	// creo al prota en la parte inferior
	// reduzco a la mitad el sprite
	prota->setScale(0.5f);
	// ojo al posicionarlo, que el tamaño es getScale * getContentSize
	prota->setPosition(origin.x + visibleSize.width/2.0f, prota->getScale()*prota->getContentSize().height/2.0f);
	addChild(prota,0);
	

	#pragma region inputs


	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(Level1::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(Level1::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	#pragma endregion

	mueveAbj = mueveArr = mueveDch = mueveIzq = false;


	// schedules update every frame with order 0
	// OJO: el orden se puede definir, primero se ejecutan los de orden más bajo
	this->scheduleUpdate();

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
	log("Key with keycode %d pressed", keyCode);

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
		case EventKeyboard::KeyCode::KEY_SPACE:
			// fiyah!
			break;
		default:
			break;
	}
}

void Level1::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event){
	log("Key with keycode %d released", keyCode);

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
		case EventKeyboard::KeyCode::KEY_SPACE:
			// no fiyah!
			break;
		default:
			break;
	}

}

// el gameloop
void Level1::update(float delta){
	/*
	auto position = sprite->getPosition();
	position.x -= 250 * delta;
	if(position.x  < 0 - (sprite->getBoundingBox().size.width / 2))
		position.x = this->getBoundingBox().getMaxX() + sprite->getBoundingBox().size.width / 2;
	sprite->setPosition(position);
	*/
	//CCLOG("tick! %f", delta);
	
	if(!prota){
		CCLOG("OJO CASCOTE, Prota sin definir");
		return;
	}


	auto visibleSize = Director::getInstance()->getVisibleSize();

	if(mueveIzq){
		//mueveIzq = false;
		auto position = prota->getPosition();
		// hace el movimiento independiente de framerates
		position.x -= protaSpeed * delta;

		if(position.x < 0){
			position.x = 0;
		}

		prota->setPosition(position);
	}

	if(mueveDch){
		//mueveDch = false;
		auto position = prota->getPosition();
		// hace el movimiento independiente de framerates
		position.x += protaSpeed * delta;

		if(position.x > visibleSize.width){
			position.x = visibleSize.width;
		}

		prota->setPosition(position);

	}


	if(mueveArr){
		auto position = prota->getPosition();
		// hace el movimiento independiente de framerates
		position.y += protaSpeed * delta;

		if(position.y > visibleSize.height){
			position.y = visibleSize.height;
		}

		prota->setPosition(position);
	}

	if(mueveAbj){
		auto position = prota->getPosition();
		// hace el movimiento independiente de framerates
		position.y -= protaSpeed * delta;

		if(position.y < 0.0f){
			position.y = 0.0f;
		}

		prota->setPosition(position);
	}

}