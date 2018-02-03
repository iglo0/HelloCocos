#pragma once

// menus, introNivel, jugando, finNivel, finHorda, muerte, gameOver 

class Player;
class Game;

//#include "Player.h"	// parece que necesito incluirlo para poder asignarle un valor

// Tal y como están, hay que instanciarlos para usarlos. Cada vez que quiera ir al estado X -> new estadoX...
// Y asegurar que se destruyen todo lo necesario al salir

class GameState {
public:
	virtual void enterState()=0;
	virtual GameState *update(float deltaT)=0;
};

class IntroNivelState : public GameState{
public:
	IntroNivelState(Player *p);

	void enterState() override;
	GameState *update(float deltaT) override;


private:
	float tIni;
	Player *player;
	Game *gameInstance;
};


class PlayingState : public GameState{
public:
	//PlayingState() : player(nullptr){};
	PlayingState(Player *p);

	void enterState() override;
	GameState *update(float deltaT) override;	// este estado no cambia internamente a otros estados (solo cambiaría por muerte, destruir oleadas, salir del juego... estímulos externos)

private:
	Player *player;
};

class PlayerDeadState : public GameState{
public:

	PlayerDeadState(Player *);

	void enterState() override;
	GameState *update(float) override;

private:
	float tIni;
	Game *gameInstance;
	Player *player;
};

class GameOverState : public GameState{
public:
	GameOverState(Player *);

	void enterState() override;
	GameState *update(float) override;

private:
	Player *player;
	Game *gameInstance;
	float tIni;
};

class SiguienteNivelState : public GameState{
public:
	SiguienteNivelState(Player *);

	void enterState() override;
	GameState *update(float) override;

private:
	Player *player;
	Game *gameInstance;
	float tIni;
};