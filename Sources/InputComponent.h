#pragma once

#include "cocos2d.h"
#include "Jugador.h"

USING_NS_CC;

class InputComponent {

public:

	InputComponent(Jugador *j);

	// TODO: yyy no se como 

	// keyboard listeners
	virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

private:
	Jugador *player;
};
