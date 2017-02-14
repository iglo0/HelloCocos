#include "Level1.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* Level1::createScene() {
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Level1::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Level1::init() {
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() ) {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// verlo en la output window
	CCLOG("visiablesize x: %f y: %f", visibleSize.width + origin.x, visibleSize.height + origin.y);

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(Level1::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
	
    /////////////////////////////
    // 3. add your codes below...

	Sprite* spriteNave = Sprite::create("spaceshipspr.png");
	//Sprite* spriteNave = Sprite::create("boss aircraft.png");

	// creo al prota en la parte inferior
	// reduzco a la mitad el sprite
	spriteNave->setScale(0.5f);
	// ojo al posicionarlo, que el tamaño es getScale * getContentSize
	spriteNave->setPosition(origin.x + visibleSize.width/2.0f, spriteNave->getScale()*spriteNave->getContentSize().height/2.0f);
	addChild(spriteNave,0);
	

	return true;
}


void Level1::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/
    
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
    
    
}
