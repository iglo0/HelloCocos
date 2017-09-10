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
	
	xmlLoadTest("test.xml");

	// Oookay tengo animaciones_ cargado. Y ahora?
	animaciones_->playStart("default");

	/*
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
	*/
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

	mueveNodo();

	if(animaciones_){
		animaciones_->update(deltaT);
	}
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

#pragma region xml cosaz

// OJO: unused
void AnimTest::xmlSaveTest(const char *filename){
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filename);

	pugi::xml_node xml_root = doc.child("default_values");

	pugi::xml_document docWrite;
	auto declarationNode = docWrite.append_child(pugi::node_declaration);
	declarationNode.append_attribute("version") = "0.1";
	declarationNode.append_attribute("encoding") = "ISO-8859-1";
	declarationNode.append_attribute("standalone") = "yes";

	auto root = docWrite.append_child("MyRoot");

	// Append some child elements below root
	// Add as last element
	pugi::xml_node nodeChild = root.append_child("MyChild");
	nodeChild.append_attribute("hint") = "inserted as last child";
	nodeChild.append_attribute("intVal") = 5;
	// Add as last element
	nodeChild = root.append_child("MyChild");
	nodeChild.append_attribute("hint") = "also inserted as last child";
	nodeChild.append_attribute("doubleVal") = 2.03;
	// Add as first element
	nodeChild = root.prepend_child("MyChild");
	nodeChild.append_attribute("hint") = "inserted at front";
	nodeChild.append_attribute("boolVal") = false;


	pugi::xml_node childrenWithValues = root.append_child("ChildrenWithValue");
	// Add child of type integer
	pugi::xml_node nodeChildWithValues = childrenWithValues.append_child("MyChildWithIntValue");
	nodeChildWithValues.append_child(pugi::node_pcdata).set_value(std::to_string(4712).c_str());
	// Add child of type double
	nodeChildWithValues = childrenWithValues.append_child("MyChildWithDoubleValue");
	nodeChildWithValues.append_child(pugi::node_pcdata).set_value(std::to_string(3.18).c_str());
	// Add child of type bool
	nodeChildWithValues = childrenWithValues.append_child("MyChildWithBoolValue");
	nodeChildWithValues.append_child(pugi::node_pcdata).set_value(std::to_string(false).c_str());


	bool saveSucceeded = docWrite.save_file(filename, PUGIXML_TEXT("  "));
	assert(saveSucceeded);

}

void AnimTest::xmlLoadTest(const char *filename){
	/*
	* xml_node is the handle to document node; it can point to any node in the document, including document itself.

	* There is a special value of xml_node type, known as null node or empty node. It does not correspond to any node in any document, and thus resembles null pointer. 
	However, all operations are defined on empty nodes; generally the operations don’t do anything and return empty nodes/attributes or empty strings as their result. 
	This is useful [...] you can get the grandparent of a node like so: node.parent().parent(); 
	if a node is a null node or it does not have a parent, the first parent() call returns null node; the second parent() call then also returns null node, so you don’t have to check for errors twice.

	* You can test if a handle is null via implicit boolean cast: if (node) { …? } or if (!node) { …? }.
	
	* xml_attribute is the handle to an XML attribute; it has the same semantics as xml_node
	
	* You can get node or attribute name via name() accessor, and value via value() accessor. Note that both functions never return null pointers
	they either return a string with the relevant content, or an empty string if name/value is absent or if the handle is null.

	Also there are two notable things for reading values:

    * It is common to store data as text contents of some node - i.e. <node><description>This is a node< / description>< / node>.
	In this case, <description> node does not have a value, but instead has a child of type node_pcdata with value "This is a node".
	pugixml provides child_value() and text() helper functions to parse such data.

	In many cases attribute values have types that are not strings - i.e.an attribute may always contain values that should be treated as integers, 
	despite the fact that they are represented as strings in XML.pugixml provides several accessors that convert attribute value to some other type.
	*/
	
	auto directorInstance = Director::getInstance();

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filename);

	if(!result){
		// error! salir o algo
		CCLOG("Error parseando xml %s", filename);
		return;
	}

	char *animSetName, *animName, *framePath;
	float frameWait, spriteScale;
	bool animLoop;

	animaciones_ = new AnimSprites(Vec2(directorInstance->getWinSize().width/2.0f, directorInstance->getWinSize().height/2.0f));
	AnimSprites::frame *tmpFrame;
	AnimSprites::animation *tmpAnimation;

	// al nivel raíz del xml tengo default_values que uso para la configuración y al mismo nivel varios "animset" que definen un set de animaciones
	for(pugi::xml_node xmlAnimSet = doc.child("animset"); xmlAnimSet; xmlAnimSet = xmlAnimSet.next_sibling("animset")){
		// encontrado un <animset>. Dentro debería haber 0..n <anim>s y 0..m <frame>s

		// animSet tiene name y nada más
		animSetName = (char *)xmlAnimSet.attribute("name").value();

		// dentro debería haber varios anim
		for(pugi::xml_node xmlAnim = xmlAnimSet.child("anim"); xmlAnim; xmlAnim = xmlAnim.next_sibling("anim")){

			// cada anim de animSet tiene un nombre para la animación, atributos como si es loop o no y la lista de frames
			animName = (char *)xmlAnim.attribute("name").value();
			animLoop = xmlAnim.attribute("loop").as_bool();

			// voy creando una estructura en memoria para el juego
			tmpAnimation = new AnimSprites::animation(animLoop);

			// busco los frames que hay y los asigno
			for(pugi::xml_node xmlFrame = xmlAnim.child("frame"); xmlFrame; xmlFrame = xmlFrame.next_sibling("frame")) {

				// ruta al archivo del sprite
				framePath = (char *)xmlFrame.attribute("path").value();
				frameWait = xmlFrame.attribute("wait").as_float();
				spriteScale = xmlFrame.attribute("size").as_float();

				tmpFrame = new AnimSprites::frame(this, framePath, frameWait, spriteScale);
				// addFrame añade el frame a la animación si existe en el set (por nombre), o crea una nueva si no.
				tmpAnimation->addFrame(tmpFrame);
			}
		}

		animaciones_->addAnimation(animName, tmpAnimation);
	}
}

#pragma endregion