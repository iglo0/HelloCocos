#include "InputComponent.h"

InputComponent::InputComponent(Jugador *j) {
	CCLOG("InputComponent Init");
	player = j;
}

void InputComponent::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event){
	//log("Key with keycode %d pressed", keyCode);

	// TODO: Mi idea no es mover directamente al jugador, estos eventos llegan independientemente de update ?
	// así que tengo que decirle de algún modo qué deberá hacer cuando le llegue su update
	switch(keyCode){
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		player->mueveIzq = true;
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		player->mueveDch = true;
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		player->mueveArr = true;
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		player->mueveAbj = true;
		break;
	case EventKeyboard::KeyCode::KEY_CTRL:
	case EventKeyboard::KeyCode::KEY_RIGHT_CTRL:
	case EventKeyboard::KeyCode::KEY_SPACE:
		// fiyah!
		player->disparo = true;
		break;
	case EventKeyboard::KeyCode::KEY_ESCAPE:
		player->sale = true;
	default:
		break;
	}
}

void InputComponent::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event){
	//log("Key with keycode %d released", keyCode);

	switch(keyCode){
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		player->mueveIzq = false;
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		player->mueveDch = false;
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		player->mueveArr = false;
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		player->mueveAbj = false;
		break;
	case EventKeyboard::KeyCode::KEY_CTRL:
	case EventKeyboard::KeyCode::KEY_RIGHT_CTRL:
	case EventKeyboard::KeyCode::KEY_SPACE:
		// no fiyah!
		player->disparo = false;
		break;
	default:
		break;
	}

}