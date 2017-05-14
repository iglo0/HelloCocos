#include "InputComponent.h"

#include "Player.h"

InputComponent::InputComponent(){}

InputComponent::~InputComponent(){}

void InputComponent::keyPressed(EventKeyboard::KeyCode keyCode){
	//CCLOG("Inputcomponent %d", keyCode);

	switch(keyCode){
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		player->mueveIzq = true;
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		player->mueveDch = true;
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		//player->mueveArr = true;
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		//player->mueveAbj = true;
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_CTRL:
	case EventKeyboard::KeyCode::KEY_LEFT_CTRL:
	case EventKeyboard::KeyCode::KEY_SPACE:
		player->disparar = true;
		break;
	default:
		break;
	}
}

void InputComponent::keyReleased(EventKeyboard::KeyCode keyCode){
	//CCLOG("Inputcomponent released %d", keyCode);

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
	case EventKeyboard::KeyCode::KEY_RIGHT_CTRL:
	case EventKeyboard::KeyCode::KEY_LEFT_CTRL:
	case EventKeyboard::KeyCode::KEY_SPACE:
		player->disparar = false;
		break;
	default:
		break;
	}
}

