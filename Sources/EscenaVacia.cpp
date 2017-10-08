#include "EscenaVacia.h"
#include "Menus.h"

// ----------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------
#pragma region borrameeee

//#include "Pool.h"
//
//void EscenaVacia::dale(){
//	MiSprite *spr = MiSprite::createWithSpriteFrameName("bullet_blue0000.png");
//
//	//spr->sprite->setPosition(Director::getInstance()->getWinSize().width / 2.0f, Director::getInstance()->getWinSize().height / 2.0f);
//	spr->sprite->setScale(4.0f);
//
//	addChild(spr->sprite);
//	//addChild(spr->sprite);
//}
//
//#pragma endregion
// ----------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------


EscenaVacia::~EscenaVacia(){
	Director::getInstance()->setDisplayStats(false);

}

Scene* EscenaVacia::createScene(){
	// 'scene' is an autorelease object
	// sin física:
	auto scene = Scene::create();
	// con física (chipmunk):
	//auto scene = Scene::createWithPhysics();
	// set the world’s gravity to zero in both directions, which essentially disables gravity
	//scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
	// enable debug drawing to see your physics bodies
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	// ----------------------------------------------------------------------------------------------------------------------------------------

	// 'layer' is an autorelease object
	auto layer = EscenaVacia::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool EscenaVacia::init(){

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
	listener->onKeyPressed = CC_CALLBACK_2(EscenaVacia::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(EscenaVacia::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Botón de vuelta atrás
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
	auto vuelveAtras = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(EscenaVacia::menuVuelveCallback, this));
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

void EscenaVacia::menuVuelveCallback(Ref *pSender){
	// vuelve al menu
	//Director::getInstance()->replaceScene(Menus::CreateScene());

	vuelveAlMenu();
}

void EscenaVacia::vuelveAlMenu(){
	Director::getInstance()->replaceScene(Menus::CreateScene());
}

void EscenaVacia::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event){
	CCLOG("Keypressed");

	switch(keyCode){
	case EventKeyboard::KeyCode::KEY_ESCAPE:
		vuelveAlMenu();
		break;
	default:
		break;
	}
}

void EscenaVacia::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event){
	CCLOG("Keyreleased");
}

void EscenaVacia::update(float deltaT){
}

