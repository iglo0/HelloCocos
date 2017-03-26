#pragma once

#include "cocos2d.h"
#include "Player.h"

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

