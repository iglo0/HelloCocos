#include "HiScores.h"
#include "Menus.h"

HiScores::~HiScores(){
	Director::getInstance()->setDisplayStats(false);

}

Scene* HiScores::createScene(){
	// 'scene' is an autorelease object
	// sin f�sica:
	auto scene = Scene::create();
	// con f�sica (chipmunk):
	//auto scene = Scene::createWithPhysics();
	// set the world�s gravity to zero in both directions, which essentially disables gravity
	//scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
	// enable debug drawing to see your physics bodies
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	// ----------------------------------------------------------------------------------------------------------------------------------------

	// 'layer' is an autorelease object
	auto layer = HiScores::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HiScores::init(){

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
	listener->onKeyPressed = CC_CALLBACK_2(HiScores::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(HiScores::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Bot�n de vuelta atr�s
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
	auto vuelveAtras = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(HiScores::menuVuelveCallback, this));
	vuelveAtras->setPosition(Vec2(origin.x + visibleSize.width - vuelveAtras->getContentSize().width / 2, origin.y + vuelveAtras->getContentSize().height / 2));
	// create menu, it's an autorelease object
	auto menu = Menu::create(vuelveAtras, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	// schedules update every frame with default order 0. Lower orders execute first
	this->scheduleUpdate();
	// displays fps
	Director::getInstance()->setDisplayStats(true);
	

	// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Mis cosas
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------


	return true;
}

void HiScores::menuVuelveCallback(Ref *pSender){
	// vuelve al menu
	//Director::getInstance()->replaceScene(Menus::CreateScene());

	vuelveAlMenu();
}

void HiScores::vuelveAlMenu(){
	Director::getInstance()->replaceScene(Menus::CreateScene());
}

void HiScores::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event){
	CCLOG("Keypressed");

	switch(keyCode){
	case EventKeyboard::KeyCode::KEY_ESCAPE:
		vuelveAlMenu();
		break;
	default:
		break;
	}
}

void HiScores::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event){
	CCLOG("Keyreleased");
}

void HiScores::update(float deltaT){
}

