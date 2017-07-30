#include "AnimTest.h"
#include "Menus.h"
#include "pugixml.hpp"
#include "Game.h"
#include "AnimSprites.h"

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
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
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

	animaciones_ = new AnimSprites(this);
	Sprite *spr;
	AnimSprites::frame *f;

	animaciones_->setPosition(Vec2(500.f, 500.f));
	auto delay = 0.1f;
	auto size = 5.0f;

	spr = Game::creaSprite("bullet_orange0000.png", "bullet_orange0000.png", 0, 0, size);
	//spr = Sprite::createWithSpriteFrameName("bullet_orange0000.png");
	f = new AnimSprites::frame(spr, delay, false, std::string(""));
	animaciones_->addFrame(std::string("loop"), f);
	
	spr = Game::creaSprite("bullet_blue0001.png", "bullet_orange0001.png", 0, 0, size);
	//spr = Sprite::createWithSpriteFrameName("bullet_orange0001.png");
	f = new AnimSprites::frame(spr, delay, false, std::string(""));
	animaciones_->addFrame(std::string("loop"), f);

	//spr = Sprite::createWithSpriteFrameName("bullet_orange0002.png");
	spr = Game::creaSprite("bullet_orange0002.png", "bullet_orange0002.png", 0, 0, size);
	f = new AnimSprites::frame(spr, delay, false, std::string(""));
	animaciones_->addFrame(std::string("loop"), f);

	//spr = Sprite::createWithSpriteFrameName("bullet_orange0003.png");
	spr = Game::creaSprite("bullet_blue0003.png", "bullet_orange0003.png", 0, 0, size);
	f = new AnimSprites::frame(spr, delay, false, std::string(""));
	animaciones_->addFrame(std::string("loop"), f);

	//spr = Sprite::createWithSpriteFrameName("bullet_orange0004.png");
	spr = Game::creaSprite("bullet_orange0004.png", "bullet_orange0004.png", 0, 0, size);
	f = new AnimSprites::frame(spr, delay, false, std::string(""));
	animaciones_->addFrame(std::string("loop"), f);

	animaciones_->playStart("loop");

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
	//Sleep(1000); // Win32 Sleep() has millisecond precision, but it's accuracy can be orders of magnitude higher.
	//std::this_thread::sleep_for(std::chrono::milliseconds(1000));	// estándar, portable

	// siempre cuento el tiempo
	Game::getInstance()->ellapsedTime += deltaT;

	mueveNodo();

	#pragma region animacion
	if(animaciones_){
		animaciones_->update(deltaT);
	}
	/*
	size_t lastFrame;
	if(currFrame_ > 0){
		lastFrame = currFrame_ - 1;
	} else{
		lastFrame = numFrames_ - 1;
	}

	AnimSprites *tmp = frames_[lastFrame];
	tmp->show(false);
	tmp = frames_[currFrame_];
	tmp->show(true);

	if(currFrame_ >= numFrames_ - 1){
		currFrame_ = 0;
	} else{
		++currFrame_;
	}

	// TODO: pero bloqueo todo haciendo esto
	std::this_thread::sleep_for(std::chrono::milliseconds(tmp->delay_));	// estándar, portable
	*/
	#pragma endregion
}

void AnimTest::mueveNodo(){
	auto winSize = Director::getInstance()->getWinSize();
	auto deltaT = Director::getInstance()->getDeltaTime();
	//Vec2 pos = nodo_->getPosition();
	Vec2 pos = animaciones_->getPosition();

	float spd = 150.0f;

	if(mueveArr_){
		pos.y += spd * deltaT;
	}
	if(mueveAbj_){
		pos.y -= spd * deltaT;
	}
	if(mueveIzq_){
		pos.x -= spd * deltaT;
	}
	if(mueveDch_){
		pos.x += spd * deltaT;
	}

	if(pos.x > winSize.width) {
		pos.x = winSize.width;
	} else if(pos.x < 0){
		pos.x = 0;
	} else if(pos.y > winSize.height) {
	pos.y = winSize.height;
	} else if(pos.y < 0) {
		pos.y = 0;
	}

	animaciones_->setPosition(pos);
}

void AnimTest::carga(const char *filename){
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filename);

	//std::cout << "Load result: " << result.description() << ", whatever: " << doc.child("book").attribute("category").value() << std::endl;
	//std::cout << doc.child("person").child_value("firstname") << std::endl;
	//std::cout << doc.child("person").child_value("lastname") << std::endl;

	// C++11
	//for(pugi::xml_node person : doc.children("person")){
	//	//std::cout << person.child_value("firstname") << " " << person.child_value("lastname") << std::endl;
	//	CCLOG("%s", person.child_value("firstname"));
	//}

	//config_properties.insert({ CONFIG_PLAYER_INITIAL_SPEED, xml_default_values.child_value(CONFIG_PLAYER_INITIAL_SPEED) });
	//config_properties.insert({ CONFIG_PLAYER_PATH_SPRITE, xml_default_values.child_value(CONFIG_PLAYER_PATH_SPRITE) });

	//auto iter = config_properties.find(CONFIG_PLAYER_INITIAL_SPEED);

	//if(iter != config_properties.end()){
	//	// ojo find devuelve un iterator
	//	CCLOG("%s", iter->second);
	//}

	//iter = config_properties.find(CONFIG_PLAYER_PATH_SPRITE);
	//if(iter != config_properties.end()){
	//	CCLOG("%s", iter->second);
	//}


	pugi::xml_node xml_default_values = doc.child("default_values");
	
	auto x = xml_default_values.child_value("bullet_t1_anim");
	//player_initial_speed = atof(xml_default_values.child_value(CONFIG_PLAYER_INITIAL_SPEED));

}