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
#include "HiScoresScene.h"
#include "HiScores.h"

#include "XmlHelper.h"


// variable estatica
Level *Level::instance;

Level::~Level(){
	Director::getInstance()->setDisplayStats(false);

	// as� a botepronto borrar...
	// pools porque son est�ticos
	Pool::deletePools();
	// enemigos?
	// player?

	gameInstance->vidas = gameInstance->vidas_iniciales;
	gameInstance->puntos = 0;
}

Scene* Level::createScene(){
	// Gracias a las f�sicas obtengo colisiones "gratis", y como un juego tipo space invaders 
	// no creo que tenga muchos problemas de rendimiento... tirar� por ah� a ver
	// TODO: Para lograr colisiones con precisi�n hay que hacer pol�gonos a mano (vs las cajas autom�ticas). Para un poco m�s adelante (pero no mucho)
	// ----------------------------------------------------------------------------------------------------------------------------------------
	
	// 'scene' is an autorelease object
	// sin f�sica:
	//auto scene = Scene::create();
	// con f�sica (chipmunk):
	auto scene = Scene::createWithPhysics();

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

	// �apa con la que voy contando el tiempo transcurrido (sumando deltaT en cada frame).
	gameInstance->ellapsedTime = 0;

	// Indica el estado inicial de esta escena
	//gameInstance->estadoActual = Game::estadosJuego::introNivel;

	initLevel();


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

	// schedules update every frame with default order 0. Lower orders execute first
	this->scheduleUpdate();
	// displays fps
	Director::getInstance()->setDisplayStats(true);


	// inicia la m�quina de estados
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
	// TODO: la gesti�n de colisiones necesita un reprogramado curioso...

	// TODO: problema! colisiones m�ltiples, imagino que al definir con pol�gonos los objetos pueden llegar a colisionar varios pol�gonos de un objeto con otro.
	// si por ejemplo le pasa al jugador, pierde varias vidas. Tengo que hacerlo de otro modo. Quiz�s esperar a hacerlo m�s tarde y no en el mismo momento en que salta una colisi�n.

	// nanananananannanaaaa prota definido con un tri�ngulo. Colisiones simples de nuevo! O:-)
	
	
	// muestra el impacto gr�ficamente
	Vec2 impactoPos = contact.getContactData()->points[0];
	Pool::activa(Pool::currentImpacts, impactoPos);


	Sprite *sprA, *sprB;

	// HACK: Gestion de impactos, primera version que funciona

	Node *nodeA = contact.getShapeA()->getBody()->getNode();
	Node *nodeB = contact.getShapeB()->getBody()->getNode();

	// env�a el impacto a los dos "impactantes"

	sprA = (Sprite *)contact.getShapeA()->getBody()->getNode();
	sprB = (Sprite *)contact.getShapeB()->getBody()->getNode();

	CCLOG("Impacto entre %s(%d) y %s(%d)", sprA->getName().c_str(), sprA->getTag(), sprB->getName().c_str(), sprB->getTag());


	// Los contactos van a ser siempre entre GameActor?
	// De momento tengo: Bullet, Player y Enemy

	// TODO: No se si sacar esto a algun sitio, de momento que funcione:
	GameActor *actor1, *actor2;
	Bullet *bulletTmp;
	float impactDmg1, impactDmg2;

	actor1 = (GameActor *)sprA->getUserData();
	actor2 = (GameActor *)sprB->getUserData();

	//if(actor1->type_ == GameActor::gameActorTypes::destructible || actor2->type_ == GameActor::gameActorTypes::destructible)
	//	actor1 = actor1;	// punto de parada if casita


	// --------------------------------------------------------------------------------------
	// Prepara la regeneraci�n de casitas apunt�ndolas en un vector para luego recuperarlas
	// --------------------------------------------------------------------------------------
	if(actor1->type_ == GameActor::gameActorTypes::destructible){
		// cosas con las casas
		//casasRotas.push_back(actor1);
		// �inserta al rev�s?
		casasRotas.insert(casasRotas.begin(), actor1);
	}

	if(actor2->type_ == GameActor::gameActorTypes::destructible){
		// cosas con las casas
		//casasRotas.push_back(actor2);
		// �inserta al rev�s?
		casasRotas.insert(casasRotas.begin(), actor2);
	}
	// --------------------------------------------------------------------------------------

	// calcula el da�o que 1 hace a 2
	if(sprA->getTag() == (int)Game::CategoriaColision::BalaJugador || sprA->getTag() == (int)Game::CategoriaColision::BalaEnemigo){
		bulletTmp = (Bullet *)sprA->getUserData();
		impactDmg1 = bulletTmp->bulletDmg;
	} else {
		CCLOG("Atencion, da�o generico!");
		impactDmg1 = gameInstance->bullet_default_dmg;
	}

	// y viceversa
	if(sprB->getTag() == (int)Game::CategoriaColision::BalaJugador || sprB->getTag() == (int)Game::CategoriaColision::BalaEnemigo){
		bulletTmp = (Bullet *)sprB->getUserData();
		impactDmg2 = bulletTmp->bulletDmg;
	} else{
		CCLOG("Atencion, da�o generico!");
		impactDmg2 = gameInstance->bullet_default_dmg;
	}

	actor1->impacto(impactDmg2);
	actor2->impacto(impactDmg1);

	//CCLOG("Impacto en %s y %s", typeid(actor1).name(), typeid(actor2).name());	// "impacto entre gameactor * y gameactor * :(

	return true;
}

