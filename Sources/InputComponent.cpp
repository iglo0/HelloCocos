#include "InputComponent.h"

InputComponent::InputComponent(){
	mueveAbj = mueveArr = mueveDch = mueveIzq = false;
	dispara = false;
	sale = false;

	//auto listener = EventListenerKeyboard::create();
	//listener->onKeyPressed = CC_CALLBACK_2(InputComponent::onKeyPressed, level1);
	//listener->onKeyReleased = CC_CALLBACK_2(InputComponent::onKeyReleased, level1);
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

}

//void InputComponent::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event){
//	//log("Key with keycode %d pressed", keyCode);
//
//	switch(keyCode){
//	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
//		mueveIzq = true;
//		break;
//	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
//		mueveDch = true;
//		break;
//	case EventKeyboard::KeyCode::KEY_UP_ARROW:
//		mueveArr = true;
//		break;
//	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
//		mueveAbj = true;
//		break;
//	case EventKeyboard::KeyCode::KEY_CTRL:
//	case EventKeyboard::KeyCode::KEY_RIGHT_CTRL:
//	case EventKeyboard::KeyCode::KEY_SPACE:
//		// fiyah!
//		dispara = true;
//		break;
//	case EventKeyboard::KeyCode::KEY_ESCAPE:
//		sale = true;
//	default:
//		break;
//	}
//}
//
//void InputComponent::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event){
//	//log("Key with keycode %d released", keyCode);
//
//	switch(keyCode){
//	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
//		mueveIzq = false;
//		break;
//	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
//		mueveDch = false;
//		break;
//	case EventKeyboard::KeyCode::KEY_UP_ARROW:
//		mueveArr = false;
//		break;
//	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
//		mueveAbj = false;
//		break;
//	case EventKeyboard::KeyCode::KEY_CTRL:
//	case EventKeyboard::KeyCode::KEY_RIGHT_CTRL:
//	case EventKeyboard::KeyCode::KEY_SPACE:
//		// no fiyah!
//		dispara = false;
//		break;
//	default:
//		break;
//	}
//
//}

//void InputComponent::update(Jugador &player){
//	player.mueve(mueveIzq, mueveDch, mueveArr, mueveAbj);
//}

