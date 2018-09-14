#include "HiScoresScene.h"
#include "HiScores.h"
#include "Menus.h"
#include "Game.h"
#include "XmlHelper.h"

//#include <iostream>	// para usar std::cout << ... << std::setfill('0') << 4 ... (leading zeros)
//#include <iomanip>	// para usar std::cout << ... << std::setfill('0') << 4 ... (leading zeros)


HiScoresScene::~HiScoresScene(){
	Director::getInstance()->setDisplayStats(false);

}

Scene* HiScoresScene::createScene(){
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
	auto layer = HiScoresScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HiScoresScene::init(){
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
	listener->onKeyPressed = CC_CALLBACK_2(HiScoresScene::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(HiScoresScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Botón de vuelta atrás
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
	auto vuelveAtras = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(HiScoresScene::menuVuelveCallback, this));
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

void HiScoresScene::menuVuelveCallback(Ref *pSender){
	// vuelve al menu
	//Director::getInstance()->replaceScene(Menus::CreateScene());

	vuelveAlMenu();
}

void HiScoresScene::vuelveAlMenu(){
	Director::getInstance()->replaceScene(Menus::createScene());
}

void HiScoresScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event){
	CCLOG("Keypressed");

	switch(keyCode){
	case EventKeyboard::KeyCode::KEY_ESCAPE:
		vuelveAlMenu();
		break;
	default:
		break;
	}
}

void HiScoresScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event){
	CCLOG("Keyreleased");
}

#pragma endregion

void HiScoresScene::update(float deltaT){
}

// Coge la lista de records y lo primero que se me ha ocurrido es dividirla en tres vectores distintos, con la intención de poder sacarlos con distintas fuentes.
void HiScoresScene::showScores(){
	auto gameInstance = Game::getInstance();
	auto visibleSize = Director::getInstance()->getVisibleSize();

	// ÑAPA: limitar el nº de récords que se muestran pero permitiendo que la lista guarde cienes y cienes de ellos
	int limiteMostrar = 10;
	int mostrados = 0;

	//// Estos vectores tendrán la lista de nombre, puntuaciones y niveles de cada record. En plan: MANOLETE 002000 03
	//std::vector<Label *> nombres;
	//std::vector<Label *> puntuaciones;
	//std::vector<Label *> niveles;

	// create a TTFConfig files for labels to share
	TTFConfig labelConfig;
	labelConfig.fontFilePath = "fonts/Marker Felt.ttf";
	labelConfig.fontSize = 24;
	labelConfig.glyphs = GlyphCollection::DYNAMIC;
	labelConfig.outlineSize = 0;
	labelConfig.customGlyphs = nullptr;
	labelConfig.distanceFieldEnabled = false;

	std::stringstream ss;
	Label *lblTemp;
	HiScores::record rTmp;
	float filaActual = visibleSize.height - 100.0f;
	float anchoX, anchoY, anchoZ;
	float iniX, iniY, iniZ;

	// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// títulos
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------

	ss << std::setw(3) << "NOMBRE";
	lblTemp = Label::createWithTTF(labelConfig, ss.str());
	//anchoX = lblTemp->getContentSize().width;
	anchoX = 100.0f;
	// si divido la pantalla en 4, me interesan las 3 "x" del medio
	// x---x---x---x---x
	// . 1 . 2 . 3 . 4 .
	// 0  <1> <2> <3>  4
	iniX = 1 * visibleSize.width / 4.0f;
	lblTemp->setColor(Color3B::RED);
	lblTemp->setPosition(Vec2(iniX, filaActual));
	lblTemp->enableShadow();
	this->addChild(lblTemp);


	// For clearing the contents of a stringstream, using:
	// m.str(""); is correct, although using:
	// m.str(std::string());
	// is technically more efficient, because you avoid invoking the std::string constructor that takes const char*.
	// But any compiler these days should be able to generate the same code in both cases - so I would just go with whatever is more readable.
	ss.str(std::string());
	ss << std::setw(6) << "PUNTOS";
	lblTemp = Label::createWithTTF(labelConfig, ss.str());
	//anchoY = lblTemp->getContentSize().width;
	anchoY = 100.0f;
	iniY = 2 * visibleSize.width / 4.0f;
	lblTemp->setColor(Color3B::RED);
	lblTemp->setPosition(Vec2(iniY, filaActual));
	lblTemp->enableShadow();
	this->addChild(lblTemp);

	ss.str(std::string());
	ss << std::setw(6) << "NIVEL";
	lblTemp = Label::createWithTTF(labelConfig, ss.str());
	//anchoZ = lblTemp->getContentSize().width;
	anchoZ = 100.0f;
	iniZ = 3 * visibleSize.width / 4.0f;
	lblTemp->setColor(Color3B::RED);
	lblTemp->setPosition(Vec2(iniZ, filaActual));
	lblTemp->enableShadow();
	this->addChild(lblTemp);

	// linea de separacion
	lblTemp = Label::createWithTTF(labelConfig, "-----------------------------------------------------------------------------------");
	lblTemp->setColor(Color3B::RED);
	lblTemp->setPosition(Vec2(visibleSize.width / 2.0f, filaActual - 30.0f));	// la línea, en el medio
	lblTemp->enableShadow();
	this->addChild(lblTemp);


	// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// muestro los marcadores
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
	int fila = 4;	// que empiece un poco más abajo (el título lleva una pos vcal absoluta)
	for(auto it = gameInstance->hiScores_->tablaRecords.cbegin(); it != gameInstance->hiScores_->tablaRecords.cend(); ++it){
		rTmp = it->second;

		filaActual = visibleSize.height - fila * 050.0f;

		// nombre
		lblTemp = Label::createWithTTF(labelConfig, rTmp.name);
		lblTemp->setColor(Color3B::YELLOW);
		lblTemp->setPosition(Vec2(iniX, filaActual));
		lblTemp->enableShadow();
		this->addChild(lblTemp);

		// puntos
		ss.str(std::string()); 
		ss << std::setw(6) << std::setfill('0') << rTmp.puntos;
		lblTemp = Label::createWithTTF(labelConfig, ss.str());
		lblTemp->setColor(Color3B::WHITE);
		lblTemp->setPosition(Vec2(iniY, filaActual));
		lblTemp->enableShadow();
		this->addChild(lblTemp);

		// niveles
		ss.str(std::string());
		ss << std::setw(2) << std::setfill('0') << rTmp.nivelAlcanzado;
		lblTemp = Label::createWithTTF(labelConfig, ss.str());
		lblTemp->setColor(Color3B::WHITE);
		lblTemp->setPosition(Vec2(iniZ, filaActual));
		lblTemp->enableShadow();
		this->addChild(lblTemp);

		++fila;

		// empieza en 1, pregunto si tengo que mostrar el 2 y así sucesivamente
		if(++mostrados >= limiteMostrar){
			break;
		}

	}

}
