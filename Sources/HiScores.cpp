#include "HiScores.h"
#include "Menus.h"
#include "Game.h"
//#include <iostream>	// para usar std::cout << ... << std::setfill('0') << 4 ... (leading zeros)
//#include <iomanip>	// para usar std::cout << ... << std::setfill('0') << 4 ... (leading zeros)


HiScores::~HiScores(){
	Director::getInstance()->setDisplayStats(false);

}

Scene* HiScores::createScene(){
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
	auto layer = HiScores::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HiScores::init(){
	#pragma region inicializaciones estandar
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
	// Botón de vuelta atrás
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
	auto vuelveAtras = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(HiScores::menuVuelveCallback, this));
	vuelveAtras->setPosition(Vec2(origin.x + visibleSize.width - vuelveAtras->getContentSize().width / 2, origin.y + vuelveAtras->getContentSize().height / 2));
	// create menu, it's an autorelease object
	auto menu = Menu::create(vuelveAtras, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	// schedules update every frame with default order 0. Lower orders execute first
	this->scheduleUpdate();
	#pragma endregion

	// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Mis cosas
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// displays fps
	Director::getInstance()->setDisplayStats(true);

	showScores();

	return true;
}

#pragma region callbacks y sarna

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

#pragma endregion

void HiScores::update(float deltaT){
}

void HiScores::showScores(){
	auto gameInstance = Game::getInstance();
	auto visibleSize = Director::getInstance()->getVisibleSize();

	std::vector<Label *> nombres;
	std::vector<Label *> puntuaciones;

	// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// marcadores
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------

	// create a TTFConfig files for labels to share
	TTFConfig labelConfig;
	labelConfig.fontFilePath = "fonts/Marker Felt.ttf";
	labelConfig.fontSize = 24;
	labelConfig.glyphs = GlyphCollection::DYNAMIC;
	labelConfig.outlineSize = 0;
	labelConfig.customGlyphs = nullptr;
	labelConfig.distanceFieldEnabled = false;
	
	//// recordatorio de formatos
	//std::stringstream ss;
	//// puntos
	//ss << std::setw(6) << std::setfill('0') << 0;
	//lblPuntos->setString(ss.str());

	//// hiscore
	//ss.str(std::string()); // is technically more efficient, because you avoid invoking the std::string constructor that takes const char*
	//ss << std::setw(6) << std::setfill('0') << hiScore;
	//lblHiScore->setString(ss.str());

	Game::record tmpRecord;
	
	

	for(int i = 0; i < maxItems; i++){
		nombres.push_back(Label::createWithTTF(labelConfig, "MAXSIZEFORNAMES" + std::to_string(i)));
		puntuaciones.push_back(Label::createWithTTF(labelConfig, "MAXSIZEFORPOINTS" + std::to_string(i)));
	}

	float separaV = nombres[0]->getContentSize().height + 20.0f;
	float minV = 300.0f;
	float separaHNombres = 200.0f;
	float separaHPuntos = 500.0f;

	Label *tmp;
	int i = 0;
	for(auto iter = nombres.cbegin(); iter != nombres.cend(); ++iter,i++){
		tmp = (*iter);

		tmp->setPosition(Vec2(separaHNombres, visibleSize.height - minV - i * separaV));
		tmp->enableShadow();
		this->addChild(tmp, 1);
	}
	
	i = 0;
	for(auto iter = puntuaciones.cbegin(); iter != puntuaciones.cend(); ++iter, i++){
		tmp = (*iter);

		tmp->setPosition(Vec2(separaHPuntos, visibleSize.height - minV - i * separaV));
		tmp->enableShadow();
		this->addChild(tmp, 1);
	}


}