void Level::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event){
	// entiendo que los input event se llaman una vez por frame?
	// Yep, no se llaman cada vez que la cpu puede, sino con cada update

	// testing
	if(keyCode == EventKeyboard::KeyCode::KEY_R){
		regenerarCasitas(1);
	}

	if(apuntandoRecords_){
		pressedKey(keyCode);
		gameInstance->lblHiScoreName->setString(playerName_);
	} else{
		//CCLOG("pressed %d", keyCode);
		inputComponent->keyPressed(keyCode);
	}
}

void Level::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event){
	// TODO: entiendo que los input event se llaman una vez por frame?
	//CCLOG("released %d", keyCode);
	inputComponent->keyReleased(keyCode);

	// HACK: �APA
	if(keyCode == EventKeyboard::KeyCode::KEY_F1){
		CCLOG("F1 Iniciando/parando captura");
		capturing_ = !capturing_;
	}
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

	// Captura de frames para hacer luego un gif animado
	// lo guarda en %appdata%\..\local\HelloCocos 
	if(capturing_){
		utils::captureScreen(CC_CALLBACK_2(Level::afterCaptured, this), std::string("shot") + std::to_string(Game::getInstance()->ellapsedTime) + std::string(".png"));
	}

	// TODO: quitar
	// actualiza mensajes debug en cada frame
	// lbl->setString() espera un string
	// para formatear el string utilizo un stringstream, y luego lo convierto a string para el lbl
	std::stringstream ss;
	//The most common IO manipulators that control padding are:
	//std::setw(width) sets the width of the field.
	//std::setfill(fillchar) sets the fill character.
	//std::setiosflags(align) sets the alignment, where align is ios::left or ios::right

	ss << SpaceInvaders::numInvadersVivos_ << "/" << SpaceInvaders::numInvadersInicial_;

	//ss << std::setw(6) << std::setfill('0') << puntos;// << std::endl;
	gameInstance->lblDebug->setString(ss.str());

}


void Level::createGUI(){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Bot�n de vuelta atr�s
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
	labelConfig.outlineSize = 2.0;
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

	// DEBUG
	gameInstance->lblDebug = Label::createWithTTF(labelConfig, "pillo sitioooooooooo");

	gameInstance->lblDebug->setPosition(Vec2(gameInstance->lblDebug->getContentSize().width / 2.0f, visibleSize.height - gameInstance->lblDebug->getContentSize().height / 2.0f - 20.0f));
	gameInstance->lblDebug->enableShadow();
	gameInstance->lblDebug->setTextColor(Color4B::YELLOW);

	this->addChild(gameInstance->lblDebug, 1);

	// label para apuntar tu record
	gameInstance->lblHiScoreName = Label::createWithTTF(labelConfig, "..........................");
	//gameInstance->lblHiScoreName->setPosition(Vec2(gameInstance->lblHiScoreName->getContentSize().width / 2.0f, visibleSize.height - gameInstance->lblHiScoreName->getContentSize().height / 2.0f - 20.0f));
	gameInstance->lblHiScoreName->setPosition(Vec2(visibleSize.width/2.0f - gameInstance->lblHiScoreName->getContentSize().width/2.0f, 300.0f));
	gameInstance->lblHiScoreName->enableShadow();
	gameInstance->lblHiScoreName->setTextColor(Color4B::YELLOW);
	//gameInstance->lblHiScoreName->setColor(Color3B::ORANGE);
	gameInstance->lblHiScoreName->setScale(2.0f);
	gameInstance->lblHiScoreName->setVisible(false);

	this->addChild(gameInstance->lblHiScoreName, 1);


	// inicializa con valores por defecto
	gameInstance->inicializaGUI();

}

