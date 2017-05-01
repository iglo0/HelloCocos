#include "Player.h"

Player::Player(Node *nodo, float playerSpeed){

	// Empieza por el constructor de la base
	GameActor::GameActor();

	CCLOG("Constructor de Player");
	// Me aseguro que las variables estén inicializadas
	inputComponent = nullptr;
	// esto se lo paso desde fuera porque será el nivel el que sepa con qué arma empieza cada momento
	//currentWeapon = nullptr;
	
	// crea el sprite (desactivado: sin física ni visualización)
	sprite = GameActor::setSprite(nodo, PLAYER_PATH_SPRITE, "player", (int)Game::CategoriaColision::Jugador, (int)Game::CategoriaColision::BalaEnemigo | (int)Game::CategoriaColision::Enemigo);

	if(!sprite){
		CCLOG("!!!Jugador sin sprite!!!");
		return;
	}

	sprite->setScale(playerInitialScale);
	sprite->setRotation(playerInitialRotation);

	// en vez de hacerlo aquí siempre, prefiero esperar a que se haga cuando toque (en el intro nivel por ejemplo)
	//activatePlayerInInitialPos();

	poolMisBalas = nullptr;

	//sprite = setSprite(nodo);

	disparar = false;

	// if Base::foo is not virtual then Derived::foo does not override Base::foo but hides it
	// C++ does not have a keyword for "the base class" (super or base) since it supports multiple inheritance which may lead to ambiguity.
	// Se puede hacer esto? ohhh...
	GameActor::gameActorSpeed = playerSpeed;
}

Player::~Player(){
}

void Player::update(float deltaT){
	// TODO: Primero el de la clase base?
	GameActor::update(deltaT);

	//CCLOG("Player update @%f", Game::getInstance()->ellapsedTime);

	// Ese se ha encargado del movimiento, ahora al resto:
	if(disparar){
		disparar = false;
		//currentWeapon->fire(getPosition());
		if(poolMisBalas){
			// TODO: no entiendo muy bien la sintaxis, ¿por qué tengo que usar *?
			Pool::activa(*poolMisBalas, sprite->getPosition());
		}
	}
}

void Player::impacto(float dmg){
	CCLOG("Player says OUCH!");

	killPlayer();

}


void Player::killPlayer(){

	Game::getInstance()->estadoActual = Game::estadosJuego::muerte;

	// TODO: matar al jugador, quitar vidas, cambiar de estado, etc
	// quién quita las vidas? player o level?
	desactiva();

}

void Player::activatePlayerInInitialPos(){
	// posición inicial centrado abajo
	Vec2 posInicial;
	posInicial.x = Director::getInstance()->getVisibleOrigin().x + Director::getInstance()->getVisibleSize().width / 2.0f;
	posInicial.y = sprite->getScale()*sprite->getContentSize().height / 2.0f;
	activa(posInicial);
}