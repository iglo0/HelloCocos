#include "GameState.h"

#include "Game.h"
#include "Pool.h"
#include "Player.h"
#include "Level.h"
#include "SpaceInvaders.h"

IntroNivelState::IntroNivelState(Player *p) : player(p){
	enterState();
}

void IntroNivelState::enterState(){
	gameInstance = Game::getInstance();

	Pool::disablePools();
	player->activatePlayerInInitialPos();
	tIni = gameInstance->ellapsedTime;
	gameInstance->lblMensajes->setString("PREPARATE!");
	gameInstance->lblMensajes->setVisible(true);

}

GameState *IntroNivelState::update(float deltaT) {

	if(gameInstance->ellapsedTime - tIni >= gameInstance->duracion_estado_intronivel){
		gameInstance->lblMensajes->setVisible(false);

		//iniciadoIntroNivel = false;
		//gameInstance->estadoActual = Game::estadosJuego::jugando;

		// TODO: ahora cojo y cambio de estado. Estee... ¿cómo?
		// así parece que va bien
		return new PlayingState(player);
	}
	
	return nullptr;
}


PlayingState::PlayingState(Player *p) : player(p){
	enterState();
}

void PlayingState::enterState(){}

GameState *PlayingState::update(float deltaT){
	// ---------------------------
	// PROTA
	// ---------------------------
	player->update(deltaT);

	// ---------------------------
	// UPDATE ALL THE THINGS!!
	// ---------------------------
	Pool::updateAll(deltaT);

	// Se ha acabado la oleada?
	if(SpaceInvaders::numInvadersVivos_ < 1){
		// TODO: ok pero entro en bucle, tengo que llamar a Level::avanzaOleada() de algún modo
		return new IntroNivelState(player);
	} else {
		return nullptr;
	}
}

PlayerDeadState::PlayerDeadState(Player *p): player(p){
	enterState();
}

void PlayerDeadState::enterState(){
	gameInstance = Game::getInstance();

	tIni = gameInstance->ellapsedTime;
	--gameInstance->vidas;
	gameInstance->actualizaVidas();
	gameInstance->lblMensajes->setString("MUERTO!");
	gameInstance->lblMensajes->setVisible(true);

}

GameState *PlayerDeadState::update(float deltaT){
	Pool::updateMinimal(deltaT);

	if(gameInstance->ellapsedTime - tIni >= gameInstance->duracion_estado_muerte){
		// cambio de estado
		if(gameInstance->vidas > 0){
			return new IntroNivelState(player);
		} else{
			return new GameOverState(player);
		}
	}

	return nullptr;
}

GameOverState::GameOverState(Player *p) : player(p){
	enterState();
}

void GameOverState::enterState(){
	gameInstance = Game::getInstance();

	tIni = gameInstance->ellapsedTime;

	gameInstance->lblMensajes->setString("GAME OVER!");
	gameInstance->lblMensajes->setVisible(true);

}

GameState *GameOverState::update(float deltaT){
	Pool::updateMinimal(deltaT);

	if(gameInstance->ellapsedTime - tIni >= gameInstance->duracion_estado_gameover){
		gameInstance->lblMensajes->setVisible(false);
		
		// so what now...
		//menuVuelveCallback(nullptr);

		CCLOG("Volver a los menus");

		// TODO: No se si llamarlo de algún modo desde level, o hacerlo desde aquí. Un include más...
		// vuelve al menu
		Level::vuelveAlMenu();
	}


	return nullptr;
}
