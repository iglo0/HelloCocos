#include "Player.h"

#include "Game.h"
#include "Pool.h"
#include "Bullet.h"

// HACK: para cambiar de gamestate desde la muerte
#include "Level.h"
#include "GameState.h"

// OJO: variable estática
Vec2 Player::playerPosition;
Player *Player::playerInstance;

Player::Player(Node *nodo, float playerSpeed){

	// Empieza por el constructor de la base
	GameActor::GameActor();
	GameActor::type_ = GameActor::gameActorTypes::player;

	mueveIzq = mueveDch = mueveArr = mueveAbj = false;


	CCLOG("Constructor de Player");
	// Me aseguro que las variables estén inicializadas
	inputComponent = nullptr;
	// esto se lo paso desde fuera porque será el nivel el que sepa con qué arma empieza cada momento
	//currentWeapon = nullptr;
	
	// crea el sprite (desactivado: sin física ni visualización)
	//sprite = GameActor::setSprite(nodo, PLAYER_PATH_SPRITE, "player", (int)Game::CategoriaColision::Jugador, (int)Game::CategoriaColision::BalaEnemigo | (int)Game::CategoriaColision::Enemigo);
	sprite_ = GameActor::setSprite(nodo, Game::getInstance()->player_path_sprite.c_str(), "player", (int)Game::CategoriaColision::Jugador, (int)Game::CategoriaColision::BalaEnemigo | (int)Game::CategoriaColision::Enemigo);

	if(!sprite_){
		CCLOG("!!!Jugador sin sprite!!!");
		return;
	}

	sprite_->setScale(playerInitialScale);
	sprite_->setRotation(playerInitialRotation);

	// en vez de hacerlo aquí siempre, prefiero esperar a que se haga cuando toque (en el intro nivel por ejemplo)
	//activatePlayerInInitialPos();

	poolMisBalas = nullptr;

	//sprite = setSprite(nodo);

	disparar = false;

	// if Base::foo is not virtual then Derived::foo does not override Base::foo but hides it
	// C++ does not have a keyword for "the base class" (super or base) since it supports multiple inheritance which may lead to ambiguity.
	// Se puede hacer esto? ohhh...
	GameActor::gameActorSpeed_ = playerSpeed;
	
	// HACK: AL crear un jugador, actualizo el static playerInstance
	playerInstance = this;
}

Player::~Player(){
	playerInstance = nullptr;
}

void Player::update(float deltaT){
	// TODO: Primero el de la clase base?
	//GameActor::update(deltaT);
	muevePlayer();

	// TODO: GameActor::update moverá al prota, ahora actualizo su posicion
	// OJO!!: variable estática
	playerPosition = sprite_->getPosition();

	//CCLOG("Player update @%f", Game::getInstance()->ellapsedTime);

	// Ese se ha encargado del movimiento, ahora al resto:
	if(disparar){
		disparar = false;
		//currentWeapon->fire(getPosition());
		if(poolMisBalas){
			// TODO: no entiendo muy bien la sintaxis, ¿por qué tengo que usar *?
			Pool::activa(*poolMisBalas, sprite_->getPosition());
		}
	}
}

void Player::impacto(float dmg){
	//CCLOG("Player says OUCH!");
	
	killPlayer();

}


void Player::killPlayer(){
	//return;

	// HACK: cambio de estado usando una referencia al nivel
	Level::setGameState(new PlayerDeadState(this));
	// "deprecated"? o no?
	//Game::getInstance()->estadoActual = Game::estadosJuego::muerte;

	// TODO: matar al jugador, quitar vidas, cambiar de estado, etc
	// quién quita las vidas? player o level?
	desactiva();

}

void Player::activatePlayerInInitialPos(){
	// posición inicial centrado abajo
	Vec2 posInicial;
	posInicial.x = Director::getInstance()->getVisibleOrigin().x + Director::getInstance()->getVisibleSize().width / 2.0f;
	posInicial.y = sprite_->getScale()*sprite_->getContentSize().height / 2.0f;
	activa(posInicial);
}

Vec2 Player::getCurrentPlayerPosition(){
	return playerPosition;
}

Player *Player::getCurrentPlayer(){
	return playerInstance;
}

void Player::muevePlayer(){
	Vec2 pos = getPosition();
	float deltaT = Director::getInstance()->getDeltaTime();

	if(mueveIzq){
		pos.x -= gameActorSpeed_ * deltaT;
		if(pos.x < 0)
			pos.x = 0;
	}

	if(mueveDch){
		pos.x += gameActorSpeed_ * deltaT;
		if(pos.x > Director::getInstance()->getVisibleSize().width){
			pos.x = Director::getInstance()->getVisibleSize().width;
		}
	}

	if(mueveArr){
		pos.y += gameActorSpeed_ * deltaT;
		if(pos.y > Director::getInstance()->getVisibleSize().height){
			pos.y = Director::getInstance()->getVisibleSize().height;

			// HACK: lo siento, lo quitaré vale?
			if(sprite_->getTag() == (int)Game::CategoriaColision::BalaJugador){
				desactiva();
			}
		}
	}

	if(mueveAbj){
		pos.y -= gameActorSpeed_ * deltaT;
		if(pos.y < 0){
			pos.y = 0;
		}
	}

	setPosition(pos);


}