void Level::initLevel(){
	auto visibleSize = Director::getInstance()->getVisibleSize();

	// ----------------------------------------------------------------------------------------------------------------------------------------
	// creacion del jugador
	// ----------------------------------------------------------------------------------------------------------------------------------------

	// instancio un jugador con los valores por defecto
	player = new Player(this, gameInstance->player_initial_speed);

	// algo que disparar
	// TODO: para cambiar de arma solo tendr�a que cambiar la referencia.... mola
	player->poolMisBalas = &Pool::currentBulletsPlayerTipo1;

	// Iba a colgar el inputComponent del jugador, quiz� est� mejor colgando del nivel y con una referencia al jugador
	inputComponent = new InputComponent;
	inputComponent->player = player;

	// ----------------------------------------------------------------------------------------------------------------------------------------
	// prepara los pools de balas a usar �en toda la partida?
	// ----------------------------------------------------------------------------------------------------------------------------------------

	// Creando pools de balas para todos los tipos necesarios
	// TODO: prueba en�sima de d�nde colgar estos pools... ahora van a una clase propia "Pool".

	// Pool de balas para el jugador
	Bullet::createBulletPool(this, Pool::currentBulletsPlayerTipo1, 2, Bullet::tipoPlayer, "bullet_player");

	// Pool para los enemigos normales
	Bullet::createBulletPool(this, Pool::currentBulletsTipoNormal, 100, Bullet::tipoEnemyNormal, "bullet_enemy_default");

	// Pool para los enemigos con disparo dirigido
	Bullet::createBulletPool(this, Pool::currentBulletsTipo2, 50, Bullet::tipoEnemyDirigido, "bullet_enemy_dirigida");

	// Pool para el ovni
	Bullet::createBulletPool(this, Pool::currentBulletsTipoBossHoming, 5, Bullet::tipoBossHoming, "bullet_enemy_homing");

	// probando a detectar el fin de las animaciones con ttl o con loop=0 (ver .xml)
	GameActor::createAnimationPool(this, Pool::currentExplosions, 5, "explosion");
	GameActor::createAnimationPool(this, Pool::currentImpacts, 5, "impacto");
	
	// ----------------------------------------------------------------------------------------------------------------------------------------
	// inicializo el ovni
	// ----------------------------------------------------------------------------------------------------------------------------------------
	#pragma region ovni 

	// No necesito que sea una variable miembro, vivir� en un array en alguna parte.
	Enemy *enemyBoss;

	// Acabo de aprender typeid :)
	CCLOG(typeid(enemyBoss).name());


	enemyBoss = new Enemy(Enemy::tiposEnemigo::tipoOvni);
	enemyBoss->initEnemy(this);

	// situo al enemigo arriba en el medio, con medio cuerpo de margen superior
	Vec2 enePos = Vec2(visibleSize.width / 2.0f, visibleSize.height - enemyBoss->getSprite()->getContentSize().height / 2.0f);
	enemyBoss->activa(enePos);

	// C�mo querr� que ataque?
	enemyBoss->funcionControlActual_ = &Enemy::funControlFireAtInterval;
	enemyBoss->funcionControlTiempoDisparo_ = 3.5f;

	// Y que se mueva?
	MueveSeno *m = new MueveSeno();
	m->init(600.0f, enePos, 0.333f);
	enemyBoss->movimiento_ = m;

	// hale, definido

	// ahora a�adirlo al pool de GameUpdates para que haga algo de lo que le he dicho
	Pool::currentEnemies.push_back(enemyBoss);
	#pragma endregion

	// ----------------------------------------------------------------------------------------------------------------------------------------
	// las casitas destructibles
	// ----------------------------------------------------------------------------------------------------------------------------------------
	creaCasitas(5, 100.0f);

	// ----------------------------------------------------------------------------------------------------------------------------------------
	// inicializo los enemigos
	// ----------------------------------------------------------------------------------------------------------------------------------------
	XmlHelper *xh = new XmlHelper();
	levels_ = xh->loadInvaderLevels(this, "oleada");

	oleadaNum_ = 0;
	actualLevel_ = nullptr;

	// Empiezo por una oleada (la primera). La destrucci�n del �ltimo enemigo de alguna manera comunicar� que hay que iniciar una nueva oleada
	// (hago p�blico un m�todo est�tico de esta clase)
	avanzaOleada();

}

