#include "polyspritetest.h"

#include "Menus.h"
#include "PhysicsShapeCache.h"


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

	// super init first
	if(!Layer::init()){
		return false;
	}

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
	//// Fisicas de PhysicsEditor
	//PhysicsShapeCache *physicsShapeCache;
	//physicsShapeCache = PhysicsShapeCache::getInstance();
	//// hacer esto una sola vez
	//// en este fichero estarán colecciones de polígonos que luego usará la parte de colisiones
	//physicsShapeCache->addShapesWithFile("physicscollision.plist");
	
	llenaPantallaDeSprites();
	/////////////////////////////

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(PolyspriteTest::onContactBegin, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

	scheduleUpdate();								// schedules update with order "0" (each frame)
	Director::getInstance()->setDisplayStats(true); // displays fps
	return true;
}

void PolyspriteTest::llenaPantallaDeSprites(){
	struct reboton tmp;
	PhysicsBody *physicsBody;
	auto visibleSize = Director::getInstance()->getVisibleSize();

	/////////////////////////////
	// Fisicas de PhysicsEditor
	PhysicsShapeCache *physicsShapeCache;
	physicsShapeCache = PhysicsShapeCache::getInstance();

	// nombre del sprite != nombre dentro del plist de physics. Quicir, que puedo usar cualquiera en cualquiera
	//const char *path1 = "collision_player.png";
	//const char *path2 = "collision_enemy.png";
	//const char *path3 = "collision_enemy_bullet.png";
	const char *path1 = "spaceshipspr.png";
	const char *path2 = "aliensprite2.png";
	const char *path3 = "bullet_orange0000.png";

	// como crear un autopolygon "DEPRECATED" --> TexturePacker se encarga de esto
	//AutoPolygon ap1 = AutoPolygon(path1);
	//PolygonInfo myInfo = ap1.generateTriangles();//use all default values
	//sprite1 = Sprite::create(myInfo);

	for(int i = 0; i < 60; i++){
		switch(cocos2d::RandomHelper::random_int(0, 2)){
		case 0:
			//tmp.sprite = creaSpriteColision(path1, "collision_player", (int)PhysicsCategory::Monster, (int)PhysicsCategory::Projectile);
			tmp.sprite = creaSpriteColision(path1, path1, (int)PhysicsCategory::Monster, (int)PhysicsCategory::Projectile);
			tmp.sprite->setScale(0.5f);
			break;
		case 1:
			//tmp.sprite = creaSpriteColision(path2, "collision_enemy", (int)PhysicsCategory::Monster, (int)PhysicsCategory::Projectile);
			tmp.sprite = creaSpriteColision(path2, path2, (int)PhysicsCategory::Monster, (int)PhysicsCategory::Projectile);
			tmp.sprite->setScale(0.5f);
			break;
		case 2:
			//tmp.sprite = creaSpriteColision(path3, "collision_enemy_bullet", (int)PhysicsCategory::Projectile, (int)PhysicsCategory::Monster);
			tmp.sprite = creaSpriteColision(path3, path3, (int)PhysicsCategory::Projectile, (int)PhysicsCategory::Monster);
			tmp.sprite->setScale(1.0f);
			break;
		default:
			//CCLOG("WTF"); tmp.sprite = creaSpriteColision(path1, "collision_player", (int)PhysicsCategory::Monster, (int)PhysicsCategory::Projectile);
			CCLOG("WTF"); tmp.sprite = creaSpriteColision(path1, path1, (int)PhysicsCategory::Monster, (int)PhysicsCategory::Projectile);
			tmp.sprite->setScale(0.5f);
			break;
		}

		tmp.sprite->setPosition(Vec2(visibleSize.width / 2.0f + cocos2d::RandomHelper::random_real(-600.0f, 600.0f), visibleSize.height / 2.0f + cocos2d::RandomHelper::random_real(-600.0f, 600.0f)));

		// le pone una aceleracion estandar
		tmp.accel = Vec2(cocos2d::RandomHelper::random_real(-200.0f, 200.0f), cocos2d::RandomHelper::random_real(-200.0f, 200.0f));

		rebotones.push_back(tmp);

		addChild(tmp.sprite, i);
	}
}

void PolyspriteTest::MenuVuelveCallback(cocos2d::Ref * pSender){
	// limpia el plist en memoria
	//PhysicsShapeCache::getInstance()->removeAllShapes();

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

	// en plan guarro, oculta los sprites aunque los colliders siguen activos
	nodeA->setVisible(false);
	nodeB->setVisible(false);

	return true;
}

Sprite *PolyspriteTest::creaSpriteColision(const char *spritePath, const char *physics_name, int claseColision, int colisionoCon ) {
	Sprite *sprite;
	PhysicsBody *physicsBody;
	auto physicsShapeCache = PhysicsShapeCache::getInstance();

	sprite = Sprite::createWithSpriteFrameName(spritePath);

	// Si hay shapecache para este sprite, usar este, si no usar createBox.
	// Ojo: physicsBody es undefined ahora, pasará a ser "null" si SetBodyOnSprite no lo consigue
	physicsBody = physicsShapeCache->ignSetBodyOnSprite(physics_name, sprite);
	if(!physicsBody){
		// physicsBody == null -> sheeit, ponerle una caja
		physicsBody = PhysicsBody::createBox(Size(sprite->getContentSize().width, sprite->getContentSize().height), PhysicsMaterial(0.1f, 1.0f, 0.0f));
	}

	// set the category, collision and contact test bit masks
	// tipo del objeto
	physicsBody->setCategoryBitmask(claseColision);
	// qué tipos de objeto generan eventos de colisión con este?
	physicsBody->setContactTestBitmask(colisionoCon);


	// Sets the sprite to be dynamic. This means that the physics engine will not apply forces to it. Instead, you’ll control it directly.
	physicsBody->setDynamic(true);
	// que objetos deberian afectar a este en las colisiones (no afectaría por que es dinamico, está puesto por completar)
	// OJO: esta parte no entiendo a qué se refiere
	physicsBody->setCollisionBitmask((int)PhysicsCategory::None);

	sprite->setPhysicsBody(physicsBody);

	return sprite;
}