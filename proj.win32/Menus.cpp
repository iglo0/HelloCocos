#include "Menus.h"
#include "Level1.h"
#include "KeyboardTest.h"

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

	// Ojo la Y va p'arriba

	// menus en orden inverso, pero la numeracion (*1.0f, *2.0f ...) va en orden y asi me es mas facil
	auto menuSalir = MenuItemFont::create("Salir", CC_CALLBACK_1(Menus::MenuCloseCallback, this));
	menuSalir->setPosition(0.0f, menuSalir->getContentSize().height * 1.0f);
	menuItems.pushBack(menuSalir);

	auto menuKeyTest = MenuItemFont::create("Keyboard Test", CC_CALLBACK_1(Menus::MenuKeyTestCallback, this));
	menuKeyTest->setPosition(0.0f, menuKeyTest->getContentSize().height * 2.0f);
	menuItems.pushBack(menuKeyTest);

	auto menuJugar = MenuItemFont::create("Jugar", CC_CALLBACK_1(Menus::MenuJugarCallback, this));
	menuJugar->setPosition(0.0f, menuJugar->getContentSize().height * 3.0f);
	menuItems.pushBack(menuJugar);

	/* repeat for as many menu items as needed */

	auto miMenu = Menu::createWithArray(menuItems);
	//miMenu->setPosition(100.0f, 200.0f);
	this->addChild(miMenu, 1);

	#pragma endregion




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

	// http://www.cocos2d-x.org/wiki/Building_and_Transitioning_Scenes
	//auto scene = Level1::createScene();
	auto scene = Level1::createScene();
	auto director = Director::getInstance();
	director->replaceScene(scene);


}

void Menus::MenuKeyTestCallback(cocos2d::Ref *pSender){

	// http://www.cocos2d-x.org/wiki/Building_and_Transitioning_Scenes
	//auto scene = Level1::createScene();
	auto scene = KeyboardTest::createScene();
	auto director = Director::getInstance();
	director->replaceScene(scene);


}