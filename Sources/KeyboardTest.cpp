#include "KeyboardTest.h"

KeyboardTest::KeyboardTest(){
}

KeyboardTest::~KeyboardTest(){
	_label->release();
}


Scene* KeyboardTest::createScene(){
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = KeyboardTest::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool KeyboardTest::init(){

#pragma region inits
	//////////////////////////////
	// 1. super init first

	if(!Layer::init()){
		return false;
	}

	auto s = Director::getInstance()->getWinSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto label = LabelTTF::create("Keyboard Test", "fonts/Arial.ttf", 28);
	addChild(label, 0);
	label->setPosition(Point(s.width / 2, s.height - 50));



	//////////////////////////////
	// 2. keyboard

	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(KeyboardTest::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(KeyboardTest::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	// create a label to display the tip string
	_label = LabelTTF::create("Please press any key and see console log...", "Arial", 22);
	_label->setPosition(Point(s.width / 2, s.height / 2));
	addChild(_label, 0);
	_label->retain();	// esto no estoy seguro de si hace algo util para mi ahora mismo



	/////////////////////////////
	// 3. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(KeyboardTest::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + s.width - closeItem->getContentSize().width / 2, origin.y + closeItem->getContentSize().height / 2));

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);




	/////////////////////////////
	// 4. add your codes below...




	return true;
}

void KeyboardTest::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event){
	CCLOG("Key with keycode %d pressed", keyCode);
}

void KeyboardTest::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event){
	CCLOG("Key with keycode %d released", keyCode);
}

void KeyboardTest::menuCloseCallback(Ref* pSender){
	// vuelve al menu
	auto scene = Menus::CreateScene();
	auto director = Director::getInstance();
	director->replaceScene(scene);
}


//void KeyboardTestScene::runThisTest(){
//	auto layer = new KeyboardTest();
//	addChild(layer);
//
//	Director::getInstance()->replaceScene(this);
//	layer->release();
//}