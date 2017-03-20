#pragma once

#include "cocos2d.h"

USING_NS_CC;

// Mi clase Singleton (o es la idea) donde guardar el estado general del juego
class InputComponent{
public:

	InputComponent();

	// TODO: sacar esto a otro componente, InputKeyboardComponent para separar los distintos inputs
	// paso a paso
	// keyboard listeners
	//virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	//virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

	// invocar este metodo desde el update general
	//void update(Jugador &player);

private:
	// acciones
	bool mueveIzq, mueveDch, mueveArr, mueveAbj, dispara, sale;
	//Jugador &player;
};
