#pragma once

#include "cocos2d.h"		// necesario para EventKeyboard::*
//#include "Player.h"

class Player;

USING_NS_CC;

class InputComponent{
public:
	InputComponent();
	~InputComponent();

	void keyPressed(EventKeyboard::KeyCode);
	void keyReleased(EventKeyboard::KeyCode);

	// TODO: pruebas, hacerlo private
	Player *player;

private:
};

