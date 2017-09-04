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

	There is a special value of xml_node type, known as null node or empty node. It does not correspond to any node in any document, and thus resembles null pointer. 
	However, all operations are defined on empty nodes; generally the operations don’t do anything and return empty nodes/attributes or empty strings as their result. 
	This is useful [...] you can get the grandparent of a node like so: node.parent().parent(); 
	if a node is a null node or it does not have a parent, the first parent() call returns null node; the second parent() call then also returns null node, so you don’t have to check for errors twice.

	You can test if a handle is null via implicit boolean cast: if (node) { …? } or if (!node) { …? }.

	* xml_attribute is the handle to an XML attribute; it has the same semantics as xml_node

	* load_file, as well as other loading functions, destroys the existing document tree and then tries to load the new tree from the specified file. 
	The result of the operation is returned in an xml_parse_result object

	* You can get node or attribute name via name() accessor, and value via value() accessor. Note that both functions never return null pointers
	they either return a string with the relevant content, or an empty string if name/value is absent or if the handle is null.

	Also there are two notable things for reading values:

    It is common to store data as text contents of some node - i.e. <node><description>This is a node< / description>< / node>.
	In this case, <description> node does not have a value, but instead has a child of type node_pcdata with value "This is a node".
	pugixml provides child_value() and text() helper functions to parse such data.

	In many cases attribute values have types that are not strings - i.e.an attribute may always contain values that should be treated as integers, 
	despite the fact that they are represented as strings in XML.pugixml provides several accessors that convert attribute value to some other type.

	*/


	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filename);

	if(!result){
		// error! salir o algo
		CCLOG("Error parseando xml %s", filename);
		return;
	}

	// al nivel raíz del xml tengo default_values que uso para la configuración y al mismo nivel varios "animset" que definen un set de animaciones
	for(pugi::xml_node animset = doc.child("animset"); animset; animset = animset.next_sibling("animset")){
		// aquí hay un animset

		CCLOG("animset name=%s", animset.attribute("name").value());

		// dentro debería haber varios anim
		for(pugi::xml_node anim = animset.child("anim"); anim; anim = anim.next_sibling("anim")){

			CCLOG("    name=%s, loop=%s", anim.attribute("name").value(), anim.attribute("loop").value());

			// dentro de cada anim, varios frame
			for(pugi::xml_node frame = anim.child("frame"); frame; frame = frame.next_sibling("frame")) {
				// y aquí por fin están cada frame que necesita la animación
				CCLOG("        path=%s t=%s", frame.attribute("path").value(), frame.attribute("wait").value());
			}

		}

	}

	//pugi::xml_node xml_root = doc.child("default_values");
	//pugi::xml_node xml_root = doc.child("animset");

	//auto x = xml_root.child_value("bullet_t1_anim");
	
	// next_sibling	"lateral traversing"
	// next_child, parent "vertical traversing"
	// 
	

	//player_initial_speed = atof(xml_default_values.child_value(CONFIG_PLAYER_INITIAL_SPEED));


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




}

#pragma endregion