void Level::avanzaOleada(){
	//SpaceInvaders *tmp;
	
	if(actualLevel_){
		// tengo un nivel ya cargado (el que acabo de terminar)
		// �debo cambiar de nivel?:
		// niveles<0,oleada>	niveles<2,oleada>	niveles<4,oleada>
		// cargar�a una configuraci�n distinta cuando oleadaNum es > que el primer �ndice
		//int dbg = levelsIterator_->first;
		if(++oleadaNum_ > levelsIterator_->first){
			// si a�n no soy el �ltimo, avanzo
			if(levelsIterator_ != levels_.end()){
				++levelsIterator_;
			}
		}

		// despu�s de avanzar, soy el �ltimo?
		if(levelsIterator_ != levels_.end()){
			actualLevel_ = levelsIterator_->second;
		}
		// else actualLevel no cambia

	} else{
		// primer nivel a cargar
		// oleadaNum_ = 1;
		//std::unordered_map<int, SpaceInvaders *>::iterator it = levels_.begin();
		levelsIterator_ = levels_.begin();
		// TODO: �por qu� levels_.cbegin() da error?

		actualLevel_ = levelsIterator_->second;
	}

	actualLevel_->creaInvaders(this);
	//actualLevel_->creaInvaders(this, fuck, )
	
	//actualLevel_->creaInvaders(this,)
	/* creo una lista de enemigos que tiene que coincidir aprox con el n� de filas. Un tipo de enemigo por fila. Si se le acaban los tipos, repite el ultimo hasta el fin de las filas
	std::vector<Enemy::tiposEnemigo> tipos;
	tipos.push_back(Enemy::tipo2);
	tipos.push_back(Enemy::tipo2);
	tipos.push_back(Enemy::tipo1);

	spaceInvaders.creaInvaders(this, tipos, 50.0f, 15.0f, 30.0f, 3600)
	*/
}

// M�todo est�tico. B�sicamente llama a "avanzaOleada" desde fuera. Y se llama desde el GameState correspondiente cuando detecta que esta oleada est� kaputt
void Level::siguienteNivel(){
	instance->avanzaOleada();

	instance->regenerarCasitas(16);
}

void Level::regenerarCasitas(int num){
	GameActor *tmp;
	int i = 0;

	// recupera un n� de casitas
	while(i++ < num && !casasRotas.empty()){
		tmp = casasRotas.back();
		tmp->regeneraCadaver();
		casasRotas.pop_back();
	}

	//for(auto a = instance->casasRotas.cbegin(); a != instance->casasRotas.cend(); ++a){
	//	tmp = *a;
	//	
	//	tmp->regeneraCadaver();
	//	if(++i > num){
	//		break;
	//	}
	//}

	//instance->casasRotas.clear();


}

void Level::creaCasitas(int numba, float margen){
	// TODO: no ponerlo a mano
	int casaWidth = 128;
	auto winSize = Director::getInstance()->getWinSize();
	float separa = (winSize.width - margen * 2) / numba;

	// Suspenso en trigonometr�a xD
	/*
	Si quiero dividir 100 / 4, me sale que coloco las casas cada 25 espacios, pero si las quiero centradas
	las tengo que poner en el medio de cada uno de estos espacios

	Y acordarme que 0,0 en un sprite es el medio, as� que tengo que colocarlas en -ancho/2 del sprite

	|....+....|....+....|....+....|....+....|
	0		 25			50		  75		100

	*/

	for(int i = 0; i < numba; i++){
		// intenta calcular como dividir el espacio para que quepan num casitas, dejando margenes a los lados y situando cada una en la mitad del punto (x - anchoCasa/2)
		creaCasita(Vec2(margen + separa/2.0f + i*separa - casaWidth/2.0f, 120.0f));
	}

}

void Level::creaCasita(Vec2 esquinaInfIzq){
	// HACK: solo cargo este sprite para calcular su tama�o
	Sprite *spr = Sprite::createWithSpriteFrameName(gameInstance->sprite_casa_bloque.c_str());
	auto blockSize = spr->getContentSize();
	Vec2 posBloque;
	GameActor *bloque;
	
	for(int j = 0; j < 4; j++){
		for(int i = 0; i < 4; i++){
			posBloque = esquinaInfIzq;
			// ojo que 0,0 es el centro del sprite, si cuento con que empiece en la esquina inferior izquierda tengo que desplazarlo la mitad de su ancho. El alto tambi�n, pero ahora me la pela :P
			posBloque.x += i * (blockSize.width-1) + blockSize.width/2.0f;
			posBloque.y += j * (blockSize.height-1);

			// hace el tejado de forma sucia
			if(i == 0 && j == 3){
				// esquina superior izquierda?
				bloque = creaDestructible(posBloque,1);
			} else if(i == 3 && j == 3){
				// esquina superior derecha?
				bloque = creaDestructible(posBloque,2);
			} else{
				// pos bloque est�ndar
				bloque = creaDestructible(posBloque);
			}
		}
	}
}

