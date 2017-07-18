#include "Level.h"

#include "Bullet.h"
#include "Player.h"
#include "InputComponent.h"
#include "Enemy.h"
#include "Pool.h"
#include "SpaceInvaders.h"
#include "Menus.h"
#include "Movimiento.h"
#include "Game.h"
#include "GameState.h"


// variable estatica
Level *Level::instance;

Level::~Level(){
	Director::getInstance()->setDisplayStats(false);

	// así a botepronto borrar...
	// pools porque son estáticos
	Pool::deletePools();
	// enemigos?
	// player?

	gameInstance->vidas = gameInstance->vidas_iniciales;
	gameInstance->puntos = 0;
}

Scene* Level::createScene(){
	// Gracias a las físicas obtengo colisiones "gratis", y como un juego tipo space invaders 
	// no creo que tenga muchos problemas de rendimiento... tiraré por ahí a ver
	// TODO: Para lograr colisiones con precisión hay que hacer polígonos a mano (vs las cajas automáticas). Para un poco más adelante (pero no mucho)
	// ----------------------------------------------------------------------------------------------------------------------------------------

	// 'scene' is an autorelease object
	// sin física:
	//auto scene = Scene::create();
	// con física (chipmunk):
	auto scene = Scene::createWithPhysics();

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

	Size visibleSize;
	Vec2 origin;
	
	instance = this;

	// ----------------------------------------------------------------------------------------------------------------------------------------
	// inits
	// ----------------------------------------------------------------------------------------------------------------------------------------
	if(!Layer::init()){
		return false;
	}

	// inicializa Game si no lo estaba
	gameInstance = Game::getInstance();

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	auto size = Director::getInstance()->getWinSize();

	iniciadoFinNivel = false;
	iniciadoIntroNivel = false;
	iniciadoMuerte = false;

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

	createGUI();

	// ----------------------------------------------------------------------------------------------------------------------------------------
	// otras inicializaciones
	// ----------------------------------------------------------------------------------------------------------------------------------------

	// ñapa con la que voy contando el tiempo transcurrido (sumando deltaT en cada frame).
	gameInstance->ellapsedTime = 0;

	// Indica el estado inicial de esta escena
	//gameInstance->estadoActual = Game::estadosJuego::introNivel;

	initLevel();


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

	// schedules update every frame with default order 0. Lower orders execute first
	this->scheduleUpdate();
	// displays fps
	Director::getInstance()->setDisplayStats(true);


	// inicia la máquina de estados
	// PRUEBA: FSM
	gameState = new IntroNivelState(player);
	//gameState->enterState();


	return true;
}

void Level::menuVuelveCallback(Ref *pSender){
	// vuelve al menu
	//Director::getInstance()->replaceScene(Menus::CreateScene());
	
	// funcion estatica visible desde fuera. Hala.
	vuelveAlMenu();
}

