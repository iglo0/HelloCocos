#include "Menus.h"
#include "Level1.h"

USING_NS_CC; // using namespace cocos2d

cocos2d::Scene * Menus::CreateScene(){
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = Menus::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool Menus::init(){

	//////////////////////////////
	// 1. super init first
	if(!Layer::init()){
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	#pragma region boton cocos
	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(Menus::MenuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2, origin.y + closeItem->getContentSize().height/2));

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
	#pragma endregion
	
	/////////////////////////////
	// 3. add your codes below...

	#pragma region mis cosas
	// creating a Menu from a Vector of items
	Vector<MenuItem*> menuItems;

	//auto menuJugar = MenuItemImage::create("uno.png", "dos.png", CC_CALLBACK_1(Menus::MenuJugarCallback, this));
	//auto menuSalir = MenuItemImage::create("uno.png", "dos.png", CC_CALLBACK_1(Menus::MenuJugarCallback, this));

	// Ojo la Y va p'arriba
	auto menuJugar = MenuItemFont::create("Jugar", CC_CALLBACK_1(Menus::MenuJugarCallback, this));
	menuJugar->setPosition(0.0f, menuJugar->getContentSize().height * 2.0f);
	menuItems.pushBack(menuJugar);

	auto menuSalir = MenuItemFont::create("Salir", CC_CALLBACK_1(Menus::MenuCloseCallback, this));
	menuSalir->setPosition(0.0f, menuSalir->getContentSize().height * 1.0f);
	menuItems.pushBack(menuSalir);

	/* repeat for as many menu items as needed */

	auto miMenu = Menu::createWithArray(menuItems);
	//miMenu->setPosition(100.0f, 200.0f);
	this->addChild(miMenu, 1);

	#pragma endregion

	/*
	// add a label shows "Hello World"
	// create and initialize a label

	
	float menuItems = 0.0f;
	float interLin = 1.1f;

	auto lblMenuTitle = Label::createWithTTF("Menuses", "fonts/Marker Felt.ttf", 24);
	// position the label on the center of the screen
	// coordenadas 0,0 = centro
	lblMenuTitle->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - lblMenuTitle->getContentSize().height * ++menuItems*interLin));
	// add the label as a child to this layer
	this->addChild(lblMenuTitle, 1);
	
	auto lblJugar = Label::createWithTTF("Jugar", "fonts/Marker Felt.ttf", 24);
	lblJugar->setPosition(Vec2(origin.x + lblJugar->getContentSize().width / 2 + 10.0f, origin.y + visibleSize.height - lblJugar->getContentSize().height * ++menuItems*interLin));
	this->addChild(lblJugar, 1);

	auto lblSalir = Label::createWithTTF("Salir", "fonts/Marker Felt.ttf", 24);
	lblSalir->setPosition(Vec2(origin.x + lblSalir->getContentSize().width / 2 + 10.0f, origin.y + visibleSize.height - lblSalir->getContentSize().height * ++menuItems*interLin));
	this->addChild(lblSalir, 1);
	

	// create menu, it's an autorelease object
	auto imgPisha = MenuItemImage::create("uno.png", "dos.png", CC_CALLBACK_1(Menus::MenuJugarCallback, this));
	imgPisha->setPosition(lblJugar->getPosition());
	auto menuPisha = Menu::create(imgPisha, NULL);
	menuPisha->setPosition(Vec2::ZERO);
	this->addChild(menuPisha, 0);	// por debajo de los label

	auto imgShosho = MenuItemImage::create("uno.png", "dos.png", CC_CALLBACK_1(Menus::MenuJugarCallback, this));
	imgShosho->setPosition(lblSalir->getPosition());
	auto menuShosho = Menu::create(imgShosho, NULL);
	menuShosho->setPosition(Vec2::ZERO);
	this->addChild(menuShosho, 0);

	*/

	// add "HelloWorld" splash screen"
	//auto sprite = Sprite::create("HelloWorld.png");

	// position the sprite on the center of the screen
	//sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

	// add the sprite as a child to this layer
	//this->addChild(sprite, 0);


	return true;
}

void Menus::MenuCloseCallback(cocos2d::Ref * pSender){
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif

	/*To navigate back to native iOS screen(if present) without quitting the application,
	do not use Director::getInstance()->end() and exit(0) as given above,instead trigger
	a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);

}

void Menus::MenuJugarCallback(cocos2d::Ref *pSender){
	// TODO
	// sale para que se note O:-)

	// http://www.cocos2d-x.org/wiki/Building_and_Transitioning_Scenes
	auto scene = Level1::createScene();
	auto director = Director::getInstance();
	director->replaceScene(scene);

	
}