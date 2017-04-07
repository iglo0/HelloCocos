#include "Level.h"

Level::~Level(){
	Director::getInstance()->setDisplayStats(false);
}


Scene* Level::createScene(){
	// NUEVO: físicas!
	// Gracias a las físicas obtengo colisiones "gratis", y como un juego tipo space invaders 
	// no creo que tenga muchos problemas de rendimiento... tiraré por ahí a ver
	// ----------------------------------------------------------------------------------------------------------------------------------------
	// INI del motor de físicas Chipmunk
	auto scene = Scene::createWithPhysics();
	// 'scene' is an autorelease object
	//auto scene = Scene::create();

	// set the world’s gravity to zero in both directions, which essentially disables gravity
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
	// enable debug drawing to see your physics bodies
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	// ----------------------------------------------------------------------------------------------------------------------------------------

	// 'layer' is an autorelease object
	auto layer = Level::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool Level::init(){

	// ----------------------------------------------------------------------------------------------------------------------------------------
	// inits
	// ----------------------------------------------------------------------------------------------------------------------------------------
	if(!Layer::init()){
		return false;
	}

	gameInstance = Game::getInstance();

	//auto visibleSize = Director::getInstance()->getVisibleSize();
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	// ----------------------------------------------------------------------------------------------------------------------------------------
	// eventos
	// ----------------------------------------------------------------------------------------------------------------------------------------
	// input listeners
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(Level::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(Level::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	// contact listener
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Level::onContactBegin, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

	// ----------------------------------------------------------------------------------------------------------------------------------------
	// GUI
	// ----------------------------------------------------------------------------------------------------------------------------------------

	// Botón de vuelta atrás
	auto vuelveAtras = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(Level::menuVuelveCallback, this));
	vuelveAtras->setPosition(Vec2(origin.x + visibleSize.width - vuelveAtras->getContentSize().width / 2, origin.y + vuelveAtras->getContentSize().height / 2));
	// create menu, it's an autorelease object
	auto menu = Menu::create(vuelveAtras, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	// ----------------------------------------------------------------------------------------------------------------------------------------
	// otras inicializaciones
	// ----------------------------------------------------------------------------------------------------------------------------------------

	gameInstance->ellapsedTime = 0;

	// Indica el estado inicial de esta escena
	gameInstance->estadoActual = Game::estadosJuego::introNivel;

	// ========================================================================================================================================
	// testz
	// ----------------------------------------------------------------------------------------------------------------------------------------

	// instancio un jugador con los valores por defecto
	player = new Player(this, PLAYER_INITIAL_SPEED);
	// Iba a colgar el inputComponent del jugador, quizá esté mejor colgando del nivel y con una referencia al jugador
	inputComponent = new InputComponent;
	inputComponent->player = player;

	// probando 1,2,3
	// pruebo la clase weapon, que gestionará las balas
	player->currentWeapon = new Weapon;
	player->currentWeapon->createBulletPool(
		this, 16,"bala_",BULLET_PATH_SPRITE1,BULLET_PATH_SOUND_FIRE,BULLET_PATH_SOUND_IMPACT,BULLET_DEFAULT_SPEED,BULLET_DEFAULT_DMG,
		(int)Game::CategoriaColision::Bala, (int)Game::CategoriaColision::Enemigo, 1.0f);


	// Pruebo el nuevo Enemy:GameActor 
	// lo inicializo y le asigno un comportamiento
	enemy = new Enemy(this, ENEMY_T1_PATH_SPRITE, ENEMY_PATH_SOUND_DIE, ENEMY_T1_INITIAL_SIZE * 2.0f, ENEMY_T1_INITIAL_ROTATION, ENEMY_BOSS_GENERIC_HP);
	// situo al enemigo arriba en el medio, con medio cuerpo de margen superior
	Vec2 enePos = Vec2(visibleSize.width / 2.0f, visibleSize.height - enemy->getSprite()->getContentSize().height);
	enemy->activa(enePos);
	enemy->weapon = new Weapon;
	enemy->weapon->createBulletPool(this, 3, "balaEne_", BULLET_PATH_SPRITE2, BULLET_PATH_SOUND_FIRE, BULLET_PATH_SOUND_IMPACT, -BULLET_DEFAULT_SPEED, BULLET_DEFAULT_DMG * 2.0f,
		(int)Game::CategoriaColision::BalaEnemigo, (int)Game::CategoriaColision::Jugador, 3.0f);

	// Cómo querré que se mueva?
	//auto funcionControlMovimiento = &GameActor::mueveSeno;
	enemy->funcionMovimientoActual = &GameActor::mueveSeno;
	enemy->funcionMovimientoAmplitude = 600.0;
	enemy->funcionMovimientoPosIni = Vec2(Director::getInstance()->getVisibleSize().width / 2.0f, enemy->getPosition().y);
	
	// y que ataque?
	enemy->funcionControlActual = &Enemy::funControl1;
	enemy->funcionControlTiempoDisparo = 0.1f;

	// hale, definido



	// cómo querré que se comporte?
	enemy->funcionControlActual = &Enemy::funControl1;

	//Vec2 posIni = Vec2(Director::getInstance()->getVisibleSize().width / 2.0f, enemy->getPosition().y);
	//enemy->update(deltaT, enemy, funcionControlMovimiento, posIni, 600.0);
	//enemy->update(Director::getInstance()->getDeltaTime(), enemy, enemy->funcionMovimientoActual, posIni, 600.0);


	// ========================================================================================================================================

	// 2 gradientes de fondo por poner algo
	// TODO: quiero hacer un cielo estrellado o algún otro tipo de fondo
	auto layer1 = LayerGradient::create(Color4B(0, 0, 0, 255), Color4B(0, 0, 127, 255));
	layer1->setContentSize(Size(visibleSize.width, visibleSize.height / 2.0f));
	layer1->setPosition(Vec2(0, visibleSize.height / 2.0f));
	addChild(layer1, -1);

	auto layer2 = LayerGradient::create(Color4B(0, 0, 128, 255), Color4B(0, 160, 180, 255));
	layer2->setContentSize(Size(visibleSize.width, visibleSize.height / 2.0f));
	layer2->setPosition(Vec2(0, 0));
	addChild(layer2, -1);


	//player = new Player;


	// schedules update every frame with default order 0. Lower orders execute first
	this->scheduleUpdate();
	// displays fps
	Director::getInstance()->setDisplayStats(true);

	return true;
}

void Level::menuVuelveCallback(Ref *pSender){
	// vuelve al menu
	Director::getInstance()->replaceScene(Menus::CreateScene());
}

bool Level::onContactBegin(PhysicsContact &contact){

	Sprite *sprA, *sprB;

	// HACK: Gestion de impactos, primera version que funciona

	Node *nodeA = contact.getShapeA()->getBody()->getNode();
	Node *nodeB = contact.getShapeB()->getBody()->getNode();

	// envía el impacto a los dos "impactantes"

	sprA = (Sprite *)contact.getShapeA()->getBody()->getNode();
	sprB = (Sprite *)contact.getShapeB()->getBody()->getNode();

	//dmg = calculaDanyoImpacto(sprA, sprB);

	//// HACK: a todos les paso el daño pero no todos hacen algo con el
	//gestionaImpacto(sprA, dmg);
	//gestionaImpacto(sprB, dmg);

	CCLOG("Impacto entre %s y %s", sprA->getName().c_str(), sprB->getName().c_str());


	// Los contactos van a ser siempre entre GameActor?
	// De momento tengo: Bullet, Player y Enemy

	// TODO: No se si sacar esto a algun sitio, de momento que funcione:

	GameActor *actor1, *actor2;
	Bullet *bulletTmp;
	float impactDmg1, impactDmg2;

	actor1 = (GameActor *)sprA->getUserData();
	actor2 = (GameActor *)sprB->getUserData();


	// calcula el daño que 1 hace a 2
	if(sprA->getTag() == (int)Game::CategoriaColision::Bala){
		bulletTmp = (Bullet *)sprA->getUserData();
		impactDmg1 = bulletTmp->bulletDmg;
	} else {
		impactDmg1 = BULLET_DEFAULT_DMG;
	}

	// y viceversa
	if(sprB->getTag() == (int)Game::CategoriaColision::Bala){
		bulletTmp = (Bullet *)sprB->getUserData();
		impactDmg2 = bulletTmp->bulletDmg;
	} else{
		impactDmg2 = BULLET_DEFAULT_DMG;
	}


	actor1->impacto(impactDmg2);
	actor2->impacto(impactDmg1);


	return true;
}

void Level::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event){
	// TODO: entiendo que los input event se llaman una vez por frame?
	//CCLOG("pressed %d", keyCode);
	inputComponent->keyPressed(keyCode);
}

void Level::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event){
	// TODO: entiendo que los input event se llaman una vez por frame?
	//CCLOG("released %d", keyCode);
	inputComponent->keyReleased(keyCode);

}

void Level::update(float deltaT){
	Game::getInstance()->ellapsedTime += deltaT;

	// ---------------------------
	// ---------------------------
	// TODO: en el update tendré que buscar manualmente todos los objetos y activar sus updates?
	// ---------------------------
	// ---------------------------

	// ---------------------------
	// PROTA
	// ---------------------------
	player->update(deltaT);

	
	// HACK: temp a ver como gestiono el mover todos los objetos
	// ¿LLevar una lista de todos los "updatables" y llamar a update en todos?
	
	// ---------------------------
	// BALAS
	// ---------------------------
	for(auto x = player->currentWeapon->bulletPool.cbegin(); x != player->currentWeapon->bulletPool.cend(); ++x){
		(*x)->update(deltaT);
	}

	for(auto x = enemy->weapon->bulletPool.cbegin(); x != enemy->weapon->bulletPool.cend(); ++x){
		(*x)->update(deltaT);
	}

	// ---------------------------
	// ENEMIGOS
	// ---------------------------

	// Sintaxis para puntero a una funcion miembro
	//void(GameActor::*funcionControlMovimiento)(Vec2, double) = &GameActor::mueveSeno;
	// same as:
	//auto funcionControlMovimiento = &GameActor::mueveSeno;

	Vec2 posIni = Vec2(Director::getInstance()->getVisibleSize().width / 2.0f, enemy->getPosition().y);
	//////enemy->update(deltaT, enemy, funcionControlMovimiento, posIni, 600.0);
	////enemy->update(deltaT, enemy, enemy->funcionMovimientoActual, posIni, 600.0);
	//enemy->update(deltaT, nullptr, nullptr, Vec2::ZERO, 0);
	enemy->update(deltaT);


}
