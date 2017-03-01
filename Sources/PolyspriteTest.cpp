#include "polyspritetest.h"
#include "Menus.h"

/* CON FÍSICAS
Not only are physics engines great for simulating realistic movement, but they are also great for detecting collisions.
You’ll use Cocos2d-x’s physics engine to determine when monsters and projectiles collide.
*/

// Here you’ve created two types, Monster and Projectile, along with two special values to specify no type or all types.
// You’ll use these categories to assign types to your objects, allowing you to specify which types of objects are
// allowed to collide with each other.
enum class PhysicsCategory{
	// The category on Cocos2d-x is simply a single 32-bit integer; this syntax sets specific bits in the integer to
	// represent different categories, giving you 32 possible categories max. Here you set the first bit to indicate
	// a monster, the next bit over to represent a projectile, and so on.
	None = 0,
	Monster = (1 << 0),    // 1
	Projectile = (1 << 1), // 2
	All = PhysicsCategory::Monster | PhysicsCategory::Projectile
	// 3 WTF visual studio marca el | como erroneo pero lo calcula bien con el valor 3
};

cocos2d::Scene *PolyspriteTest::createScene(){
	// 'scene' is an autorelease object
	//auto scene = Scene::create();
	
	// This creates a Scene with physics enabled. Cocos2d-x uses a PhysicsWorld to control its physics simulation
	auto scene = Scene::createWithPhysics();
	// set the world’s gravity to zero in both directions, which essentially disables gravity
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
	// enable debug drawing to see your physics bodies
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	
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
	/////////////////////////////
	/////////////////////////////
	/////////////////////////////
	// esto es un sindios, lo se
	/////////////////////////////
	/////////////////////////////
	/////////////////////////////
	/////////////////////////////
	for(int i = 0; i < 20; i++){
		struct reboton tmp;
		PhysicsBody *physicsBody;
		//PolygonInfo tmpInfo;

		switch(cocos2d::RandomHelper::random_int(0, 2)){
		case 0:
			tmp.sprite = Sprite::create(ap1.generateTriangles());
			// TODO: temporalmente el physicsBody será una caja. Para esto hago los sprites poligonales :D
			physicsBody = PhysicsBody::createBox(Size(tmp.sprite->getContentSize().width, tmp.sprite->getContentSize().height), PhysicsMaterial(0.1f, 1.0f, 0.0f));
			// set the category, collision and contact test bit masks
			// tipo del objeto
			physicsBody->setCategoryBitmask((int)PhysicsCategory::Monster);
			// qué tipos de objeto generan eventos de colisión con este?
			physicsBody->setContactTestBitmask((int)PhysicsCategory::Projectile);
			//physicsBody->setContactTestBitmask((int)PhysicsCategory::All);
			break;
		case 1:
			tmp.sprite = Sprite::create(ap2.generateTriangles());
			physicsBody = PhysicsBody::createBox(Size(tmp.sprite->getContentSize().width, tmp.sprite->getContentSize().height), PhysicsMaterial(0.1f, 1.0f, 0.0f));
			// set the category, collision and contact test bit masks
			// tipo del objeto
			physicsBody->setCategoryBitmask((int)PhysicsCategory::Monster);
			// qué tipos de objeto generan eventos de colisión con este?
			physicsBody->setContactTestBitmask((int)PhysicsCategory::Projectile);
			break;
		case 2:
			tmp.sprite = Sprite::create(ap3.generateTriangles());
			physicsBody = PhysicsBody::createBox(Size(tmp.sprite->getContentSize().width, tmp.sprite->getContentSize().height), PhysicsMaterial(0.1f, 1.0f, 0.0f));
			// set the category, collision and contact test bit masks
			// tipo del objeto
			physicsBody->setCategoryBitmask((int)PhysicsCategory::Projectile);
			// qué tipos de objeto generan eventos de colisión con este?
			physicsBody->setContactTestBitmask((int)PhysicsCategory::Monster);
			break;
		default:
			CCLOG("WTF");
			tmp.sprite = Sprite::create(ap1.generateTriangles());
			physicsBody = PhysicsBody::createBox(Size(tmp.sprite->getContentSize().width, tmp.sprite->getContentSize().height), PhysicsMaterial(0.1f, 1.0f, 0.0f));
			// set the category, collision and contact test bit masks
			// tipo del objeto
			physicsBody->setCategoryBitmask((int)PhysicsCategory::Monster);
			// qué tipos de objeto generan eventos de colisión con este?
			physicsBody->setContactTestBitmask((int)PhysicsCategory::Projectile);
			break;
		}

		// crea sprites con el bounding box poligonal
		tmp.sprite->setPosition(Vec2(visibleSize.width / 2.0f + cocos2d::RandomHelper::random_real(-200.0f, 200.0f), visibleSize.height / 2.0f + cocos2d::RandomHelper::random_real(-200.0f, 200.0f)));
		tmp.sprite->setScale(0.5f);

		// Sets the sprite to be dynamic. This means that the physics engine will not apply forces to the
		// monster. Instead, you’ll control it directly through the MoveTo actions 
		physicsBody->setDynamic(true);

		// que objetos deberian afectar a este en las colisiones
		// (no afectaría por que es dinamico, está puesto por completar)
		physicsBody->setCollisionBitmask((int)PhysicsCategory::None);

		tmp.sprite->setPhysicsBody(physicsBody);



		// le pone una aceleracion estandar
		tmp.accel = Vec2(cocos2d::RandomHelper::random_real(-200.0f, 200.0f), cocos2d::RandomHelper::random_real(-200.0f, 200.0f));

		auto contactListener = EventListenerPhysicsContact::create();
		contactListener->onContactBegin = CC_CALLBACK_1(PolyspriteTest::onContactBegin, this);
		getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

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
		if(spr->isVisible()){
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
}


void PolyspriteTest::update(float deltaT){

	//megustaElMueveMueve(sprite1, sprite1Accel);
	//megustaElMueveMueve(sprite2, sprite2Accel);

	// ojo que quiero modificar la estructura, no me valen cbegin y cend
	for(auto r = rebotones.begin(); r != rebotones.end(); ++r){
		megustaElMueveMueve(r->sprite, r->accel);
	}

}


// physicscontact test
bool PolyspriteTest::onContactBegin(PhysicsContact &contactoConTacto){
	//CCLOG("Contacto");

	auto nodeA = contactoConTacto.getShapeA()->getBody()->getNode();
	auto nodeB = contactoConTacto.getShapeB()->getBody()->getNode();

	// ign mis tests
	nodeA->setVisible(false);
	nodeB->setVisible(false);

	//nodeA->removeFromParent();
	//nodeB->removeFromParent();
	// y esto va a cascar como un hijo de piii

	return true;
}
