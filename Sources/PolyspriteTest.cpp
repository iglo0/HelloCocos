#include "polyspritetest.h"
#include "Menus.h"

cocos2d::Scene * PolyspriteTest::createScene(){
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = PolyspriteTest::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool PolyspriteTest::init(){

	//////////////////////////////
	// 1. super init first
	if(!Layer::init()){
		return false;
	}

	/////////////////////////////

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(PolyspriteTest::MenuVuelveCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2, origin.y + closeItem->getContentSize().height/2));

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	/////////////////////////////

	const char *path1 = "spaceshipspr.png";
	const char *path2 = "Spaceship15.png";
	const char *path3 = "bullet_orange0001.png";
	//const char *path2 = "Spaceship15.png";

	AutoPolygon ap1 = AutoPolygon(path1);
	AutoPolygon ap2 = AutoPolygon(path2);
	AutoPolygon ap3 = AutoPolygon(path3);

	//PolygonInfo myInfo = ap1.generateTriangles();//use all default values
	//sprite1 = Sprite::create(myInfo);
	// 
	//// Por lo que he visto del parametro epsilon (5.0) -> a más alto menos detalle en el poly-bounding box
	////PolygonInfo myInfo2 = ap2.generateTriangles(Rect::ZERO, 5.0, 0.1);//ap can be reused to generate another set of PolygonInfo with different settings
	//PolygonInfo myInfo2 = ap2.generateTriangles();//use all default values
	//sprite2 = Sprite::create(myInfo2);

	//sprite1->setPosition(Vec2(visibleSize.width / 2.0f - 100.0f, visibleSize.height / 2.0f));
	//sprite2->setPosition(Vec2(visibleSize.width / 2.0f + 100.0f, visibleSize.height / 2.0f));

	//addChild(sprite1);
	//addChild(sprite2);

	//sprite1Accel = Vec2(-200.0f, 200.0f);
	//sprite2Accel = Vec2(200.0f, -200.0f);

	/////////////////////////////

	for(int i = 0; i < 200; i++){
		struct reboton tmp;
		PolygonInfo tmpInfo;

		switch(cocos2d::RandomHelper::random_int(0, 2)){
		case 0:
			tmpInfo = ap1.generateTriangles();
			break;
		case 1:
			tmpInfo = ap2.generateTriangles();
			break;
		case 2:
			tmpInfo = ap3.generateTriangles();
			break;
		default:
			CCLOG("WTF");
			tmpInfo = ap1.generateTriangles();
			break;
		}


		tmp.sprite = Sprite::create(tmpInfo);
		tmp.sprite->setPosition(Vec2(visibleSize.width / 2.0f + cocos2d::RandomHelper::random_real(-200.0f, 200.0f), visibleSize.height / 2.0f + cocos2d::RandomHelper::random_real(-200.0f, 200.0f)));
		tmp.sprite->setScale(0.5f);
		tmp.accel = Vec2(cocos2d::RandomHelper::random_real(-200.0f, 200.0f), cocos2d::RandomHelper::random_real(-200.0f, 200.0f));


		rebotones.push_back(tmp);

		addChild(tmp.sprite,i);
	}



	/////////////////////////////
	scheduleUpdate();								// schedules update with order "0" (each frame)
	Director::getInstance()->setDisplayStats(true); // displays fps

	/////////////////////////////
	return true;
}

void PolyspriteTest::MenuVuelveCallback(cocos2d::Ref * pSender){
	Director::getInstance()->replaceScene(Menus::CreateScene());
}

void PolyspriteTest::megustaElMueveMueve(Sprite *spr, Vec2 &cant){
	if(spr){
		spr->setPosition(spr->getPosition() + cant * Director::getInstance()->getDeltaTime());

		// efecto rebote al llegar a los bordes
		Vec2 tmp = spr->getPosition();

		if(tmp.x < 0 || tmp.x>Director::getInstance()->getVisibleSize().width){
			cant.x = -cant.x;
		}

		if(tmp.y < 0 || tmp.y>Director::getInstance()->getVisibleSize().height){
			cant.y = -cant.y;
		}
	}
}


void PolyspriteTest::update(float deltaT){

	//megustaElMueveMueve(sprite1, sprite1Accel);
	//megustaElMueveMueve(sprite2, sprite2Accel);

	// ojo que quiero modificar la estructura, no me valen cbegin y cend
	for(auto r = rebotones.begin(); r != rebotones.end(); ++r){
		megustaElMueveMueve(r->sprite, r->accel);
	}

}