bool Level::onContactBegin(PhysicsContact &contact){
	// TODO: la gestión de colisiones necesita un reprogramado curioso...

	// TODO: problema! colisiones múltiples, imagino que al definir con polígonos los objetos pueden llegar a colisionar varios polígonos de un objeto con otro.
	// si por ejemplo le pasa al jugador, pierde varias vidas. Tengo que hacerlo de otro modo. Quizás esperar a hacerlo más tarde y no en el mismo momento en que salta una colisión.

	// nanananananannanaaaa prota definido con un triángulo. Colisiones simples de nuevo! O:-)

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

	CCLOG("Impacto entre %s(%d) y %s(%d)", sprA->getName().c_str(), sprA->getTag(), sprB->getName().c_str(), sprB->getTag());


	// Los contactos van a ser siempre entre GameActor?
	// De momento tengo: Bullet, Player y Enemy

	// TODO: No se si sacar esto a algun sitio, de momento que funcione:

	GameActor *actor1, *actor2;
	Bullet *bulletTmp;
	float impactDmg1, impactDmg2;

	actor1 = (GameActor *)sprA->getUserData();
	actor2 = (GameActor *)sprB->getUserData();

	if(actor1->type_ == GameActor::gameActorTypes::destructible || actor2->type_ == GameActor::gameActorTypes::destructible)
		actor1 = actor1;	// punto de parada if casita

	// calcula el daño que 1 hace a 2
	if(sprA->getTag() == (int)Game::CategoriaColision::BalaJugador || sprA->getTag() == (int)Game::CategoriaColision::BalaEnemigo){
		bulletTmp = (Bullet *)sprA->getUserData();
		impactDmg1 = bulletTmp->bulletDmg;
	} else {
		impactDmg1 = gameInstance->bullet_default_dmg;
	}

	// y viceversa
	if(sprB->getTag() == (int)Game::CategoriaColision::BalaJugador || sprB->getTag() == (int)Game::CategoriaColision::BalaEnemigo){
		bulletTmp = (Bullet *)sprB->getUserData();
		impactDmg2 = bulletTmp->bulletDmg;
	} else{
		impactDmg2 = gameInstance->bullet_default_dmg;
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
	GameState *tmpState;
	// siempre cuento el tiempo
	gameInstance->ellapsedTime += deltaT;

	tmpState = gameState->update(deltaT);
	if(tmpState){
		// el estado ha cambiado 
		delete gameState;
		gameState = tmpState;
	}

}


void Level::createGUI(){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Botón de vuelta atrás
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
	auto vuelveAtras = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(Level::menuVuelveCallback, this));
	vuelveAtras->setPosition(Vec2(origin.x + visibleSize.width - vuelveAtras->getContentSize().width / 2, origin.y + vuelveAtras->getContentSize().height / 2));
	// create menu, it's an autorelease object
	auto menu = Menu::create(vuelveAtras, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// marcadores
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------

	// TODO: Ojo que es un poco "raro". Para actualizar los marcadores desde cualquier instancia, los voy a colocar mayormente en Game (los que se actualizan al menos)

	// create a TTFConfig files for labels to share
	TTFConfig labelConfig;
	labelConfig.fontFilePath = "fonts/Marker Felt.ttf";
	labelConfig.fontSize = 24;
	labelConfig.glyphs = GlyphCollection::DYNAMIC;
	labelConfig.outlineSize = 0;
	labelConfig.customGlyphs = nullptr;
	labelConfig.distanceFieldEnabled = false;

	// en el centro
	gameInstance->lblMensajes = Label::createWithTTF(labelConfig, "blah blah blah blah");
	gameInstance->lblMensajes->setPosition(Vec2(	visibleSize.width / 2.0f - gameInstance->lblMensajes->getContentSize().width / 2.0f, 
													visibleSize.height / 2.0f + gameInstance->lblMensajes->getContentSize().height / 2.0f));
	gameInstance->lblMensajes->enableShadow();
	gameInstance->lblMensajes->setVisible(false);

	// arriba a la izquierda
	// label fijo
	auto lblPuntosFijo = Label::createWithTTF(labelConfig, "Puntos:");
	lblPuntosFijo = Label::createWithTTF(labelConfig, "Puntos:");
	lblPuntosFijo->setPosition(Vec2(50.0f, visibleSize.height - lblPuntosFijo->getContentSize().height / 2.0f));
	lblPuntosFijo->enableShadow();
	lblPuntosFijo->setTextColor(Color4B::RED);	// o setColor(Color3B)??
	// label variable
	//lblPuntos = Label::createWithTTF(labelConfig, "000000");
	//lblPuntos->setPosition(Vec2(140.0f, visibleSize.height - lblPuntos->getContentSize().height / 2.0f));
	//lblPuntos->enableShadow();
	gameInstance->lblPuntos = Label::createWithTTF(labelConfig, "ffffuuuu");
	gameInstance->lblPuntos->setPosition(Vec2(140.0f, visibleSize.height - gameInstance->lblPuntos->getContentSize().height / 2.0f));
	gameInstance->lblPuntos->enableShadow();

	// arriba en el centro
	// label fijo
	auto lblHiScoreFijo = Label::createWithTTF(labelConfig, "HiScore:");
	lblHiScoreFijo->setPosition(Vec2(visibleSize.width / 2.0f - lblHiScoreFijo->getContentSize().width / 2.0f, visibleSize.height - lblHiScoreFijo->getContentSize().height / 2.0f));
	lblHiScoreFijo->enableShadow();
	lblHiScoreFijo->setTextColor(Color4B::RED);
	// label variable
	gameInstance->lblHiScore = Label::createWithTTF(labelConfig, "xxxxxx");
	gameInstance->lblHiScore->setPosition(Vec2(70.0f + visibleSize.width / 2.0f - gameInstance->lblHiScore->getContentSize().width / 2.0f, visibleSize.height - gameInstance->lblHiScore->getContentSize().height / 2.0f));
	gameInstance->lblHiScore->enableShadow();


	// arriba a la derecha
	// label fijo
	auto lblVidasFijo = Label::createWithTTF(labelConfig, "Vidas:");
	lblVidasFijo->setPosition(Vec2(visibleSize.width - lblVidasFijo->getContentSize().width - 50.0f, visibleSize.height - lblVidasFijo->getContentSize().height / 2.0f));
	lblVidasFijo->enableShadow();
	lblVidasFijo->setTextColor(Color4B::RED);
	// label variable
	gameInstance->lblVidas = Label::createWithTTF(labelConfig, "69");
	gameInstance->lblVidas->setPosition(Vec2(20.0f + visibleSize.width - gameInstance->lblVidas->getContentSize().width - 50.0f, visibleSize.height - gameInstance->lblVidas->getContentSize().height / 2.0f));
	gameInstance->lblVidas->enableShadow();

	// estos no necesitan updates
	this->addChild(lblPuntosFijo, 1);
	this->addChild(lblHiScoreFijo, 1);
	this->addChild(lblVidasFijo, 1);

	// marcadores que cambian durante la partida
	this->addChild(gameInstance->lblMensajes, 1);
	this->addChild(gameInstance->lblPuntos, 1);
	this->addChild(gameInstance->lblHiScore, 1);
	this->addChild(gameInstance->lblVidas, 1);

	// inicializa con valores por defecto
	gameInstance->inicializaGUI();
}


void Level::initLevel(){
	auto visibleSize = Director::getInstance()->getVisibleSize();

	// ----------------------------------------------------------------------------------------------------------------------------------------
	// creacion del jugador
	// ----------------------------------------------------------------------------------------------------------------------------------------

	// instancio un jugador con los valores por defecto
	//player = new Player(this, PLAYER_INITIAL_SPEED);
	player = new Player(this, gameInstance->player_initial_speed);

	// algo que disparar
	// TODO: para cambiar de arma solo tendría que cambiar la referencia.... mola
	player->poolMisBalas = &Pool::currentBulletsPlayerTipo1;

	// Iba a colgar el inputComponent del jugador, quizá esté mejor colgando del nivel y con una referencia al jugador
	inputComponent = new InputComponent;
	inputComponent->player = player;

	// ----------------------------------------------------------------------------------------------------------------------------------------
	// prepara los pools de balas a usar ¿en toda la partida?
	// ----------------------------------------------------------------------------------------------------------------------------------------

	// Creando pools de balas para todos los tipos necesarios
	// TODO: prueba enésima de dónde colgar estos pools... ahora van a una clase propia "Pool".
	// Pool de balas para el jugador
	Bullet::createBulletPool(this, Pool::currentBulletsPlayerTipo1, 2, Bullet::tipoPlayer);

	// TODO: Pool para los enemigos normales
	Bullet::createBulletPool(this, Pool::currentBulletsTipoNormal, 60, Bullet::tipoEnemyNormal);

	// TODO: Pool para los enemigos con disparo dirigido
	Bullet::createBulletPool(this, Pool::currentBulletsTipo2, 60, Bullet::tipoEnemyDirigido);

	// TODO: Pool para el boss
	Bullet::createBulletPool(this, Pool::currentBulletsTipoBossHoming, 5, Bullet::tipoBossHoming);

	// ----------------------------------------------------------------------------------------------------------------------------------------
	// inicializo los enemigos iniciales
	// TODO: que esto dependa de una estructura definible idealmente en un .ini
	// ----------------------------------------------------------------------------------------------------------------------------------------

	// No necesito que sea una variable miembro, vivirá en un array en alguna parte.
	Enemy *enemyBoss;

	enemyBoss = new Enemy(Enemy::tiposEnemigo::tipoBoss);
	enemyBoss->initEnemy(this);

	// situo al enemigo arriba en el medio, con medio cuerpo de margen superior
	Vec2 enePos = Vec2(visibleSize.width / 2.0f, visibleSize.height - enemyBoss->getSprite()->getContentSize().height / 2.0f);
	enemyBoss->activa(enePos);

	// Cómo querré que se mueva?
	//enemyBoss->funcionMovimientoActual = &GameActor::mueveSeno;
	//enemyBoss->funcionMovimientoAmplitude = 600.0;
	//enemyBoss->funcionMovimientoPosIni = Vec2(Director::getInstance()->getVisibleSize().width / 2.0f, enemyBoss->getPosition().y);
	//enemyBoss->funcionMovimientoSpeed = 1 / 2.5;

	// y que ataque?
	enemyBoss->funcionControlActual_ = &Enemy::funControlFireAtInterval;
	enemyBoss->funcionControlTiempoDisparo_ = 5.0f;

	MueveSeno *m = new MueveSeno();
	m->init(600.0f, enePos, 0.333f);
	enemyBoss->movimiento_ = m;

	//enemyBoss->poolMisBalas_ = &Pool::currentBulletsTipoBossHoming;

	// hale, definido

	// ahora añadirlo al pool de GameUpdates para que haga algo de lo que le he dicho
	Pool::currentEnemies.push_back(enemyBoss);

	// ---------------------------------------------------------------------------------------------
	// crea los "Space Invaders
	// TODO: temporal, tiene que haber una progresión
	SpaceInvaders spaceInvaders = SpaceInvaders(11, 5, 0.42f, 0.52f, 50.0f, 150.0f);

	// creo una lista de enemigos que tiene que coincidir aprox con el nº de filas. Un tipo de enemigo por fila. Si se le acaban los tipos, repite el ultimo hasta el fin de las filas
	std::vector<Enemy::tiposEnemigo> tipos;
	tipos.push_back(Enemy::tipo2);
	tipos.push_back(Enemy::tipo2);
	tipos.push_back(Enemy::tipo1);

	spaceInvaders.creaInvaders(this, tipos, Pool::currentBulletsTipoNormal, 50.0f, 15.0f, 30.0f, 3600);

	creaCasitas(4, 100.0f);
}

void Level::creaCasitas(int numba, float margen){
	// TODO: no ponerlo a mano
	int casaWidth = 128;
	auto winSize = Director::getInstance()->getWinSize();
	float separa = (winSize.width - margen * 2) / numba;

	// Suspenso en trigonometría xD
	/*
	Si quiero dividir 100 / 4, me sale que coloco las casas cada 25 espacios, pero si las quiero centradas
	las tengo que poner en el medio de cada uno de estos espacios

	Y acordarme que 0,0 en un sprite es el medio, así que tengo que colocarlas en -ancho/2 del sprite

	|....+....|....+....|....+....|....+....|
	0		 25			50		  75		100

	*/

	for(int i = 0; i < numba; i++){
		// intenta calcular como dividir el espacio para que quepan num casitas, dejando margenes a los lados y situando cada una en la mitad del punto (x - anchoCasa/2)
		creaCasita(Vec2(margen + separa/2.0f + i*separa - casaWidth/2.0f, 120.0f));
	}

}

void Level::creaCasita(Vec2 esquinaInfIzq){
	// HACK: solo cargo este sprite para calcular su tamaño
	Sprite *spr = Sprite::createWithSpriteFrameName(gameInstance->sprite_casa_bloque.c_str());
	auto blockSize = spr->getContentSize();
	Vec2 posBloque;
	GameActor *bloque;

	for(int j = 0; j < 4; j++){
		for(int i = 0; i < 4; i++){
			posBloque = esquinaInfIzq;
			// ojo que 0,0 es el centro del sprite, si cuento con que empiece en la esquina inferior izquierda tengo que desplazarlo la mitad de su ancho. El alto también, pero ahora me la pela :P
			posBloque.x += i * blockSize.width + blockSize.width/2.0f;
			posBloque.y += j * blockSize.height;

			// hace el tejado de forma sucia
			if(i == 0 && j == 3){
				// esquina superior izquierda?
				bloque = creaDestructible(posBloque,1);
			} else if(i == 3 && j == 3){
				// esquina superior derecha?
				bloque = creaDestructible(posBloque,2);
			} else{
				// pos bloque estándar
				bloque = creaDestructible(posBloque);
			}
		}
	}
}

GameActor *Level::creaDestructible(Vec2 pos, int type){
	GameActor *tmp = new GameActor();
	tmp->type_ = GameActor::gameActorTypes::destructible;
	const char *spritePath;

	switch(type){
	case 0:
		spritePath = gameInstance->sprite_casa_bloque.c_str();
		break;
	case 1:
		spritePath = gameInstance->sprite_casa_esquina_izq.c_str();
		break;
	case 2:
		spritePath = gameInstance->sprite_casa_esquina_dch.c_str();
		break;
	default:
		break;
	}

	tmp->setSprite(this, spritePath, "casa", (int)Game::CategoriaColision::Destructible, 
		(int)Game::CategoriaColision::BalaJugador | (int)Game::CategoriaColision::BalaEnemigo | (int)Game::CategoriaColision::Enemigo);
	tmp->activa(pos);
	
	return tmp;
}

void Level::setGameState(GameState *nuevo){
	if(instance){
		// HACK: Puedo hacer esto a lo bruto?
		// me tendría que asegurar que gameState existe antes del delete. A priori... sí...
		delete instance->gameState;
		instance->gameState = nuevo;
	} else{
		CCLOG("Instance sin inicializar en Level");
	}
}

// fun estatica
void Level::vuelveAlMenu(){
	Director::getInstance()->replaceScene(Menus::CreateScene());
}
