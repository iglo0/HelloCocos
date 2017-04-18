#include "Level.h"

Level::~Level(){
	Director::getInstance()->setDisplayStats(false);
}


Scene* Level::createScene(){
	// Gracias a las f�sicas obtengo colisiones "gratis", y como un juego tipo space invaders 
	// no creo que tenga muchos problemas de rendimiento... tirar� por ah� a ver
	// TODO: Para lograr colisiones con precisi�n hay que hacer pol�gonos a mano (vs las cajas autom�ticas). Para un poco m�s adelante (pero no mucho)
	// ----------------------------------------------------------------------------------------------------------------------------------------
	// INI del motor de f�sicas Chipmunk
	auto scene = Scene::createWithPhysics();
	// 'scene' is an autorelease object
	//auto scene = Scene::create();

	// set the world�s gravity to zero in both directions, which essentially disables gravity
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

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	// ----------------------------------------------------------------------------------------------------------------------------------------
	// preparando los eventos
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

	// Bot�n de vuelta atr�s
	auto vuelveAtras = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(Level::menuVuelveCallback, this));
	vuelveAtras->setPosition(Vec2(origin.x + visibleSize.width - vuelveAtras->getContentSize().width / 2, origin.y + vuelveAtras->getContentSize().height / 2));
	// create menu, it's an autorelease object
	auto menu = Menu::create(vuelveAtras, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	// ----------------------------------------------------------------------------------------------------------------------------------------
	// otras inicializaciones
	// ----------------------------------------------------------------------------------------------------------------------------------------

	// �apa con la que voy contando el tiempo transcurrido (sumando deltaT en cada frame).
	gameInstance->ellapsedTime = 0;

	// Indica el estado inicial de esta escena
	gameInstance->estadoActual = Game::estadosJuego::introNivel;

	// ========================================================================================================================================
	// testz
	// ----------------------------------------------------------------------------------------------------------------------------------------
	
	// Creando pools de balas para todos los tipos necesarios
	// TODO: prueba en�sima de d�nde colgar estos pools... ahora van a una clase propia "Pool".
	// TODO: Pool ""Tipo 1"" ... ehm... para los enemigos normales?
	Bullet::createBulletPool(this, Pool::currentBulletsTipo1, 30, "balaEne_", BULLET_PATH_SPRITE2, BULLET_PATH_SOUND_FIRE, BULLET_PATH_SOUND_IMPACT, -BULLET_DEFAULT_SPEED, BULLET_DEFAULT_DMG * 2.0f,
		(int)Game::CategoriaColision::BalaEnemigo, (int)Game::CategoriaColision::Jugador, BULLET_DEFAULT_BOSS_SCALE);
	// Pool de balas para el jugador
	Bullet::createBulletPool(this, Pool::currentBulletsPlayerTipo1, 16, "bala_", BULLET_PATH_SPRITE1, BULLET_PATH_SOUND_FIRE, BULLET_PATH_SOUND_IMPACT, BULLET_DEFAULT_SPEED, BULLET_DEFAULT_DMG,
		(int)Game::CategoriaColision::Bala, (int)Game::CategoriaColision::Enemigo, BULLET_DEFAULT_SCALE);

	// TODO: ahora me falta c�mo realacionar los "pools" con quienes los necesitan
	// Problema: Pool �necesita? acceso a Enemy y Bullet porque se crean "pools" de ambos tipos
	// Enemy y Bullet no pueden acceder a Pool �tengo que buscar un intermediario?

	// La raz�n de tanto pool es tener los updates centralizados, me parece que estoy haciendo un embrollo otra vez...

	// instancio un jugador con los valores por defecto
	player = new Player(this, PLAYER_INITIAL_SPEED);
	// Iba a colgar el inputComponent del jugador, quiz� est� mejor colgando del nivel y con una referencia al jugador
	inputComponent = new InputComponent;
	inputComponent->player = player;

	// Pruebo el nuevo Enemy:GameActor 

	// No necesito que sea una variable miembro, vivir� en un array en alguna parte.
	Enemy *enemyBoss;

	enemyBoss = new Enemy(this, ENEMY_BOSS_PATH_SPRITE, ENEMY_PATH_SOUND_DIE, ENEMY_BOSS_INITIAL_SIZE, ENEMY_BOSS_INITIAL_ROTATION, ENEMY_BOSS_GENERIC_HP);
	// situo al enemigo arriba en el medio, con medio cuerpo de margen superior
	Vec2 enePos = Vec2(visibleSize.width / 2.0f, visibleSize.height - enemyBoss->getSprite()->getContentSize().height);
	enemyBoss->activa(enePos);

	// C�mo querr� que se mueva?
	//auto funcionControlMovimiento = &GameActor::mueveSeno;
	enemyBoss->funcionMovimientoActual = &GameActor::mueveSeno;
	enemyBoss->funcionMovimientoAmplitude = 600.0;
	enemyBoss->funcionMovimientoPosIni = Vec2(Director::getInstance()->getVisibleSize().width / 2.0f, enemyBoss->getPosition().y);
	enemyBoss->funcionMovimientoSpeed = 1 / 2.5;
	
	// y que ataque?
	enemyBoss->funcionControlActual = &Enemy::funControl1;
	enemyBoss->funcionControlTiempoDisparo = 1.0f;
	
	enemyBoss->poolMisBalas = &Pool::currentBulletsTipo1;

	// hale, definido
	
	// ahora a�adirlo al pool de GameUpdates para que haga algo de lo que le he dicho
	Pool::currentEnemies.push_back(enemyBoss);

	SpaceInvaders spaceInvaders;
	spaceInvaders.creaInvaders(this, 3, 3, Pool::currentBulletsTipo1, 50.0f, 10.0f, 1, 1.0f);

	// TODO: Tengo que a�adir a los bichos creados por SpaceInvaders al "pool" de updates

	//Vec2 posIni = Vec2(Director::getInstance()->getVisibleSize().width / 2.0f, enemy->getPosition().y);
	//enemy->update(deltaT, enemy, funcionControlMovimiento, posIni, 600.0);
	//enemy->update(Director::getInstance()->getDeltaTime(), enemy, enemy->funcionMovimientoActual, posIni, 600.0);


	// ========================================================================================================================================

	// 2 gradientes de fondo por poner algo
	// TODO: quiero hacer un cielo estrellado o alg�n otro tipo de fondo
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

	// env�a el impacto a los dos "impactantes"

	sprA = (Sprite *)contact.getShapeA()->getBody()->getNode();
	sprB = (Sprite *)contact.getShapeB()->getBody()->getNode();

	//dmg = calculaDanyoImpacto(sprA, sprB);

	//// HACK: a todos les paso el da�o pero no todos hacen algo con el
	//gestionaImpacto(sprA, dmg);
	//gestionaImpacto(sprB, dmg);

	CCLOG("Impacto entre %s(%d) y %s(%d)", sprA->getName().c_str(), sprA->getTag(), sprB->getName().c_str(), sprB->getTag());


	// Los contactos van a ser siempre entre GameActor?
	// De momento tengo: Bullet, Player y Enemy

	// TODO: No se si sacar esto a algun sitio, de momento que funcione:

	GameActor *actor1, *actor2;
	Bullet *bulletTmp;
	float impactDmg1, impactDmg2;

	actor1 = (GameActor *)sprA->getUserData();
	actor2 = (GameActor *)sprB->getUserData();

	// calcula el da�o que 1 hace a 2
	if(sprA->getTag() == (int)Game::CategoriaColision::Bala || sprA->getTag() == (int)Game::CategoriaColision::BalaEnemigo){
		bulletTmp = (Bullet *)sprA->getUserData();
		impactDmg1 = bulletTmp->bulletDmg;
	} else {
		impactDmg1 = BULLET_DEFAULT_DMG;
	}

	// y viceversa
	if(sprB->getTag() == (int)Game::CategoriaColision::Bala || sprB->getTag() == (int)Game::CategoriaColision::BalaEnemigo){
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
	// PROTA
	// ---------------------------
	player->update(deltaT);

	// ---------------------------
	// UPDATE ALL THE THINGS!!
	// ---------------------------

	Pool::updateAll(deltaT);

}
