#include "AnimTest.h"
#include "Menus.h"
#include "pugixml.hpp"
#include "Game.h"
#include "AnimSprites.h"

#include "XmlHelper.h"

#include "Bullet.h"
#include "Pool.h"

// C++11 -> std::this_thread::sleep_for(std::chrono::milliseconds(x));
//#include <chrono>
//#include <thread>

AnimTest::~AnimTest(){
	Director::getInstance()->setDisplayStats(false);
}

#pragma region inicializaciones estandar

Scene* AnimTest::createScene(){
	// 'scene' is an autorelease object
	// ----------------------------------------------------------------------------------------------------------------------------------------
	// sin física:
	//auto scene = Scene::create();
	// ----------------------------------------------------------------------------------------------------------------------------------------
	// con física (chipmunk):
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	// ----------------------------------------------------------------------------------------------------------------------------------------

	// 'layer' is an autorelease object
	auto layer = AnimTest::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool AnimTest::init(){

	Size visibleSize;
	Vec2 origin;
	
	// ----------------------------------------------------------------------------------------------------------------------------------------
	// inits
	// ----------------------------------------------------------------------------------------------------------------------------------------
	if(!Layer::init()){
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	auto size = Director::getInstance()->getWinSize();
	// ----------------------------------------------------------------------------------------------------------------------------------------
	// preparando los eventos
	// ----------------------------------------------------------------------------------------------------------------------------------------
	// input listeners
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(AnimTest::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(AnimTest::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Botón de vuelta atrás
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
	auto vuelveAtras = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(AnimTest::menuVuelveCallback, this));
	vuelveAtras->setPosition(Vec2(origin.x + visibleSize.width - vuelveAtras->getContentSize().width / 2, origin.y + vuelveAtras->getContentSize().height / 2));
	// create menu, it's an autorelease object
	auto menu = Menu::create(vuelveAtras, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	// schedules update every frame with default order 0. Lower orders execute first
	this->scheduleUpdate();
	// displays fps
	Director::getInstance()->setDisplayStats(true);
	
	miInit();

	return true;
}

#pragma endregion

void AnimTest::miInit(){
	XmlHelper *xh = new XmlHelper;
	auto screenSize = Director::getInstance()->getWinSize();
	//cocos2d::RandomHelper::random_int<int>(1, dado) == 1)
	//xh->loadAnimation(this, "mibala");
	
	//for(int i = 0; i < 100; i++){
	//	tmp = Bullet::creaBalaAnimada(this, Bullet::bulletTypes::tipoEnemyNormal, "plz", "mibala");
	//	//tmp->getSprite()->setScale(4.0f);
	//	tmp->activa(cocos2d::RandomHelper::random_real<float>(0, screenSize.width), cocos2d::RandomHelper::random_real<float>(0, screenSize.height));
	//	//tmp->activa(cocos2d::RandomHelper::random_real<float>(0, screenSize.width), screenSize.height);
	//	//tmp->animSprites_ = xh->loadAnimation(this, "mibala");

	//	balas_.push_back(tmp);
	//}
}

void AnimTest::menuVuelveCallback(Ref *pSender){
	// vuelve al menu
	//Director::getInstance()->replaceScene(Menus::CreateScene());

	vuelveAlMenu();
}

void AnimTest::vuelveAlMenu(){
	Director::getInstance()->replaceScene(Menus::CreateScene());
}

void AnimTest::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event){

	switch(keyCode){
	case EventKeyboard::KeyCode::KEY_ESCAPE:
		vuelveAlMenu();
		break;
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		mueveIzq_ = true;
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		mueveDch_= true;
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		mueveArr_ = true;
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		mueveAbj_ = true;
		break;
	default:
		break;
	}
}

void AnimTest::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event){
	switch(keyCode){
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		mueveIzq_ = false;
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		mueveDch_ = false;
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		mueveArr_ = false;
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		mueveAbj_ = false;
		break;
	default:
		break;
	}
}

void AnimTest::update(float deltaT){
	// siempre cuento el tiempo
	Game::getInstance()->ellapsedTime += deltaT;

	for(auto b = balas_.cbegin(); b != balas_.cend(); ++b){
		(*b)->update(deltaT);
	}


}

