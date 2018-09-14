#include "GameState.h"

#include "Game.h"
#include "Pool.h"
#include "Player.h"
#include "Level.h"
#include "SpaceInvaders.h"
#include "HiScores.h"


#pragma region IntroNivelState

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

#pragma endregion


#pragma region PlayingState

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
		return new SiguienteNivelState(player);
	} else {
		return nullptr;
	}
}

#pragma endregion

#pragma region PlayerDead

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

#pragma endregion

#pragma region GameOver

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

		// TODO: en caso de hi score... ¿o siempre? apunta tu nombre y va a la pantalla de tabla de records

		CCLOG("Fin game over state");

		// TODO: No se si llamarlo de algún modo desde level, o hacerlo desde aquí. Un include más...
		// vuelve al menu
		//Level::vuelveAlMenu();
		//Level::pantallaHiScore();
		
		// Na, avisa a Level para que se encargue del Game Over (apuntar records, mostrar puntuaciones, volver al menú, por ejemplo?)
		//Level::gameOver();

		// Re-Na, pasa al siguiente estado
		return new ApuntarRecordState(player);

		// Y después la lógica del nivel toma el control y ya si eso se inicia de nuevo una partida o lo que sea
	}


	return nullptr;
}

#pragma endregion

#pragma region SiguienteNivel

SiguienteNivelState::SiguienteNivelState(Player *p) : player(p){
	enterState();
}

void SiguienteNivelState::enterState(){
	gameInstance = Game::getInstance();

	Pool::disablePools();
	//player->activatePlayerInInitialPos();
	tIni = gameInstance->ellapsedTime;
	gameInstance->lblMensajes->setString("SIGUIENTE OLEADA!");
	gameInstance->lblMensajes->setVisible(true);
}

GameState *SiguienteNivelState::update(float deltaT){
	if(gameInstance->ellapsedTime - tIni >= gameInstance->duracion_estado_intronivel){
		gameInstance->lblMensajes->setVisible(false);

		//iniciadoIntroNivel = false;
		//gameInstance->estadoActual = Game::estadosJuego::jugando;

		// TODO: ahora cojo y cambio de estado. Estee... ¿cómo? -> por ejemplo:
		Level::siguienteNivel();

		// así parece que va bien
		return new PlayingState(player);
	}

	return nullptr;
}

#pragma endregion

#pragma region ApuntarRecord

ApuntarRecordState::ApuntarRecordState(Player *p) : player(p) {
	enterState();
}

void ApuntarRecordState::enterState(){
	gameInstance = Game::getInstance();

	tIni = gameInstance->ellapsedTime;

	gameInstance->lblMensajes->setString("APUNTA TU NOMBRE!:");
	gameInstance->lblMensajes->setVisible(true);

	// here be dragons
	Level::apuntaHiScore();
}

GameState *ApuntarRecordState::update(float deltaT){

	// condicion de fin de apuntar records... ¿cuando el usuario pulsa enter?
	// lo consultaré en una variable de... ¿Level?
	if(1 == 0){

	}

	return nullptr;
}

#pragma endregion