GameActor *Level::creaDestructible(Vec2 pos, int type){
	GameActor *tmp = new GameActor();
	tmp->type_ = GameActor::gameActorTypes::destructible;
	tmp->setSonidoImpacto(Game::getInstance()->game_sonido_casa_impacto.c_str());
	const char *spritePath;

	switch(type){
	case 0:
		spritePath = gameInstance->sprite_casa_bloque.c_str();
		tmp->setCadaver(this, gameInstance->devuelveBloqueRotoAleatorio(), "cachotrozo", 0, 0);
		break;
	case 1:
		spritePath = gameInstance->sprite_casa_esquina_izq.c_str();
		tmp->setCadaver(this, gameInstance->sprite_casa_bloque_roto_izq.c_str(), "cachotrozo", 0, 0);
		break;
	case 2:
		spritePath = gameInstance->sprite_casa_esquina_dch.c_str();
		tmp->setCadaver(this, gameInstance->sprite_casa_bloque_roto_dch.c_str(), "cachotrozo", 0, 0);
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
		// me tendr�a que asegurar que gameState existe antes del delete. A priori... s�...
		delete instance->gameState;
		instance->gameState = nuevo;
	} else{
		CCLOG("Instance sin inicializar en Level");
	}
}

void Level::vuelveAlMenu(){
	Director::getInstance()->replaceScene(Menus::createScene());
}

// OJO static function
void Level::apuntaHiScore(){
	instance->apuntandoRecords_ = true;
	instance->gameInstance->lblHiScoreName->setString("");
	instance->gameInstance->lblHiScoreName->setVisible(true);
}

void Level::afterCaptured(bool succeed, const std::string& outputFile){
	if(succeed){
		log("Capture screen succeeded.");
		//// show screenshot
		//auto sp = Sprite::create(outputFile);
		//addChild(sp, 0, childName);
		//Size s = Director::getInstance()->getWinSize();
		//sp->setPosition(s.width / 2, s.height / 2);
		//sp->setScale(0.25);
	} else{
		log("Capture screen failed.");
	}
}

void Level::pressedKey(cocos2d::EventKeyboard::KeyCode kc){

	if(kc == cocos2d::EventKeyboard::KeyCode::KEY_BACKSPACE || kc == cocos2d::EventKeyboard::KeyCode::KEY_DELETE){
		// borra
		if(playerName_.length() > 1){
			playerName_.erase(playerName_.length() - 1);
		}
	} else if (kc == cocos2d::EventKeyboard::KeyCode::KEY_ENTER){
		// aqu� se acaba el estado de capturar el nombre...
		apuntandoRecords_ = false;

		// oleadaNum empieza en 0
		gameInstance->hiScores_->insertHiScore(playerName_, std::to_string(oleadaNum_+1), gameInstance->puntos);

		// guardo la tabla de records
		gameInstance->hiScores_->guardaTablaRecords();

		// y se va a mostrar la tabla de records
		Director::getInstance()->replaceScene(HiScoresScene::createScene());

	} else{
	playerName_ += keyCodeToChar(kc);
}
}

// convierte keycodes de cocos2d en simples char (en may�scula me vale)
const unsigned char Level::keyCodeToChar(const cocos2d::EventKeyboard::KeyCode kc){
	// KeyCode es un enum y no parece estar relacionado con su c�digo ascii as� que...

	if(kc == cocos2d::EventKeyboard::KeyCode::KEY_SPACE){
		return ' ';
	}

	if(kc >= cocos2d::EventKeyboard::KeyCode::KEY_0 && kc <= cocos2d::EventKeyboard::KeyCode::KEY_9){
		unsigned char s = (unsigned char)kc - (unsigned char)cocos2d::EventKeyboard::KeyCode::KEY_0 + '0';

		return s;
	}


	if(kc >= cocos2d::EventKeyboard::KeyCode::KEY_A && kc <= cocos2d::EventKeyboard::KeyCode::KEY_Z){
		unsigned char s = (unsigned char)kc - (unsigned char)cocos2d::EventKeyboard::KeyCode::KEY_A + 'A';

		return s;
	}

	return '?';
}